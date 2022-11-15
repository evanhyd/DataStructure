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

    Matrix<T> c(I, std::vector<T>(K, T{}));
    for (size_t i = 0; i < I; ++i) {
      for (size_t k = 0; k < K; ++k) {
        for (size_t j = 0; j < J; ++j) {
          c[i][k] += a[i][j] * b[j][k];
        }
      }
    }
    return c;
  }
}