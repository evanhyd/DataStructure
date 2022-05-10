#pragma once
#include <vector>
#include <string>

namespace cug::graph
{
    using Vertex = int;
    using AdjacentList = std::vector<std::vector<Vertex>>;

    AdjacentList GetTree(int size);
    void PrintTree(const AdjacentList& tree);
}

