#pragma once
#include <iostream>

template <typename... Ts>
class Tuple
{
    //empty
};

template <typename T, typename... Ts>
class Tuple<T, Ts...>
{
    T val_;
    Tuple<Ts...> tuple_;

public:

    Tuple(T&& new_val, Ts&... new_vals);
};


template <typename T, typename... Ts>
Tuple<T, Ts...>::Tuple(T&& new_val, Ts&... new_vals) :
    tuple_(std::forward<Ts>(new_vals)...), val_(std::forward<T>(new_val))
{

}