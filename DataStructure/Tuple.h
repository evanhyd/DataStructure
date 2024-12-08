#pragma once
#include <type_traits>

namespace flow {
  template <typename... Ts>
  class Tuple {
  public:
    constexpr int size() const {
      return 0;
    }
  };

  template <typename T, typename... Ts>
  class Tuple<T, Ts...> : Tuple<Ts...> {
    T value_;

  public:
    constexpr Tuple() 
      : Tuple<Ts...>{}, value_{} {
    };

    explicit constexpr Tuple(T&& value, Ts&&... values)
      : Tuple<Ts...>(std::forward<Ts>(values)...), value_(std::forward<T>(value)) {
    }

    constexpr int size() const {
      return sizeof...(Ts) + 1;
    }

    template <std::size_t N>
    auto& get() {
      if constexpr (N == 0) {
        return value_;
      } else {
        //explicit template keyword requires for the parser
        //who tf designed the language syntax
        return Tuple<Ts...>::template get<N - 1>();
      }
    }

    template <std::size_t N>
    const auto& get() const {
      if constexpr (N == 0) {
        return value_;
      } else {
        return Tuple<Ts...>::template get<N - 1>();
      }
    }
  };

  template <typename T, typename ...Ts>
  Tuple(T&&, Ts&&...) -> Tuple<T, Ts...>;

  template <typename T>
  constexpr bool is_tuple = false;

  template <typename ...Args>
  constexpr bool is_tuple<Tuple<Args...>> = true;

  template <typename T, typename ...Ts>
  struct TupleCat {
    static_assert(is_tuple<T> && sizeof...(Ts) == 0, "concatenate non-tuple types");
    using type = T;
  };

  template <typename ...Args1, typename ...Args2, typename ...Tuples>
  struct TupleCat<Tuple<Args1...>, Tuple<Args2...>, Tuples...> {
    using type = TupleCat<Tuple<Args1..., Args2...>, Tuples...>::type;
  };
}
