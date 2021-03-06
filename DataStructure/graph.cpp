#include "graph.h"
#include "random.h"
#include "io.h"

using cug::graph::AdjacentList;

AdjacentList cug::graph::GetTree(int size)
{
    AdjacentList graph(size);
    std::vector<Vertex> dfs_stack = { 0 };

    constexpr int kBacktrack = 0;
    constexpr int kAddDepth = 1;

    Vertex last_vertex_id = 0;

    // size > 1 to account for the root node 0
    while (size > 1)
    {
        int action = cug::random::GetRandom(kBacktrack, kAddDepth);

        switch (action)
        {
        case kBacktrack:
        {
            //dfs stack requires at least 1 vertex (root node)
            if (dfs_stack.size() > 1) dfs_stack.pop_back();
            break;
        }

        case kAddDepth:
        {
            ++last_vertex_id;

            //obtain the current vertex
            Vertex curr_vertex_id = dfs_stack.back();

            //add to the adjacent list
            graph[curr_vertex_id].push_back(last_vertex_id);

            //update dfs stack
            dfs_stack.push_back(last_vertex_id);
            --size;
            break;
        }

        default:
        {
            cug::io::Log("Error switch cases {}", action);
        }
        }
    }

    return graph;
}




static std::string PrintTreeHelper(const AdjacentList& tree, const cug::graph::Vertex curr, const int depth, int &max_depth, int &max_breadth)
{
    std::string tree_str(std::to_string(curr));
    if (!tree[curr].empty()) tree_str += " -> ";

    const std::string space((depth + 1) * 5, ' ');

    //reverse the order, smaller vertex id on the left
    for(auto child = tree[curr].rbegin(); child != tree[curr].rend(); ++child)
    {
        tree_str +=  "\n" + space + PrintTreeHelper(tree, *child, depth + 1, max_depth, max_breadth);
    }

    //update the max_depth
    max_depth = std::max(max_depth, depth);
    max_breadth = std::max(max_breadth , static_cast<int>(tree[curr].size()));

    return tree_str;
}

void cug::graph::PrintTree(const AdjacentList& tree)
{
    int max_depth = 0;
    int max_breadth = 0;
    std::string graph_str = PrintTreeHelper(tree, 0, 0, max_depth, max_breadth);
    std::string line_break(max_depth * 6, '-');

    cug::io::Output("\n{}\n{}\nMax Depth: {}\nMax Breadth: {}\n", graph_str, line_break, max_depth, max_breadth);
}