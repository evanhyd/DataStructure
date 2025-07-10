#pragma once
#include <type_traits>
#include <utility>

namespace flow {
  template <typename... Ts>
  class Tuple {
  public:
    constexpr std::size_t size() const {
      return 0;
    }
  };

  template <typename T, typename... Ts>
  class Tuple<T, Ts...> : Tuple<Ts...> {
    using BaseType = Tuple<Ts...>;
    T value_;

  public:
    constexpr Tuple() 
      : BaseType{}, value_{} {
    }

    template <typename Arg1, typename ...Args2>
    explicit constexpr Tuple(Arg1&& value, Args2&&... values)
      : BaseType(std::forward<Args2>(values)...), value_(std::forward<Arg1>(value)) {
    }

    constexpr std::size_t size() const {
      return sizeof...(Ts) + 1;
    }

    template <std::size_t N>
    auto& get() {
      if constexpr (N == 0) {
        return value_;
      } else {
        return BaseType::get<N - 1>();
      }
    }

    template <std::size_t N>
    const auto& get() const {
      if constexpr (N == 0) {
        return value_;
      } else {
        return BaseType::get<N - 1>();
      }
    }
  };

  template <typename T, typename ...Ts>
  Tuple(T&&, Ts&&...) -> Tuple<T, Ts...>;



  ////IsTuple implementation
  //namespace detail {
  //  template <typename T>
  //  struct IsTuple : std::false_type {};

  //  template <typename ...Ts>
  //  struct IsTuple<Tuple<Ts...>> : std::true_type {};
  //}

  //template <typename T>
  //constexpr bool is_tuple_v = detail::IsTuple<std::decay_t<T>>::value;

  ////TupleSize implementation
  //namespace detail {
  //  template <typename T>
  //  struct TupleSize {};

  //  template <typename ...Ts>
  //  struct TupleSize<Tuple<Ts...>> {
  //    static constexpr std::size_t value = sizeof...(Ts);
  //  };
  //}

  //template <typename T>
  //constexpr std::size_t tuple_size_v = detail::TupleSize<std::decay_t<T>>::value;

  ////MergedTuple implementation
  //namespace detail {
  //  template <typename T, typename ...Ts>
  //  struct MergedTuple {
  //    static_assert(is_tuple_v<T> && sizeof...(Ts) == 0, "concatenate non-tuple types");
  //    using type = T;
  //  };

  //  template <typename ...Ts1, typename ...Ts2, typename ...Tuples>
  //  struct MergedTuple<Tuple<Ts1...>, Tuple<Ts2...>, Tuples...> {
  //    using type = MergedTuple<Tuple<Ts1..., Ts2...>, Tuples...>::type;
  //  };
  //}

  //template <typename ...Ts>
  //using merged_tuple_t = detail::MergedTuple<std::decay_t<Ts>...>::type;

  //// TODO: generalize to N tuples.
  //template<typename T1, typename T2>
  //merged_tuple_t<T1, T2> merge_tuple(T1&& t1, T2&& t2) {
  //  return [&]<std::size_t ...INDEX1, std::size_t ...INDEX2>
  //    (std::index_sequence<INDEX1...>, std::index_sequence<INDEX2...>) {
  //    return merged_tuple_t<T1, T2>{
  //      std::move(t1).template get<INDEX1>()...,
  //      std::move(t2).template get<INDEX2>()...
  //    };
  //  }(std::make_index_sequence<tuple_size_v<T1>>(), std::make_index_sequence<tuple_size_v<T2>>());
  //}
}
