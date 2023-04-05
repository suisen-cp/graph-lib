#pragma once

#include <utility>
#include <vector>

#include "../graph_traits/graph_traits.hpp"

namespace graph {
    template <typename Edge>
    struct GraphCSR {
        using value_type = Edge;
        using const_iterator = typename std::vector<value_type>::const_iterator;
        using iterator = typename std::vector<value_type>::iterator;

        GraphCSR() = default;

        // edges: vector of pair { from, edge }
        GraphCSR(int n, const std::vector<std::pair<int, Edge>> &edges) : _n(n), _m(edges.size()), _edges(_m), _start(_n + 1) {
            for (const auto &[from, _] : edges) {
                ++_start[from];
            }
            for (int i = 1; i <= _n; ++i) {
                _start[i] += _start[i - 1];
            }
            for (const auto &[from, edge] : edges) {
                _edges[--_start[from]] = edge;
            }
        }

        int size() const {
            return _n;
        }

    private:
        template <typename Iterator>
        struct AdjacentListView {
            AdjacentListView(const Iterator &l, const Iterator &r) : _l(l), _r(r) {}

            int size() const { return _r - _l; }

            Edge& operator[](int i) { return *(_l + i);}
            const Edge& operator[](int i) const { return *(_l + i); }

            Iterator begin() const { return _l; }
            Iterator end() const { return _r; }
        private:
            Iterator _l, _r;
        };

    public:
        AdjacentListView<const_iterator> operator[](int u) const {
            return AdjacentListView<const_iterator>(_edges.begin() + _start[u], _edges.begin() + _start[u + 1]);
        }
        AdjacentListView<iterator> operator[](int u) {
            return AdjacentListView<iterator>(_edges.begin() + _start[u], _edges.begin() + _start[u + 1]);
        }

        static auto builder(int n) {
            struct Builder {
                friend GraphCSR<Edge>;

                Builder() = delete;

                void add_edge(int from, const Edge &edge) { _edges.emplace_back(from, edge); }

                GraphCSR<Edge> build() { return GraphCSR<Edge>(_n, std::move(_edges)); }
                GraphCSR<Edge> build_copy() const { return GraphCSR<Edge>(_n, _edges); }
            private:
                int _n;
                std::vector<std::pair<int, Edge>> _edges;

                Builder(int n) : _n(n) {}
            };
            return Builder{ n };
        }

    private:
        int _n, _m;
        std::vector<Edge> _edges;
        std::vector<int> _start;
    };
}
