#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>

const int64_t kInf = INT64_MAX;

struct Edge {
    explicit Edge(int to, int64_t ww, int64_t cost)
        : to(to), ww(ww), cost(cost) {
    }
    int to;
    int64_t ww;
    int64_t cost;
};

int nn;
int mm;
std::vector<std::vector<Edge>> graph;
std::vector<int64_t> dist_start, dist_end;

void Dijkstra(std::vector<int64_t> &distance, int start) {
    distance[start] = 0;
    std::priority_queue<std::pair<int64_t, int>> que;
    que.push(std::make_pair(0, start));
    while (!que.empty()) {
        int vv = que.top().second;
        int64_t cur_d = -que.top().first;
        que.pop();
        if (cur_d > distance[vv]) {
            continue;
        }

        for (size_t jj = 0; jj < graph[vv].size(); ++jj) {
            int to = graph[vv][jj].to;
            int64_t len = graph[vv][jj].ww;
            if (distance[vv] + len < distance[to]) {
                distance[to] = distance[vv] + len;
                que.push(std::make_pair(-distance[to], to));
            }
        }
    }
}
