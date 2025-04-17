#pragma once

#include <unordered_map>

#include "weighted-graph.hpp"
#include "graph-types.h"

template <typename T>
void computeIndegrees(const WeightedGraph<T>& graph, std::unordered_map<value_type<T>, int>& indegrees) {
    for(auto vertex : graph) {
        indegrees[vertex.first] = 0;
    }

    for(auto vertex : graph) {
        for(auto edge : vertex.second) {
            ++indegrees[edge.first];
        }
    }
}
