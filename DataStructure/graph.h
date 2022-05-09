#pragma once
#include <vector>
#include <string>

namespace cug::graph
{
    using Vertex = int;
    using AdjacentList = std::vector<std::vector<Vertex>>;

    AdjacentList GetTree(int size);
    void PrintTreeDFS(const AdjacentList& tree);
    static std::string PrintTreeDFSHelper(const AdjacentList& tree, const Vertex curr, const int depth, int &max_depth);
    void PrintTreeBFS(const AdjacentList& tree);

}

