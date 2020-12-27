#include <iostream>
#include <queue>
#include <vector>

struct Edge {
    Edge(int to, int cost) : to(to), cost(cost) {
    }
    int to, cost;
};

const int kInf = 1000000007;

int nn;
int mm;
std::vector<std::vector<Edge>> graph;
std::vector<std::queue<int>> queues;
std::vector<int> dd;

int GetVal(int idx) {
    int xx = queues[idx].front();
    queues[idx].pop();
    return xx;
}

int GetVertex() {
    bool ff = !queues[0].empty(), ss = !queues[1].empty(), tt = !queues[2].empty();
    int fv = ff ? dd[queues[0].front()] : kInf;
    int sv = ss ? dd[queues[1].front()] : kInf;
    int tv = tt ? dd[queues[2].front()] : kInf;
    if (fv <= sv && fv <= tv) {
        return GetVal(0);
    }
    if (sv <= fv && sv <= tv) {
        return GetVal(1);
    }
    return GetVal(2);
}

bool Empty() {
    return queues[0].empty() && queues[1].empty() && queues[2].empty();
}

int main() {
    std::cin >> nn >> mm;
    graph.resize(nn);
    dd.resize(nn, kInf);
    queues.resize(3);
    for (int ii = 0; ii < mm; ++ii) {
        int uu, vv, cost;
        std::cin >> uu >> vv >> cost;
        --uu;
        --vv;
        graph[uu].emplace_back(vv, cost);
    }

    int kk;
    std::cin >> kk;
    while (kk--) {
        int xx, yy;
        std::cin >> xx >> yy;
        --xx;
        --yy;

        queues[0].push(xx);
        dd[xx] = 0;

        while (!Empty()) {
            int vv = GetVertex();
            for (auto uu : graph[vv]) {
                if (dd[vv] + uu.cost < dd[uu.to]) {
                    dd[uu.to] = dd[vv] + uu.cost;
                    queues[uu.cost].push(uu.to);
                }
            }
        }

        std::cout << (dd[yy] == kInf ? -1 : dd[yy]) << "\n";

        queues.assign(3, std::queue<int>());
        dd.assign(nn, kInf);
    }

    return 0;
}
