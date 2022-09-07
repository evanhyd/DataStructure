#pragma once
#include <iostream>

//Base Case
template <typename... Ts>
class Tuple {
  //empty
};


//Recursive Definition
//partial specialization, requires non empty template parameters
template <typename T, typename... Ts>
class Tuple<T, Ts...> {
  T val_;
  [[no_unique_address]] Tuple<Ts...>  tup_; //doesn't work on MSVC lmao

public:
  Tuple() = default;

  Tuple(T&& new_val, Ts&&... new_vals) :
    tup_(std::forward<Ts>(new_vals)...), val_(std::forward<T>(new_val)) {
    //empty
  }

  constexpr int Size() const {
    return sizeof...(Ts) + 1;
  }

  template <int N>
  auto& Get() {
    if constexpr (N == 0) return val_;

    //explicit template keyword requires for the parser
    //who tf designed the language syntax
    else return tup_.template Get<N - 1>();
  }

  template <int N>
  const auto& Get() const {
    if constexpr (N == 0) return val_;
    else return tup_.template Get<N - 1>();
  }
};

template <typename T, typename... Ts>
Tuple(T&&, Ts&&...)->Tuple<T, Ts...>;