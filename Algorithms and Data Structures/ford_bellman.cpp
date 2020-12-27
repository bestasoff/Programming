int FordBellman(const std::vector<int>& scc, const std::vector<Edge>& edges) {
    dd[scc.front()] = 0;
    bool xx = false;
    for (size_t ii = 0; ii < scc.size(); ++ii) {
        xx = false;
        for (const auto& edge : edges) {
            if (dd[edge.from] + edge.cost < dd[edge.to]) {
                dd[edge.to] = dd[edge.from] + edge.cost;
                xx = true;
            }
        }
    }

    if (!xx) {
        return kInf;
    }

    int min = scc.front();
    for (const auto& el : scc) {
        min = std::min(min, el);
    }
    return min;
}
