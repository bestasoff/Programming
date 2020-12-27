#include <vector>
#include <mutex>
#include <thread>
#include <iostream>
#include <algorithm>
#include "logger.h"

using std::vector;
using std::lock_guard;
using std::mutex;

using matrix = vector<vector<int>>;


class Wrapper {
public:
    Wrapper(int a, int b, int c, int num) {
        A.resize(a, vector<int>(b));
        B.resize(b, vector<int>(c));
        C.resize(a, vector<int>(c));
        thread_num = num;
        bound1 = bound2 = bound3 = 0;
        FillMatrices();
    }

    void FillMatrices() {
        for (auto& vec : A) {
            for (auto& x : vec) {
                std::cin >> x;
            }
        }

        for (auto& vec : B) {
            for (auto& x : vec) {
                std::cin >> x;
            }
        }
    }

    void SimpleMul() {
        C.clear();
        C.resize(A.size(), vector<int>(B[0].size()));
        lock_guard<mutex> guard(mtex);
        for (int i = 0; i < A.size(); ++i) {
            for (int j = 0; j < B[0].size(); ++j) {
                for (int k = 0; k < B.size(); ++k) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

    void RowsToColumn() {
        C.clear();
        C.resize(A.size(), vector<int>(B[0].size()));
        int k = (thread_num > std::min(A.size(), B[0].size())) ?
                std::min(int(A.size()), int(B[0].size())) :
                thread_num;
        int row_delta = (int)A.size() / k;
        int col_delta = (int)B[0].size() / k;
        bound1 = row_delta * (k - 1);
        bound2 = col_delta * (k - 1);

        auto foo = [&](int i, int j) {
            RowsToColumnMul(
                    row_delta * i,
                    row_delta * (i + 1),
                    col_delta * j,
                    col_delta * (j + 1)
                    );
        };
        for (int i = 0; i < k; ++i) {
            threads.clear();
            for (int j = 0; j < k; ++j) {
                threads.emplace_back(foo, i, j);
            }

            for (auto& x : threads) {
                x.join();
            }
        }
    }

    void ColumnsToRows() {
        C.clear();
        C.resize(A.size(), vector<int>(B[0].size()));
        int k = thread_num;
        int col_delta;
        if (k > (int)A.size()) {
            k = (int)A.size();
            col_delta = 1;
            bound1 = k - 1;
        } else {
            col_delta = (int)A[0].size() / k;
            bound1 = col_delta * (k - 1);
        }

        auto foo = [&](int i) {
            ColumnsToRowsMul(
                    col_delta * i,
                    col_delta * (i + 1)
                    );
        };

        threads.clear();
        for (int i = 0; i < k; ++i) {
            threads.emplace_back(foo, i);
        }
        for (auto& x : threads) {
            x.join();
        }
    }

    void BlockToBlock() {
        C.clear();
        C.resize(A.size(), vector<int>(B[0].size()));
        int min = std::min(A.size(), std::min(B.size(), A[0].size()));
        int k = (thread_num > min) ? min : thread_num;
        int row_delta = (int)A.size() / k;
        int second_col_delta = (int)B[0].size() / k;
        int col_delta = (int)A[0].size() / k;
        bound1 = row_delta * (k - 1);
        bound2 = second_col_delta * (k - 1);
        bound3 = col_delta * (k - 1);

        auto foo = [&](int i, int j, int q) {
            BlockToBlockMul(
                    row_delta * i,
                    row_delta * (i + 1),
                    second_col_delta * j,
                    second_col_delta * (j + 1),
                    col_delta * q,
                    col_delta * (q + 1)
                    );
        };

        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                threads.clear();
                for (int q = 0; q < k; ++q) {
                    threads.emplace_back(foo, i, j, q);
                }

                for (auto& x : threads) {
                    x.join();
                }
            }
        }
    }

    void PrintResult() {
        for (auto& vec : C) {
            for (auto& x : vec) {
                cout << x << " ";
            }
            cout << endl;
        }
    }

private:
    int bound1, bound2, bound3;
    vector<std::thread> threads;
    int thread_num;
    mutex mtex;
    matrix A;
    matrix B;
    matrix C;

    void ColumnsToRowsMul(
            int begin,
            int end) {
        end = (begin == bound1) ? (int)A[0].size() : end;
        for (int i = 0; i < A.size(); ++i) {
            for (int j = 0; j < B[0].size(); ++j) {
                for (int k = begin; k < end; ++k) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

    void BlockToBlockMul(
            int ibegin, int iend,
            int jbegin, int jend,
            int kbegin, int kend
    ) {
        std::lock_guard<mutex> quard(mtex);
        iend = (ibegin == bound1) ? (int)A.size() : iend;
        jend = (jbegin == bound2) ? (int)B[0].size() : jend;
        kend = (kbegin == bound3) ? (int)A[0].size() : kend;

        for (int i = ibegin; i < iend; ++i) {
            for (int j = jbegin; j < jend; ++j) {
                for (int k = kbegin; k < kend; ++k) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

    void RowsToColumnMul(
            int ibegin,
            int iend,
            int jbegin,
            int jend) {
        iend = (ibegin == bound1) ? (int)A.size() : iend;
        jend = (jbegin == bound2) ? (int)B[0].size() : jend;

        for (int i = ibegin; i < iend; ++i) {
            for (int j = jbegin; j < jend; ++j) {
                for (int k = 0; k < A[0].size(); ++k) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

};
