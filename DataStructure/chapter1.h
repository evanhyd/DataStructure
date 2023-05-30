#pragma once
#include <iostream>

namespace chapter1 {

  /// <summary>
  /// <para>Sort the given interval via Insertion Sort.</para>
  /// <para>Time Complexity: O(N^2)</para>
  /// <para>Space Complexity: O(1)</para>
  /// </summary>
  /// <typeparam name="T"></typeparam>
  /// <param name="begin">: beginning address of the interval</param>
  /// <param name="end">: endding address of the interval</param>
  template<typename T>
  void InsertionSort(T* begin, T* end) {

    /*
    * Time: O(N ^ 2)
    * Space: O(1)
    */
    for (T* pos = begin; pos != end; ++pos) { // N

      T* curr = pos;
      T key = std::move(*pos);

      /*
      * Time: O(N)
      * Space: O(1)
      */
      while (curr != begin) { // N
        T* prev = curr; --prev;
        if (key < *prev) {
          *(curr--) = std::move(*prev);
        } else {
          break;
        }
      }

      *curr = std::move(key);
    }
  }


  /// <summary>
  /// <para>Sort the given interval via Merge Sort.</para>
  /// <para>Time Complexity: O(N*log(N))</para>
  /// <para>Space Complexity: O(N)</para>
  /// </summary>
  /// <typeparam name="T"></typeparam>
  /// <param name="begin">: beginning address of the interval</param>
  /// <param name="end">: endding address of the interval</param>
  template <typename T>
  void MergeSort(T* begin, T* end) {

    const auto total_sz = end - begin;
    if (total_sz == 1) {
      return;
    }

    /*
    * conquer
    * Time: O(MergeSort(N / 2))
    * Space: O(N/2)
    */
    const auto mid = begin + total_sz / 2;
    MergeSort(begin, mid); // ???
    MergeSort(mid, end); // ???

    /*
    * merge
    * Time: O(N)
    * Space: O(N)
    */
    const auto left_sz = mid - begin;
    const auto right_sz = end - mid;
    T* const left_begin = static_cast<T*>(operator new[](left_sz * sizeof(T))); // N/2
    T* const right_begin = static_cast<T*>(operator new[](right_sz * sizeof(T))); // N/2
    const T* const left_end = left_begin + left_sz;
    const T* const right_end = right_begin + right_sz;

    for (T* srce = begin, *vx = left_begin; vx != left_end; ++srce, ++vx) { // N/2
      new(vx) T(std::move(*srce));
    }

    for (T* srce = mid, *vx = right_begin; vx != right_end; ++srce, ++vx) { // N/2
      new(vx) T(std::move(*srce));
    }
    
    /*
    * combine
    * Time: O(N)
    * Space: O(1)
    */
    T* left = left_begin;
    T* right = right_begin;
    T* vx = begin;
    while (left != left_end && right != right_end) { // N
      if (*left < *right) {
        *(vx++) = std::move(*left++);
      } else {
        *(vx++) = std::move(*right++);
      }
    }

    while (left != left_end) { // N/2
      *(vx++) = std::move(*left++);
    }

    while (right != right_end) { // N/2
      *(vx++) = std::move(*right++);
    }

    delete[] left_begin;
    delete[] right_begin;
  }

}