#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include "io.h"

using namespace cug::io;

template <typename T, typename BinOp>
    requires std::same_as<T, std::remove_const_t<decltype(BinOp::init_value)>>
class SegmentTree
{
    std::vector<T> tree_;

public:

    SegmentTree(std::initializer_list<T> lst);
    SegmentTree(const std::vector<T>& lst);


    void PrintTree(int width) const;
    T Query(int left, int right, BinOp op = BinOp()) const;
    void Update(int segment, const T& new_value, BinOp op = BinOp());

private:
    void BuildParent(BinOp op = BinOp());
};


template <typename T, typename BinOp>
    requires std::same_as<T, std::remove_const_t<decltype(BinOp::init_value)>>
SegmentTree<T, BinOp>::SegmentTree(std::initializer_list<T> lst) : tree_(lst.size() * 2)
{
    std::copy(lst.begin(), lst.end(), tree_.begin() + tree_.size() / 2);
    this->BuildParent();
}

template <typename T, typename BinOp>
    requires std::same_as<T, std::remove_const_t<decltype(BinOp::init_value)>>
SegmentTree<T, BinOp>::SegmentTree(const std::vector<T>& lst) : tree_(lst.size() * 2)
{
    std::copy(lst.begin(), lst.end(), tree_.begin() + tree_.size() / 2);
    this->BuildParent();
}


template <typename T, typename BinOp>
    requires std::same_as<T, std::remove_const_t<decltype(BinOp::init_value)>>
void SegmentTree<T, BinOp>::PrintTree(int width) const
{
    //notice the log2 and pow 2 does not cancel out each other due to int trancation
    //get the height of the tree
    int height = static_cast<int>(std::log2(tree_.size()));

    //get the number of elements at the bottom row
    int space = static_cast<int>(std::pow(2.0, height)) * width;

    for (int curr_tw = 0, max_tw = 1, i = 1; i < tree_.size(); ++i)
    {
        //get the length of each segment
        int segment_space = space / max_tw;

        //subtract the text length of the number
        int num_len = 1, temp_num = tree_[i];
        if (temp_num < 0) num_len += 1;
        while (temp_num /= 10)
        {
            ++num_len;
        }

        segment_space -= num_len;
        int left = segment_space / 2;
        int right = left + segment_space % 2;

        std::cout << std::string(left, ' ') << tree_[i] << std::string(right, ' ');

        if (++curr_tw == max_tw)
        {
            curr_tw = 0;
            max_tw *= 2;
            std::cout << '\n';
        }
    }
}

template <typename T, typename BinOp>
    requires std::same_as<T, std::remove_const_t<decltype(BinOp::init_value)>>
T SegmentTree<T, BinOp>::Query(int left, int right, BinOp op) const
{
    T res = BinOp::init_value;
    for (left += static_cast<int>(tree_.size()) / 2, right += static_cast<int>(tree_.size()) / 2;
        left <= right; left /= 2, right /= 2)
    {
        if (left % 2 == 1) res = op(res, tree_[left++]);
        if (right % 2 == 0) res = op(res, tree_[right--]);
    }

    return res;
}

template<typename T, typename BinOp>
    requires std::same_as<T, std::remove_const_t<decltype(BinOp::init_value)>>
void SegmentTree<T, BinOp>::Update(int segment, const T& new_value, BinOp op)
{
    segment += static_cast<int>(tree_.size()) / 2;
    tree_[segment] = new_value;
    while (segment >= 1)
    {
        segment /= 2;
        tree_[segment] = op(tree_[segment * 2], tree_[segment * 2 + 1]);
    }
}

template<typename T, typename BinOp>
    requires std::same_as<T, std::remove_const_t<decltype(BinOp::init_value)>>
void SegmentTree<T, BinOp>::BuildParent(BinOp op)
{
    for (int i = static_cast<int>(tree_.size()) / 2 - 1; i >= 1; --i)
    {
        tree_[i] = op(tree_[i * 2], tree_[i * 2 + 1]);
    }
}