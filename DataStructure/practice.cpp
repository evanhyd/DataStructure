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

#include "chapter1.h"
#include "random.h"


int main() {
  #ifdef ONLINE_JUDGE
  std::cin.tie(nullptr)->sync_with_stdio(false);
  #endif

  
  int nums[20] = {};
  for (int& n : nums) {
    n = cug::random::GetRandom(1, 50);
  }

  chapter1::MergeSort<int>(nums, nums + sizeof(nums) / sizeof(int));

  for (int& n : nums) {
    cout << n << ' ';
  }
  cout << '\n';
}


#endif


/*
overflow
array bounds
edge case n = 0, n = 1
reset between test cases
*/
