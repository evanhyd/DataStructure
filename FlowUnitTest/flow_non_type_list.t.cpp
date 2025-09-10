#include "../DataStructure/flow_non_type_list.h"
#include <gtest/gtest.h>

namespace flow {
  // ---------------------- Append ----------------------
  TEST(NonTypeListTest, AppendToNonEmpty) {
    using L = NonTypeList<1, 2>;
    using Result = L::append<3>;
    static_assert(std::is_same_v<Result, NonTypeList<1, 2, 3>>);
  }

  TEST(NonTypeListTest, AppendToEmpty) {
    using L = NonTypeList<>;
    using Result = L::append<42>;
    static_assert(std::is_same_v<Result, NonTypeList<42>>);
  }

  // ---------------------- Prepend ----------------------
  TEST(NonTypeListTest, PrependToNonEmpty) {
    using L = NonTypeList<2, 3>;
    using Result = L::prepend<1>;
    static_assert(std::is_same_v<Result, NonTypeList<1, 2, 3>>);
  }

  TEST(NonTypeListTest, PrependToEmpty) {
    using L = NonTypeList<>;
    using Result = L::prepend<99>;
    static_assert(std::is_same_v<Result, NonTypeList<99>>);
  }

  // ---------------------- PopFront ----------------------
  TEST(NonTypeListTest, PopFrontNonEmpty) {
    using L = NonTypeList<1, 2, 3>;
    using Result = L::popFront;
    static_assert(std::is_same_v<Result, NonTypeList<2, 3>>);
  }

  TEST(NonTypeListTest, PopFrontSingleElement) {
    using L = NonTypeList<7>;
    using Result = L::popFront;
    static_assert(std::is_same_v<Result, NonTypeList<>>);
  }

  // ---------------------- PopBack ----------------------
  TEST(NonTypeListTest, PopBackNonEmpty) {
    using L = NonTypeList<1, 2, 3>;
    using Result = L::popBack;
    static_assert(std::is_same_v<Result, NonTypeList<1, 2>>);
  }

  TEST(NonTypeListTest, PopBackSingleElement) {
    using L = NonTypeList<5>;
    using Result = L::popBack;
    static_assert(std::is_same_v<Result, NonTypeList<>>);
  }

  // ---------------------- Set ----------------------
  TEST(NonTypeListTest, SetMiddleElement) {
    using L = NonTypeList<1, 2, 3>;
    using Result = L::set<1, 42>;
    static_assert(std::is_same_v<Result, NonTypeList<1, 42, 3>>);
  }

  TEST(NonTypeListTest, SetFirstElement) {
    using L = NonTypeList<9, 8, 7>;
    using Result = L::set<0, 100>;
    static_assert(std::is_same_v<Result, NonTypeList<100, 8, 7>>);
  }

  TEST(NonTypeListTest, SetSingleElement) {
    using L = NonTypeList<55>;
    using Result = L::set<0, 99>;
    static_assert(std::is_same_v<Result, NonTypeList<99>>);
  }

  // ---------------------- Size ----------------------
  TEST(NonTypeListTest, SizeOfEmpty) {
    using L = NonTypeList<>;
    static_assert(L::size == 0);
  }

  TEST(NonTypeListTest, SizeOfNonEmpty) {
    using L = NonTypeList<1, 2, 3, 4>;
    static_assert(L::size == 4);
  }

  // ---------------------- Data / Get ----------------------
  TEST(NonTypeListTest, DataAndGet) {
    using L = NonTypeList<10, 20, 30>;
    constexpr auto arr = L::data();
    static_assert(arr[0] == 10 && arr[1] == 20 && arr[2] == 30);
    static_assert(L::get(2) == 30);
  }

}
