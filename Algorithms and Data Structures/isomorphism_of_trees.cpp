#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

int nn;
int order_marker;
std::vector<std::vector<int>> graph_x, graph_y;
std::vector<size_t> depth_var;
std::vector<size_t> ver_hashes_x, ver_hashes_y;
std::vector<int> y_order, x_order;
std::vector<int> result;

std::vector<int> Centroid(const std::vector<std::vector<int>>& graph) {
    int size = static_cast<int>(graph.size());
    std::vector<int> centroid;
    std::vector<int> sz(size);
    std::function<void (int, int)> dfs = [&](int uu, int prev) {
      sz[uu] = 1;
      bool is_centroid = true;
      for (auto vv : graph[uu]) {
          if (vv != prev) {
              dfs(vv, uu);
              sz[uu] += sz[vv];
              if (sz[vv] > size / 2) {
                  is_centroid = false;
              }
          }
      }
      if (size - sz[uu] > size / 2) {
          is_centroid = false;
      }
      if (is_centroid) {
          centroid.push_back(uu);
      }
    };
    dfs(0, -1);
    return centroid;
}

void Read(std::vector<std::vector<int>>& graph) {
    graph.resize(nn);
    for (int ii = 0; ii < nn - 1; ++ii) {
        int uu, vv;
        std::cin >> uu >> vv;
        graph[--uu].push_back(--vv);
        graph[vv].push_back(uu);
    }
}

size_t HashDFS(std::vector<size_t>& ver_hashes, int uu, int par,
            const std::vector<std::vector<int>>& graph, int& depth) {
    size_t res = 1;
    std::vector<size_t> hash_dfs;
    for (const auto& vv : graph[uu]) {
        if (vv != par) {
            int new_depth = 0;
            hash_dfs.push_back(HashDFS(ver_hashes, vv, uu, graph, new_depth));
            depth = std::max(new_depth + 1, depth);
        }
    }
    for (const auto& hash : hash_dfs) {
        res *= depth_var[depth] + hash;
        res %= 1'000'000'007;
    }
    ver_hashes[uu] = res;
    return res;
}

std::vector<size_t> Hashes() {
    std::vector<size_t> hashes(nn);
    std::mt19937_64 random(std::chrono::system_clock::now().time_since_epoch().count());
    for (int ii = 0; ii < nn; ++ii) {
        hashes[ii] = static_cast<size_t>(random()) % 1'000'000'007;
    }
    return hashes;
}

void OrderDFS(std::vector<int>& order, const std::vector<size_t>& hashes, int uu, int par,
              const std::vector<std::vector<int>>& graph) {
    order[order_marker++] = uu;
    std::vector<std::pair<size_t, int>> sorted_children;
    for (const auto& ver : graph[uu]) {
        sorted_children.emplace_back(hashes[ver], ver);
    }
    std::sort(sorted_children.begin(), sorted_children.end());
    for (const auto& vv : sorted_children) {
        if (vv.second != par) {
            OrderDFS(order, hashes, vv.second, uu, graph);
        }
    }
}

void CheckAndEnumerate(int y_ver, int x_ver) {
    if (ver_hashes_y[y_ver] != ver_hashes_x[x_ver]) {
        return;
    }

    order_marker = 0;
    OrderDFS(y_order, ver_hashes_y, y_ver, y_ver, graph_y);
    order_marker = 0;
    OrderDFS(x_order, ver_hashes_x, x_ver, x_ver, graph_x);
    result.resize(nn);
    for (int ii = 0; ii < nn; ++ii) {
        result[y_order[ii]] = x_order[ii];
    }
}


void Solve() {
    std::vector<int> centroids_x{Centroid(graph_x)}, centroids_y{Centroid(graph_y)};
    depth_var = Hashes();
    ver_hashes_x.resize(nn);
    ver_hashes_y.resize(nn);
    y_order.resize(nn);
    x_order.resize(nn);

    for (const auto& vv : centroids_y) {
        for (const auto& uu : centroids_x) {
            int depth = 0;
            HashDFS(ver_hashes_y, vv, vv, graph_y, depth);
            depth = 0;
            HashDFS(ver_hashes_x, uu, uu, graph_x, depth);
            CheckAndEnumerate(vv, uu);
        }
    }

    if (result.empty()) {
        std::cout << -1 << "\n";
        return;
    }
    for (const auto& index : result) {
        std::cout << index + 1 << "\n";
    }
}

int main() {
    std::cin >> nn;
    Read(graph_y);
    Read(graph_x);
    Solve();
    return 0;
}
