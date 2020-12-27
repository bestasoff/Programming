#include <algorithm>
#include <iostream>
#include <vector>

int nn;
int mm;
std::vector<std::vector<int>> graph;
std::vector<std::vector<int>> tr_graph;
std::vector<int> scc_colors;
std::vector<bool> used;
std::vector<int> order;
std::vector<int> component;
std::vector<int> scc_sizes;
std::vector<int> components_degree;
int current_color = 0;

void AssistantDFS(int vv) {
    used[vv] = true;
    for (int uu : graph[vv]) {
        if (!used[uu]) {
            AssistantDFS(uu);
        }
    }
    order.push_back(vv);
}

void SCCDFS(int vv) {
    used[vv] = true;
    scc_colors[vv] = current_color;
    component.push_back(vv);
    for (int uu : tr_graph[vv]) {
        if (!used[uu]) {
            SCCDFS(uu);
        }
    }
}

int main() {
    std::cin >> nn >> mm;
    graph.resize(nn);
    tr_graph.resize(nn);
    used.resize(nn);
    scc_colors.resize(nn);

    for (int ii = 0; ii < mm; ++ii) {
        int uu, vv, res;
        std::cin >> uu >> vv >> res;
        --uu;
        --vv;
        if (res == 3) {
            continue;
        }
        if (res == 1) {
            graph[uu].push_back(vv);
            tr_graph[vv].push_back(uu);
            continue;
        }
        graph[vv].push_back(uu);
        tr_graph[uu].push_back(vv);
    }

    for (int ii = 0; ii < nn; ++ii) {
        if (!used[ii]) {
            AssistantDFS(ii);
        }
    }
    used.assign(nn, false);
    std::reverse(order.begin(), order.end());
    for (int ii = 0; ii < nn; ++ii) {
        int vv = order[ii];
        if (!used[vv]) {
            SCCDFS(vv);
            scc_sizes.push_back(component.size());
            component.clear();
            ++current_color;
        }
    }
    components_degree.resize(scc_sizes.size());
    for (int uu = 0; uu < nn; ++uu) {
        for (const int& vv : graph[uu]) {
            if (scc_colors[uu] != scc_colors[vv]) {
                ++components_degree[scc_colors[vv]];
            }
        }
    }
    std::vector<int> zero_degree_scc;
    for (int ii = 0; ii < static_cast<int>(components_degree.size()); ++ii) {
        if (components_degree[ii] == 0) {
            zero_degree_scc.push_back(ii);
        }
    }

    int min_size = scc_sizes[zero_degree_scc.front()];
    for (const auto& com_num : zero_degree_scc) {
        if (scc_sizes[com_num] < min_size) {
            min_size = scc_sizes[com_num];
        }
    }

    std::cout << nn - min_size + 1 << std::endl;

    return 0;
}
