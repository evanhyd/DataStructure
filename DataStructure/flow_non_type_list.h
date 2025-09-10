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

    static constexpr std::array<type, sizeof...(values)> data() {
      return { values... };
    }

    static constexpr auto get(std::size_t index) {
      return data()[index];
    }
  };

  template <>
  struct NonTypeList<> {
    constexpr static std::size_t size = 0;

    template <auto value>
    using append = Append<NonTypeList, value>::type;

    template <auto value>
    using prepend = Prepend<NonTypeList, value>::type;

    template <std::size_t index, auto value>
    using set = Set<NonTypeList, index, value>::type;
  };

  // NonTypeList type checking and concept.
  template <typename>
  struct IsNonTypeList : std::false_type {};

  template <auto... values>
  struct IsNonTypeList<NonTypeList<values...>> : std::true_type {};

  template <auto... values, auto value>
  struct Append<NonTypeList<values...>, value> {
    static_assert((std::is_same_v<decltype(value), decltype(values)> && ...), "value has a different type");
    using type = NonTypeList<values..., value>;
  };

  template <auto... values, auto value>
  struct Prepend<NonTypeList<values...>, value> {
    static_assert((std::is_same_v<decltype(value), decltype(values)> && ...), "value has a different type");
    using type = NonTypeList<value, values...>;
  };

  template <auto value, auto... values>
  struct PopFront<NonTypeList<value, values...>> {
    using type = NonTypeList<values...>;
  };

  template <auto value, auto... values>
  struct PopBack<NonTypeList<value, values...>> {
    using type = Prepend<
      typename PopBack<NonTypeList<values...>>::type,
      value>::type;
  };

  template <auto value>
  struct PopBack<NonTypeList<value>> {
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
}
