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


//#define TEST
#ifdef TEST
template <typename T>
class Tree;

template <typename T>
void swap(Tree<T>& lhs, Tree<T>& rhs);

template <typename T>
class Tree {
  class Node;
  int a;

  friend void swap<T>(Tree& lhs, Tree& rhs);
};

template <typename T>
void swap(Tree<T>& lhs, Tree<T>& rhs) {
  lhs.a = rhs.a;
}



template <typename T>
void swap(typename Tree<T>::Node& lhs, typename Tree<T>::Node& rhs);

// definition of inner
template <typename T>
class Tree<T>::Node {
  int b;

public:
  friend void swap<T>(Tree<T>::Node& lhs, Tree<T>::Node& rhs);
};

template <typename T>
void swap(typename Tree<T>::Node& lhs, typename Tree<T>::Node& rhs) {
  lhs.b = rhs.b;
}

int main() {
  Tree<int> o1, o2;
  swap(o1, o2);
}

#else

int main() {
  #ifdef ONLINE_JUDGE
  std::cin.tie(nullptr)->sync_with_stdio(false);
  #endif

  MSVC_MEMORY_GUARD();
  AVLTree<int> t1, t2;

  for (int i = 0; i < 10; ++i) {
    t1.Insert(i);
    t2.Insert(i * i);
  }

  t1.Print();
  t2.Print();

  t1 = std::move(t2);
  t1.Print();
  t2.Print();
}

#endif

/*
overflow
array bounds
edge case n = 0, n = 1
reset between test cases
*/

#endif