#pragma once

#include <vector>
#include <stdexcept>

void Swap(std::vector<int>& vec, int j, int k) {
    int tmp = vec[j];
    vec[j] = vec[k];
    vec[k] = tmp;
}

bool NextPermutation(std::vector<int>& vec) {
    int n = static_cast<int>(vec.size());
    int j = n - 2;
    while (j != -1 && vec[j] >= vec[j + 1]) {
        --j;
    }
    if (j == -1) {
        return false;
    }
    int k = n - 1;

    while (vec[j] >= vec[k]) {
        --k;
    }
    Swap(vec, j, k);
    int l = j + 1, r = n - 1;
    while (l < r) {
        Swap(vec, l++, r--);
    }
    return true;
}

int Factorial(int a) {
    if (a == 0) {
        return 1;
    }
    int res = 1;
    for (int i = 2; i <= a; ++i) {
        res *= i;
    }
    return res;
}

std::vector<std::vector<int>> GeneratePermutations(size_t len) {
    std::vector<std::vector<int>> perm_matr(Factorial(len), std::vector<int>(len));
    std::vector<int> vec(len);
    int beg = 0;
    for (auto& x : vec) {
        x = beg++;
    }
    int i = 0;
    perm_matr[i++] = vec;
    while (NextPermutation(vec)) {
        perm_matr[i++] = vec;
    }
    return perm_matr;
}
