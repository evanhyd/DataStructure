#pragma once
#include <array>
#include <concepts>
#include <type_traits>

namespace flow {
  /// @brief Append a value to the list.
  /// @tparam T The NonTypeList type.
  /// @tparam value The value to append.
  template <typename T, auto value> struct Append;

  /// @brief Prepend a value to the list.
  /// @tparam T The NonTypeList type.
  /// @tparam value The value to prepend.
  template <typename T, auto value> struct Prepend;

  /// @brief Remove the first element from the list.
  /// @tparam T The NonTypeList type.
  template <typename T> struct PopFront;

  /// @brief Remove the last element from the list.
  /// @tparam T The NonTypeList type.
  template <typename T> struct PopBack;

  /// @brief Set the element at the index to the new value in the list.
  /// @tparam T The NonTypeList type.
  /// @tparam index The index of the element.
  /// @tparam value The new value.
  template <typename T, std::size_t index, auto value> struct Set;

  /// @brief Splice out a list from the range [first, last).
  /// @tparam T The NonTypeList type.
  /// @tparam first The begin.
  /// @tparam last The end.
  template <typename T, std::size_t first, std::size_t last> struct Splice;

  // A compiled time homogeneous value list.
  template <auto... values>
  struct NonTypeList {
    using type = std::common_type_t<decltype(values)...>;

    static_assert((std::is_same_v<type, decltype(values)> && ...), "all the value must have the same types");

    constexpr static std::size_t size = sizeof...(values);

    template <auto value>
    using append = Append<NonTypeList, value>::type;

    template <auto value>
    using prepend = Prepend<NonTypeList, value>::type;

    using popFront = PopFront<NonTypeList>::type;

    using popBack = PopBack<NonTypeList>::type;

    template <std::size_t index, type value>
    using set = Set<NonTypeList, index, value>::type;

    template <std::size_t first, std::size_t last>
    using splice = Splice<NonTypeList, first, last>::type;

    /// @brief Return all the values in an array.
    /// @return An array of template value.
    static constexpr std::array<type, sizeof...(values)> data() {
      return { values... };
    }

    static constexpr auto get(std::size_t index) {
      return data()[index];
    }
  };

  template <>
  struct NonTypeList<> {
    using type = int;

    constexpr static std::size_t size = 0;

    template <auto value>
    using append = Append<NonTypeList, value>::type;

    template <auto value>
    using prepend = Prepend<NonTypeList, value>::type;

    template <std::size_t index, type value>
    using set = Set<NonTypeList, index, value>::type;

    template <std::size_t first, std::size_t last>
    using splice = Splice<NonTypeList, first, last>::type;

    /// @brief Return all the values in an array.
    /// @return An array of template value.
    static constexpr std::array<type, 0> data() {
      return {};
    }
  };

  // NonTypeList type checking and concept.
  template <typename>
  struct IsNonTypeList : std::false_type {};

  template <auto... vs>
  struct IsNonTypeList<NonTypeList<vs...>> : std::true_type {};

  template <auto... vs, auto v>
  struct Append<NonTypeList<vs...>, v> {
    static_assert((std::is_same_v<decltype(v), decltype(vs)> && ...), "value has a different type");
    using type = NonTypeList<vs..., v>;
  };

  template <auto... vs, auto v>
  struct Prepend<NonTypeList<vs...>, v> {
    static_assert((std::is_same_v<decltype(v), decltype(vs)> && ...), "value has a different type");
    using type = NonTypeList<v, vs...>;
  };

  template <auto v, auto... vs>
  struct PopFront<NonTypeList<v, vs...>> {
    using type = NonTypeList<vs...>;
  };

  template <auto v, auto... vs>
  struct PopBack<NonTypeList<v, vs...>> {
    using type = Prepend<
      typename PopBack<NonTypeList<vs...>>::type,
      v>::type;
  };

  template <auto v>
  struct PopBack<NonTypeList<v>> {
    using type = NonTypeList<>;
  };

  template <auto v, auto... vs, std::size_t index, auto sv>
  struct Set<NonTypeList<v, vs...>, index, sv> {
    using type = Prepend<
      typename Set<NonTypeList<vs...>, index - 1, sv>::type,
      v>::type;
  };

  template <auto v, auto... vs, std::size_t index, auto sv>
  requires (index == 0)
  struct Set<NonTypeList<v, vs...>, index, sv> {
    using type = NonTypeList<sv, vs...>;
  };

  template <auto v, auto... vs, std::size_t first, std::size_t last>
    requires (first < last && first > 0)
  struct Splice<NonTypeList<v, vs...>, first, last> {
    using type = Splice<NonTypeList<vs...>, first - 1, last - 1>::type;
  };

  template <auto v, auto... vs, std::size_t first, std::size_t last>
    requires (first < last && first == 0)
  struct Splice<NonTypeList<v, vs...>, first, last> {
    using type = Prepend<typename Splice<NonTypeList<vs...>, first, last - 1>::type, v>::type;
  };

  template <auto... vs, std::size_t first, std::size_t last>
    requires (first == last)
  struct Splice<NonTypeList<vs...>, first, last> {
    using type = NonTypeList<>;
  };
}
