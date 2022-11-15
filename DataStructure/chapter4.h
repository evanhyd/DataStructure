#pragma once
#include <vector>

namespace chapter4 {
  template <typename T>
  using Matrix = std::vector<std::vector<T>>;

  template <typename T>
  Matrix<T> MatrixMultiplyIterative(const Matrix<T>& a, const Matrix<T>& b) {
    assert(!a.empty() && !b.empty() && a.front().size() == b.size());

    using std::size_t;
    const size_t I = a.size();
    const size_t J = b.size();
    const size_t K = b.front().size();

    //O(I * K)
    Matrix<T> c(I, std::vector<T>(K, T{}));

    //O(I * J * K)
    for (size_t i = 0; i < I; ++i) {
      for (size_t k = 0; k < K; ++k) {
        for (size_t j = 0; j < J; ++j) {
          c[i][k] += a[i][j] * b[j][k];
        }
      }
    }
    return c;
  }


  template <typename T>
  Matrix<T> MatrixMultiplyRecursive(const Matrix<T>& a, const Matrix<T>& b) {
    assert(!a.empty() && !b.empty() && a.front().size() == b.size());

    using std::size_t;
    struct Range {
      size_t top, left;
      size_t bot, right;
    };

    const size_t I = a.size();
    const size_t J = b.size();
    const size_t K = b.front().size();

    //O(I * K)
    Matrix<T> c(I, std::vector<T>(K, T{}));

    auto mult = [&](const Range& c_r, const Range& a_r, const Range& b_r, auto&& mult) {
      const size_t kLen = c_r.bot - c_r.top;
      const size_t kHLen = kLen / 2;

      //base case, assume perfect 2^N square matrix
      if (kLen == 1) {
        c[c_r.top][c_r.left] += a[a_r.top][a_r.left] * b[b_r.top][b_r.left];
        return;
      }

      const Range c_r00{ c_r.top, c_r.left, c_r.bot - kHLen, c_r.right - kHLen };
      const Range c_r01{ c_r.top, c_r.left + kHLen, c_r.bot - kHLen, c_r.right };
      const Range c_r10{ c_r.top + kHLen, c_r.left, c_r.bot, c_r.right - kHLen };
      const Range c_r11{ c_r.top + kHLen, c_r.left + kHLen, c_r.bot, c_r.right };
      const Range a_r00{ a_r.top, a_r.left, a_r.bot - kHLen, a_r.right - kHLen };
      const Range a_r01{ a_r.top, a_r.left + kHLen, a_r.bot - kHLen, a_r.right };
      const Range a_r10{ a_r.top + kHLen, a_r.left, a_r.bot, a_r.right - kHLen };
      const Range a_r11{ a_r.top + kHLen, a_r.left + kHLen, a_r.bot, a_r.right };
      const Range b_r00{ b_r.top, b_r.left, b_r.bot - kHLen, b_r.right - kHLen };
      const Range b_r01{ b_r.top, b_r.left + kHLen, b_r.bot - kHLen, b_r.right };
      const Range b_r10{ b_r.top + kHLen, b_r.left, b_r.bot, b_r.right - kHLen };
      const Range b_r11{ b_r.top + kHLen, b_r.left + kHLen, b_r.bot, b_r.right };

      //recursively multiply the matrix by dividing into smaller square
      //8 * T(N / 2)
      mult(c_r00, a_r00, b_r00, mult); mult(c_r00, a_r01, b_r10, mult);
      mult(c_r01, a_r00, b_r01, mult); mult(c_r01, a_r01, b_r11, mult);
      mult(c_r10, a_r10, b_r00, mult); mult(c_r10, a_r11, b_r10, mult);
      mult(c_r11, a_r10, b_r01, mult); mult(c_r11, a_r11, b_r11, mult);
    };

    mult({ 0, 0, I, K }, { 0, 0, I, J }, { 0, 0, J, K }, mult);
    return c;
  }
}