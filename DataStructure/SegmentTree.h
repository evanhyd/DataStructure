#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

template <typename T, typename BinOp>
class SegmentTree
{
    std::vector<T> tree_;

public:

    SegmentTree(std::initializer_list<T> lst);
    SegmentTree(std::vector<T> lst);

private:
    void BuildParent(BinOp op = BinOp());


public:
    void PointUpdate(int segment, const T& new_value, BinOp op = BinOp());
    T RangeQuery(int left, int right, BinOp op = BinOp()) const;

    void RangeUpdate(int left, int right, const T& new_value, BinOp op = BinOp());
    T PointQuery(int segment, BinOp op = BinOp()) const;
    
    void Print(int width) const;
};


template <typename T, typename BinOp>
SegmentTree<T, BinOp>::SegmentTree(std::initializer_list<T> lst) : tree_(lst.size() * 2)
{
    std::copy(lst.begin(), lst.end(), tree_.begin() + tree_.size() / 2);
    this->BuildParent();
}

template <typename T, typename BinOp>
SegmentTree<T, BinOp>::SegmentTree(std::vector<T> lst) : tree_(std::move(lst))
{
    tree_.resize(tree_.size() * 2);
    this->BuildParent();
}

template <typename T, typename BinOp>
void SegmentTree<T, BinOp>::BuildParent(BinOp op)
{
    for (int r = static_cast<int>(tree_.size()) / 2 - 1; r >= 1; --r)
    {
        tree_[r] = op(tree_[r * 2], tree_[r * 2 + 1]);
    }
}







//query type 0, point update + range query

template <typename T, typename BinOp>
void SegmentTree<T, BinOp>::PointUpdate(int segment, const T& new_value, BinOp op)
{
    segment += static_cast<int>(tree_.size()) / 2;
    tree_[segment] = new_value;
    while (segment >= 1)
    {
        segment /= 2;
        tree_[segment] = op(tree_[segment * 2], tree_[segment * 2 + 1]);
    }
}

template <typename T, typename BinOp>
T SegmentTree<T, BinOp>::RangeQuery(int left, int right, BinOp op) const
{
    //convert to the child leaf index
    left += static_cast<int>(tree_.size()) / 2;
    right += static_cast<int>(tree_.size()) / 2;

    //set up the initial query value
    T res = tree_[left];

    //check if left and right borders collide
    while(left <= right)
    {
        //apply the operators to the edge segment
        if (left % 2 == 1) res = op(res, tree_[left++]);
        if (right % 2 == 0) res = op(res, tree_[right--]);

        //climb up the tree
        left /= 2;
        right /= 2;
    }

    return res;
}



template <typename T, typename BinOp>
void SegmentTree<T, BinOp>::RangeUpdate(int left, int right, const T& new_value, BinOp op)
{
    left += static_cast<int>(tree_.size() / 2);
    right += static_cast<int>(tree_.size() / 2);

    while(left <= right)
    {
        if (left % 2 == 1)
        {
            tree_[left] = op(tree_[left], new_value);
            ++left;
        }

        if (right % 2 == 0)
        {
            tree_[right] = op(tree_[right], new_value);
            --right;
        }

        left /= 2;
        right /= 2;
    }
}

template <typename T, typename BinOp>
T SegmentTree<T, BinOp>::PointQuery(int segment, BinOp op) const
{
    segment += static_cast<int>(tree_.size() / 2);
}








template <typename T, typename BinOp>
void SegmentTree<T, BinOp>::Print(int width) const
{
    //notice the log2 and pow 2 does not cancel out each other due to int trancation
    //get the height of the tree
    int height = static_cast<int>(std::log2(tree_.size()));

    //get the number of elements at the bottom row
    int space = static_cast<int>(std::pow(2.0, height)) * width;

    for (int curr_tw = 0, max_tw = 1, r = 1; r < tree_.size(); ++r)
    {
        //get the length of each segment
        int segment_space = space / max_tw;

        //subtract the text length of the number
        std::string segment_text = std::to_string(tree_[r]);

        segment_space -= static_cast<int>(segment_text.size());
        segment_space = std::max(0, segment_space);
        int left = segment_space / 2;
        int right = left + segment_space % 2;

        std::cout << std::string(left, ' ') + segment_text + std::string(right, ' ');

        if (++curr_tw == max_tw)
        {
            curr_tw = 0;
            max_tw *= 2;
            std::cout << '\n';
        }
    }
}