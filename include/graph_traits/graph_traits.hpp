#pragma once

#include <utility>

#include "../type_traits/type_traits.hpp"

namespace graph::traits {
    namespace impl {
        template <typename Graph, typename SFINAE = void>
        struct get_size {};

        template <typename Edge, typename SFINAE = void>
        struct get_dest {};

        template <typename Edge, typename SFINAE = void>
        struct get_cost {};

        template <typename Graph, typename SFINAE = void>
        struct get_edges {};
    }

    /**
     * @brief Get the number of vertices of the given graph.
     * @tparam Graph needs specialization `get_size<Graph>` that has `operator()(Graph)`
     * @param g graph
     * @return number of vertices
     */
    template <typename Graph>
    decltype(auto) get_size(Graph&& g) { return impl::get_size<std::decay_t<Graph>>{}(std::forward<Graph>(g)); }

    /**
     * @brief Get the target node of the given edge.
     * @tparam Edge needs specialization `get_dest<Edge>` that has `operator()(Edge)`
     * @param e edge
     * @return target node
     */
    template <typename Edge>
    decltype(auto) get_dest(Edge&& e) { return impl::get_dest<std::decay_t<Edge>>{}(std::forward<Edge>(e)); }

    /**
     * @brief Get the cost of the given edge.
     * @tparam Edge needs specialization `get_cost<Edge>` that has `operator()(Edge)`
     * @param e edge
     * @return cost
     */
    template <typename Edge>
    decltype(auto) get_cost(Edge&& e) { return impl::get_cost<std::decay_t<Edge>>{}(std::forward<Edge>(e)); }

    /**
     * @brief Get edges connected to a node s
     * @tparam Graph needs specialization `get_edges<Graph>` that has `operator()(Graph, int)`
     * @param g graph
     * @param s node
     * @return iterable object that contains all edges connected to a node s
     */
    template <typename Graph>
    decltype(auto) get_edges(Graph&& g, int s) { return impl::get_edges<std::decay_t<Graph>>{}(std::forward<Graph>(g), s); }
}

namespace graph::traits::impl {
    // for types s.t. member function `size()` is defined.
    template <typename Graph>
    struct get_size<Graph, std::enable_if_t<type_traits::has_member_size_v<Graph>>> {
        int operator()(const Graph& g) { return g.size(); }
    };

    // for types s.t. `std::get<0>(Edge)` is valid.
    template <typename Edge>
    struct get_dest<Edge, std::enable_if_t<type_traits::has_kth_element_v<Edge, 0>>> {
        decltype(auto) operator()(const Edge& e) { return std::get<0>(e); }
        decltype(auto) operator()(Edge& e) { return std::get<0>(e); }
    };
    // for types s.t. `std::get<1>(Edge)` is valid.
    template <typename Edge>
    struct get_cost<Edge, std::enable_if_t<type_traits::has_kth_element_v<Edge, 1>>> {
        decltype(auto) operator()(const Edge& e) { return std::get<1>(e); }
        decltype(auto) operator()(Edge& e) { return std::get<1>(e); }
    };

    template <>
    struct get_dest<int> {
        const int& operator()(const int& v) { return v; }
        int& operator()(int& v) { return v; }
    };
    template <>
    struct get_cost<int> {
        int operator()(int) { return 1; }
    };

    // for types s.t. member function `operator[](int)` is valid
    template <typename Graph>
    struct get_edges<Graph, std::enable_if_t<type_traits::has_subscript_operator_v<Graph, int>>> {
        decltype(auto) operator()(const Graph& g, int u) { return g[u]; }
        decltype(auto) operator()(Graph& g, int u) { return g[u]; }
    };
}