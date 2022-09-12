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
#include <numeric>

#include "AVLTree.h"
#include "random.h"
#include "memory.h"



using ll = long long;
using ull = unsigned long long;

constexpr ull kMod = 1000000007;
constexpr ull kPrimes[] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,
    43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101,
    103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
    241, 251, 257, 263, 269, 271, 277, 281, 283, 293 };

using namespace std;


#define IN_USE
#ifdef IN_USE
#define IN_USE


struct Num {
  int n;
  int i;

  bool operator<(const Num& rhs) const {
    return n < rhs.n;
  }

  bool operator==(const Num& rhs) const {
    return n == rhs.n;
  }
};


class Solution {
public:
  bool containsNearbyDuplicate(const vector<int>& nums, int k) {
    AVLTree<Num> tree;
    for (int i = 0; i < nums.size(); ++i) {
      auto res = tree.Find(Num{ nums[i], 0 });

      if (res) {
        if (abs(res->i - i) <= k) {
          return true;
        } else {
          tree.Erase(Num{ res->n, 0 });
          tree.Insert(Num{ nums[i], i });
        }
      }

      tree.Insert(Num{ nums[i], i });
    }
    return false;
  }
};


int main() {
  #ifdef ONLINE_JUDGE
  std::cin.tie(nullptr)->sync_with_stdio(false);
  #endif

  MSVC_MEMORY_GUARD();

  cout<<Solution().containsNearbyDuplicate({ 1, 0, 1, 1 }, 1);

}

/*
overflow
array bounds
edge case n = 0, n = 1
reset between test cases
*/

#endif