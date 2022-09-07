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

    SparseTable(std::vector<T> arr, BinOp op = BinOp());
    T Query(int left, int right, BinOp op = BinOp()) const;
    T FastQuery(int left, int right, BinOp op = BinOp()) const;
    void Print(int width) const;

    static int Pow2(int power);
};

template <typename T, typename BinOp>
SparseTable<T, BinOp>::SparseTable(std::vector<T> arr, BinOp op)
{
    //base layer
    data_.push_back(std::move(arr));

    //increase height if the interval covering length <= arr size
    for (int h = 1; Pow2(h) <= data_[0].size(); ++h)
    {
        //allocate table length with estimated size
        int inter_len = Pow2(h - 1);
        data_.push_back(std::vector<T>(data_[h - 1].size() - inter_len));
        for (int r = 0; r < data_[h].size(); ++r)
        {
            data_[h][r] = op(data_[h - 1][r], data_[h - 1][r + inter_len]);
        }
    }
}

template <typename T, typename BinOp>
T SparseTable<T, BinOp>::Query(int left, int right, BinOp op) const
{
    //initial query result
    T res = data_[0][left];

    //top down
    for (int h = static_cast<int>(data_.size()) - 1; h >= 0; --h)
    {
        //check if the table includes the left index
        if (left < data_[h].size())
        {
            int inter_len = Pow2(h);
            int require_len = right - left + 1;

            //if the interval fits within the range
            if (inter_len <= require_len)
            {
                //query
                res = op(res, data_[h][left]);

                //update range
                left += inter_len;
            }
        }
    }

    return res;
}

template <typename T, typename BinOp>
T SparseTable<T, BinOp>::FastQuery(int left, int right, BinOp op) const
{

}

template <typename T, typename BinOp>
void SparseTable<T, BinOp>::Print(int width) const
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



template <typename T, typename BinOp>
int SparseTable<T, BinOp>::Pow2(int power)
{
    return 1 << power;
}