#pragma once
#include <iostream>

//Base Case
template <typename... Ts>
class Tuple {
public:
  constexpr int Size() const {
    return 0;
  }
};

//Recursive Definition
//partial specialization, requires non empty template parameters
template <typename T, typename... Ts>
class Tuple<T, Ts...> : Tuple<Ts...> {
  T _data;

public:
  Tuple() = default;

  Tuple(T&& new_val, Ts&&... new_vals)
    : Tuple<Ts...>(std::forward<Ts>(new_vals)...), _data(std::forward<T>(new_val)) {
    //empty
  }

  constexpr int Size() const {
    return sizeof...(Ts) + 1;
  }

  template <int N>
  auto& Get() {
    if constexpr (N == 0) {
      return _data;
    } else {
      //explicit template keyword requires for the parser
      //who tf designed the language syntax
      return Tuple<Ts...>::template Get<N - 1>();
    }
  }

  template <int N>
  const auto& Get() const {
    if constexpr (N == 0) {
      return _data;
    } else {
      return Tuple<Ts...>::template Get<N - 1>();
    }
  }
};

template <typename T, typename... Ts>
Tuple(T&&, Ts&&...)->Tuple<T, Ts...>;
