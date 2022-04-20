#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include "io.h"

using namespace cug::io;

template <typename T, typename BinOp>
class SegmentTree
{
    std::vector<T> tree_;

public:

    SegmentTree(std::initializer_list<T> lst);
    SegmentTree(const std::vector<T>& lst);


    void PrintTree(int width) const;

private:
    void BuildParent(BinOp op = BinOp());
};


template <typename T, typename BinOp>
SegmentTree<T, BinOp>::SegmentTree(std::initializer_list<T> lst) : tree_(lst.size() * 2)
{
    std::copy(lst.begin(), lst.end(), tree_.begin() + tree_.size() / 2);
    this->BuildParent();
}

template <typename T, typename BinOp>
SegmentTree<T, BinOp>::SegmentTree(const std::vector<T>& lst) : tree_(lst.size() * 2)
{
    std::copy(lst.begin(), lst.end(), tree_.begin() + tree_.size() / 2);
    this->BuildParent();
}


template <typename T, typename BinOp>
void SegmentTree<T, BinOp>::PrintTree(int width) const
{
    int height = static_cast<int>(std::log2(tree_.size()));
    int max_space = static_cast<int>(pow(2.0, height)) * width;

    for (int curr_tw = 0, max_tw = 1, curr_height = 0, i = 1; i < tree_.size(); ++i)
    {
        int num_len = static_cast<int>(std::log10(tree_[i])) + 1;
        //Output("{} && {} => {}\n", 4.0, curr_height, static_cast<int>(4.0, curr_height));
        int space =  std::min(max_space / static_cast<int>(std::pow(4.0, curr_height)) - num_len, 1);
        int left = space / 4 + space % 2, right = space / 4;
        std::cout << std::string(left, ' ') << tree_[i] << std::string(right, ' ');

        if (++curr_tw == max_tw)
        {
            curr_tw = 0;
            max_tw *= 2;
            ++curr_height;
            std::cout << '\n';
        }
    }
}



template<typename T, typename BinOp>
void SegmentTree<T, BinOp>::BuildParent(BinOp op)
{
    Log("tree size {}\n", tree_.size());
    
    for (int i = static_cast<int>(tree_.size()) / 2 - 1; i >= 1; --i)
    {
        tree_[i] = op(tree_[i * 2], tree_[i * 2 + 1]);
    }
}