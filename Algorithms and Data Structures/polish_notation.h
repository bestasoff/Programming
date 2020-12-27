#pragma once

#include <string>
#include <stdexcept>
#include <stack>
#include <sstream>

std::vector<std::string> Split(const std::string& string, const std::string& delimiter = " ") {
    std::vector<std::string> tokens;
    size_t prev = 0, pos;
    do {
        pos = string.find(delimiter, prev);
        if (pos == std::string::npos) {
            pos = string.length();
        }
        std::string token = string.substr(prev, pos - prev);
        tokens.push_back(token);
        prev = pos + delimiter.length();
    } while (pos < string.length() && prev < string.length());
    if (string.substr(pos, string.size()) == delimiter) {
        tokens.push_back("");
    }
    return tokens;
}

bool NotOp(const std::string& ex) {
    return (ex != "+") && (ex != "-") && (ex != "*");
}

int EvaluateExpression(const std::string& expression) {
    std::vector<std::string> expr = Split(expression);
    std::stack<int> res;
    for (int i = 0; i < static_cast<int>(expr.size()); ++i) {
        if (NotOp(expr[i])) {
            std::stringstream ss;
            ss << expr[i];
            int x;
            ss >> x;
            res.push(x);
            ss.clear();
        } else {
            int x2 = res.top();
            res.pop();
            int x1 = res.top();
            res.pop();
            if (expr[i] == "+") {
                res.push(x2 + x1);
            } else if (expr[i] == "-") {
                res.push(x1 - x2);
            } else {
                res.push(x1 * x2);
            }
        }
    }
    return res.top();
}
