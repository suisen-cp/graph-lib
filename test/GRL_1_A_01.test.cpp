#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A"

#include <iostream>

#include "../include/graph/csr_graph.hpp"
#include "../include/algorithm/dijkstra.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, s;
    std::cin >> n >> m >> s;

    using Edge = std::pair<int, int>;

    auto builder = graph::GraphCSR<Edge>::builder(n);
    for (int i = 0; i < m; ++i) {
        int u, v, d;
        std::cin >> u >> v >> d;
        builder.add_edge(u, { v, d });
    }

    std::vector<int> dist = graph::dijkstra<int>(builder.build(), s);

    for (int i = 0; i < n; ++i) {
        if (dist[i] == std::numeric_limits<int>::max()) {
            std::cout << "INF" << '\n';
        } else {
            std::cout << dist[i] << '\n';
        }
    }

    return 0;
}