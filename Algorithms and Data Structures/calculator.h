#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <stack>
#include <map>

class Tokenizer {
public:
    Tokenizer(std::istream* in) : in_(in) {
    }

    enum TokenType { kUnknown, kNumber, kSymbol, kEnd };

    void Consume() {
        used_ = true;
        while (!in_->eof() && in_->peek() == ' ') {
            in_->get();
        }
        if (in_->eof()) {
            type_ = TokenType::kEnd;
            return;
        }
        if (isdigit(in_->peek())) {
            std::string number;
            while (isdigit(in_->peek())) {
                number += in_->get();
            }
            number_ = std::stod(number);
            type_ = TokenType::kNumber;
            return;
        }
        char c = in_->peek();
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            in_->get();
            symbol_ = c;
            type_ = TokenType::kSymbol;
            return;
        }
        type_ = TokenType::kUnknown;
    }

    TokenType GetType() {
        return type_;
    }

    int64_t GetNumber() {
        return number_;
    }

    char GetSymbol() {
        return symbol_;
    }

    bool Used() const {
        return used_;
    }

private:
    std::istream* in_;

    TokenType type_ = TokenType::kUnknown;
    int64_t number_;
    char symbol_;

    bool used_ = false;
};

class Expression {
public:
    virtual ~Expression() {
    }
    virtual int64_t Evaluate() = 0;
};

class Number : public Expression {
public:
    explicit Number(const int64_t& number) : number_(number) {
    }

    int64_t Evaluate() final {
        return number_;
    }

private:
    int64_t number_;
};

class UnaryMinus : public Expression {
public:
    UnaryMinus(std::unique_ptr<Expression> number) : number_(std::move(number)) {
    }

    int64_t Evaluate() final {
        return -number_->Evaluate();
    }

private:
    std::unique_ptr<Expression> number_;
};

class BinaryOperation : public Expression {
public:
    BinaryOperation(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {
    }

protected:
    std::unique_ptr<Expression> left_, right_;
};

class BinaryPlus : public BinaryOperation {
public:
    explicit BinaryPlus(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : BinaryOperation(std::move(left), std::move(right)) {
    }

    int64_t Evaluate() final {
        return BinaryOperation::left_->Evaluate() + BinaryOperation::right_->Evaluate();
    }
};

class BinaryMinus : public BinaryOperation {
public:
    explicit BinaryMinus(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : BinaryOperation(std::move(left), std::move(right)) {
    }

    int64_t Evaluate() final {
        return BinaryOperation::left_->Evaluate() - BinaryOperation::right_->Evaluate();
    }
};

class BinaryMul : public BinaryOperation {
public:
    explicit BinaryMul(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : BinaryOperation(std::move(left), std::move(right)) {
    }

    int64_t Evaluate() final {
        return BinaryOperation::left_->Evaluate() * BinaryOperation::right_->Evaluate();
    }
};

class BinaryDel : public BinaryOperation {
public:
    explicit BinaryDel(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : BinaryOperation(std::move(left), std::move(right)) {
    }

    int64_t Evaluate() final {
        return BinaryOperation::left_->Evaluate() / BinaryOperation::right_->Evaluate();
    }
};

std::unique_ptr<Expression> BinaryOps(Tokenizer* tok, std::map<char, int>& precedence,
                                      int last_precedence, int& plus_minus, int& mul_div);

std::unique_ptr<Expression> UnaryOps(Tokenizer* tok, std::map<char, int>& precedence,
                                     int& plus_minus, int& mul_div) {
    auto previous_type = tok->GetType();
    auto previous_symbol = tok->GetSymbol();
    tok->Consume();
    bool unary = (tok->GetType() == Tokenizer::kSymbol) &&
                 (previous_type == Tokenizer::kUnknown || previous_type == Tokenizer::kSymbol);
    if (unary && tok->GetSymbol() == '-') {
        char operation = tok->GetSymbol();
        auto t = BinaryOps(tok, precedence, precedence[operation], ++plus_minus, mul_div);
        if (previous_symbol != '+' && tok->GetSymbol() != '+' && tok->GetSymbol() != '-' &&
            tok->GetSymbol() != ')') {
            tok->Consume();
        }
        return std::unique_ptr<UnaryMinus>(new UnaryMinus(std::move(t)));
    } else if (tok->GetSymbol() == '(' && tok->GetType() == Tokenizer::kSymbol) {
        int cur_plus = 0;
        int cur_mul = 0;
        auto t = BinaryOps(tok, precedence, 0, cur_plus, cur_mul);
        tok->Consume();
        return t;
    } else if (tok->GetType() == Tokenizer::kNumber) {
        auto t = tok->GetNumber();
        tok->Consume();
        return std::unique_ptr<Number>(new Number(t));
    }
}

std::unique_ptr<Expression> BinaryOps(Tokenizer* tok, std::map<char, int>& precedence,
                                      int last_precedence, int& plus_minus, int& mul_div) {
    auto t = UnaryOps(tok, precedence, plus_minus, mul_div);

    bool binary = tok->GetSymbol() != ')';
    while (binary && precedence[tok->GetSymbol()] >= last_precedence &&
           tok->GetType() != Tokenizer::kEnd) {
        char op = tok->GetSymbol();
        if (op == '+') {
            mul_div = 0;
            if (plus_minus == 0) {
                auto t1 = BinaryOps(tok, precedence, precedence[op], ++plus_minus, mul_div);
                t = std::unique_ptr<BinaryPlus>(new BinaryPlus(std::move(t), std::move(t1)));
            } else {
                --plus_minus;
                return t;
            }
        } else if (op == '-') {
            mul_div = 0;
            if (plus_minus == 0) {
                auto t1 = BinaryOps(tok, precedence, precedence[op], ++plus_minus, mul_div);
                t = std::unique_ptr<BinaryMinus>(new BinaryMinus(std::move(t), std::move(t1)));
            } else {
                --plus_minus;
                return t;
            }
        } else if (op == '*') {
            if (mul_div == 0) {
                auto t1 = BinaryOps(tok, precedence, precedence[op], plus_minus, ++mul_div);
                t = std::unique_ptr<BinaryMul>(new BinaryMul(std::move(t), std::move(t1)));
            } else {
                mul_div = 0;
                return t;
            }
        } else {
            if (mul_div == 0) {
                auto t1 = BinaryOps(tok, precedence, precedence[op], plus_minus, ++mul_div);
                t = std::unique_ptr<BinaryDel>(new BinaryDel(std::move(t), std::move(t1)));
            } else {
                mul_div = 0;
                return t;
            }
        }
        binary = tok->GetSymbol() != ')';
    }
    return t;
}

std::unique_ptr<Expression> ParseExpression(Tokenizer* tok) {
    std::map<char, int> precedence = {{'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'(', 3}, {')', 3}};

    int plus_minus = 0;
    int mul_div = 0;

    return BinaryOps(tok, precedence, 0, plus_minus, mul_div);
}
