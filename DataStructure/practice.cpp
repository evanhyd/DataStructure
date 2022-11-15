#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <array>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <numeric>
#include <bitset>
#include <chrono>
#include <random>
#include <functional>

using ll = long long;
using ull = unsigned long long;

constexpr ull kMod = 1000000007;
constexpr ull kPrimes[] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,
    43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101,
    103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
    241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
};

using namespace std;


#define IN_USE
#ifdef IN_USE
#define IN_USE


template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
Matrix<T> MatrixMultiplyRecursive(const Matrix<T>& a, const Matrix<T>& b) {
  assert(!a.empty() && !b.empty() && a.front().size() == b.size());

  using std::size_t;
  const size_t I = a.size();
  const size_t J = b.size();
  const size_t K = b.front().size();
  Matrix<T> c(I, std::vector<T>(K, T{}));

  struct Range {
    size_t top, left;
    size_t bot, right;
  };


  auto mult = [&](const Range& c_r, const Range& a_r, const Range& b_r, auto&& mult) {
    const size_t kLen = c_r.bot - c_r.top + 1;
    const size_t kHLen= kLen / 2;

    //base case
    if (c_r.bot == c_r.top && c_r.right == c_r.left) {
      c[c_r.top][c_r.left] = a[a_r.top][a_r.left] * b[b_r.top][b_r.left];
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

    mult(c_r00, a_r00, b_r00, mult); mult(c_r00, a_r01, b_r10, mult);
    mult(c_r01, a_r00, b_r01, mult); mult(c_r01, a_r01, b_r11, mult);
    mult(c_r10, a_r10, b_r00, mult); mult(c_r10, a_r11, b_r10, mult);
    mult(c_r11, a_r10, b_r01, mult); mult(c_r11, a_r11, b_r11, mult);
  };

  const Range c_r = { 0, 0, I - 1, K - 1 };
  const Range a_r = { 0, 0, I - 1, J - 1 };
  const Range b_r = { 0, 0, J - 1, K - 1 };
  mult(c_r, a_r, b_r, mult);
  
  return c;
}


int main() {
  #ifdef ONLINE_JUDGE
  std::cin.tie(nullptr)->sync_with_stdio(false);
  #endif

  Matrix<int> a = {
    {1, 2},
    {4, 5},
  };

  Matrix<int> b = {
    {7, 8},
    {9, 10}
  };

  auto c = MatrixMultiplyRecursive(a, b);
  for (auto& row : c) {
    for (auto& col : row) {
      cout << col << ' ';
    }
    cout << '\n';
  }
}

/*
overflow
array bounds
edge case n = 0, n = 1
reset between test cases
*/
#endif
