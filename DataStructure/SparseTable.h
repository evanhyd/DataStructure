#pragma once
#include <iostream>
#include <iomanip>
#include <vector>


//idempotent operator requires
template <typename T, typename BinOp>
class SparseTable
{
    std::vector<std::vector<T>> data_;

public:

    SparseTable(const std::vector<T>& arr, BinOp op = BinOp());
    T Query(int left, int right) const;
    void PrintTable(int width) const;

    static int Pow2(int power);
};

template <typename T, typename BinOp>
SparseTable<T, BinOp>::SparseTable(const std::vector<T>& arr, BinOp op)
{
    //base layer
    data_.push_back(arr);

    //increase height if the interval covering length <= arr size
    for (int h = 1; Pow2(h) <= data_[0].size(); ++h)
    {
        //allocate table length with estimated size
        int inter_len = Pow2(h - 1);
        data_.push_back(std::vector<T>(data_[h - 1].size() - inter_len));
        for (int i = 0; i < data_[h].size(); ++i)
        {
            data_[h][i] = op(data_[h - 1][i], data_[h - 1][i + inter_len]);
        }
    }
}

template <typename T, typename BinOp>
int SparseTable<T, BinOp>::Pow2(int power)
{
    return 1 << power;
}

template <typename T, typename BinOp>
void SparseTable<T, BinOp>::PrintTable(int width) const
{
    for (const auto& row : data_)
    {
        for (const auto& column : row)
        {
            std::cout << std::setw(width) << column;
        }
        std::cout << '\n';
    }
}

//use default value from segment tree instead of custom init value