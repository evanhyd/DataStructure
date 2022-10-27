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

#include "DynamicArray.h"

void Print(const auto& arr) {
  cout << arr << ' ';
}


int main() {
  #ifdef ONLINE_JUDGE
  std::cin.tie(nullptr)->sync_with_stdio(false);
  #endif


  //create an array of ten "0"
  DynamicArray<int> arr0(10);
  arr0.ForEach(Print<int>);
  cout << '\n';


  //fill the array with increasing int value  starting from 2
  int val = 2;
  auto arr1 = arr0.Map([&](auto x) { return x + val++; });
  arr1.ForEach(Print<int>);
  cout << '\n';


  //filter out all the odd numbers
  auto arr2 = arr1.Filter([](auto x) { return x % 2 == 0; });
  arr2.ForEach(Print<int>);
  cout << '\n';


  //create strings with x repeated 'a'
  auto arr3 = arr2.Map([](auto x) {return string(x, 'a'); });
  arr3.ForEach(Print<string>);
  cout << '\n';
}


#endif


/*
overflow
array bounds
edge case n = 0, n = 1
reset between test cases
*/
