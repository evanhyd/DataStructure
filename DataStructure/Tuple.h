#pragma once
#include <type_traits>
#include <utility>

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

    template <typename Arg1, typename ...Args2>
    explicit constexpr Tuple(Arg1&& value, Args2&&... values)
      : Tuple<Ts...>(std::forward<Args2>(values)...), value_(std::forward<Arg1>(value)) {
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
  constexpr bool is_tuple_v = false;

  template <typename ...Args>
  constexpr bool is_tuple_v<Tuple<Args...>> = true;

  namespace detail {
    template <typename T, typename ...Ts>
    struct MergeTuple {
      static_assert(is_tuple_v<T> && sizeof...(Ts) == 0, "concatenate non-tuple types");
      using type = T;
    };

    template <typename ...Args1, typename ...Args2, typename ...Tuples>
    struct MergeTuple<Tuple<Args1...>, Tuple<Args2...>, Tuples...> {
      using type = MergeTuple<Tuple<Args1..., Args2...>, Tuples...>::type;
    };
  }

  template <typename ...Ts>
  using merged_tuple_t = detail::MergeTuple<Ts...>::type;

  template<typename ...Args1, typename ...Args2>
  Tuple<Args1..., Args2...> merge_tuple(Tuple<Args1...> t1, Tuple<Args2...> t2) {
    using MergedType = Tuple<Args1..., Args2...>;

    return [&]<std::size_t ...INDEX1, std::size_t ...INDEX2>
      (std::index_sequence<INDEX1...>, std::index_sequence<INDEX2...>) {
      return MergedType{t1.template get<INDEX1>()..., t2.template get<INDEX2>()...};
    }(std::make_index_sequence<sizeof...(Args1)>(), std::make_index_sequence<sizeof...(Args2)>());
  }
}
