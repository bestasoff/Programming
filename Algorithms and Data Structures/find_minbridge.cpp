#include <iostream>
#include <tuple>
#include <vector>

using std::vector;
using std::cin;
using std::cout;

struct Edge {
    Edge(int vertex, int weight) : vertex(vertex), weight(weight) {
    }
    bool operator==(Edge rhs) {
        return std::tie(vertex, weight) == std::tie(rhs.vertex, rhs.weight);
    }
    int vertex;
    int weight;
};

const int kMaxN = 50001;
int nn;
vector<Edge> graph[kMaxN];
vector<bool> used;
int timer;
vector<int> tin;
vector<int> fup;
int min_cost = -1;

void DFS(int vv, const Edge& pp = {-1, -1}) {
    used[vv] = true;
    tin[vv] = fup[vv] = timer++;
    for (Edge to : graph[vv]) {
        if (to == pp) {
            continue;
        }
        if (used[to.vertex]) {
            fup[vv] = std::min(fup[vv], tin[to.vertex]);
        } else {
            DFS(to.vertex, {vv, to.weight});
            fup[vv] = std::min(fup[vv], fup[to.vertex]);
            if (fup[to.vertex] > tin[vv]) {
                if (min_cost == -1) {
                    min_cost = to.weight;
                } else {
                    min_cost = std::min(to.weight, min_cost);
                }
            }
        }
    }
}

void FindMinBridge() {
    timer = 0;
    used.resize(nn, false);
    tin.resize(nn, -1);
    fup.resize(nn, -1);
    for (int ii = 0; ii < nn; ++ii) {
        if (!used[ii]) {
            DFS(ii);
        }
    }
}
