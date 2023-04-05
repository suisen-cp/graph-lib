#pragma once

#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

#include "../graph_traits/graph_traits.hpp"

namespace graph {
    template <typename Cost, typename Graph>
    std::vector<Cost> dijkstra(const Graph& g, int s) {
        using state = std::pair<Cost, int>;

        const int n = traits::get_size(g);

        std::vector<Cost> dist(n, std::numeric_limits<Cost>::max());

        std::priority_queue<state, std::vector<state>, std::greater<state>> pq{};
        pq.emplace(dist[s] = 0, s);

        while (pq.size()) {
            auto [du, u] = pq.top();
            pq.pop();
            if (du != dist[u]) {
                continue;
            }
            for (const auto& edge : traits::get_edges(g, u)) {
                int v = traits::get_dest(edge);
                Cost cost = traits::get_cost(edge);
                if (du + cost < dist[v]) {
                    pq.emplace(dist[v] = du + cost, v);
                }
            }
        }

        return dist;
    }
}