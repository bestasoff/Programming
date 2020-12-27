#pragma once

struct Sum {
    int operator()(int lhs, int rhs) {
        return lhs + rhs;
    }
};

struct Prod {
    int operator()(int lhs, int rhs) {
        return lhs * rhs;
    }
};

struct Concat {
    std::vector<int> operator()(const std::vector<int>& lhs, const std::vector<int>& rhs) {
        std::vector<int> res = lhs;
        res.insert(res.end(), rhs.begin(), rhs.end());
        return res;
    }
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    T res = init;
    while (first != last) {
        res = func(res, *first);
        ++first;
    }
    return res;
}

class Length {
public:
    explicit Length(int* init) : len_(init) {
    }

    template <class T>
    int operator()(T, T) {
        return ++(*len_);
    }

private:
    int* len_;
};
