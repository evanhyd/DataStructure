#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <tuple>
#include <cassert>
#include <numeric>
#include <cmath>
#include <functional>
#include <bitset>

using namespace std;

using ll = long long;
using ull = unsigned long long;



//#define IN_USE
#ifdef IN_USE
#define IN_USE




bool Evaluate(int data, int val) {

  //data: c fb bb fa aa
  //val fb fa dcba

  int a = data & 0b11;
  bool flip_a = data >> 2 & 0b1;

  int b = data >> 3 & 0b11;
  bool flip_b = data >> 4 & 0b1;
  int op = data >> 5 & 0b1;

  bool a_val = val >> a & 1;
  if (flip_a) a_val = !a_val;

  bool b_val = val >> b & 1;
  if (flip_b) b_val = !b_val;

  if (op) {
    return a_val && b_val;
  } else {
    return a_val || b_val;
  }
}

void PrintSym(int data) {

  int a = data & 0b11;
  bool flip_a = data >> 2 & 0b1;

  int b = data >> 3 & 0b11;
  bool flip_b = data >> 4 & 0b1;
  int op = data >> 5 & 0b1;

  string str;
  if (flip_a) str += "!";
  switch (a) {
    case 0:
      str += "a";
      break;
    case 1:
      str += "b";
      break;
    case 2:
      str += "c";
      break;
    case 3:
      str += "d";
      break;
    default:
      cout << "ops\n";
  }

  if (op) {
    str += "&";
  } else {
    str += "|";
  }

  if (flip_b) str += "!";
  switch (b) {
    case 0:
      str += "a";
      break;
    case 1:
      str += "b";
      break;
    case 2:
      str += "c";
      break;
    case 3:
      str += "d";
      break;
    default:
      cout << "ops\n";
  }

  cout << str;
}






int main() {
  #if not defined _WIN32 && not defined _WIN64
  const string kFileName = "pairup";
  ifstream in(kFileName + ".in");
  ofstream out(kFileName + ".out");
  cin.rdbuf(in.rdbuf());
  cout.rdbuf(out.rdbuf());
  #endif


  {

   /* bool conflict = false;
    for (int i = 0; i <= 0b111; ++i) {
      bool a = i & 1;
      bool b = i >> 1 & 1;
      bool c = i >> 2 & 1;

      bool p1 = (c || a) && (b);
      bool p2 = !(!c) || (a && b);

      if (p1 != p2) {
        printf("conflict at a: %d, b: %d, c: %d", a, b, c);
        conflict = true;
        break;
      }
    }

    if (!conflict) {
      cout << "great\n";
    }
    return 0;*/


    int cnt = 0;
    for (int a = 0; a <= 0b1111111; ++a) {

      if ((a & 0b11) == (a >> 3 & 0b11)) continue;


      for (int b = 0; b <= 0b1111111; ++b) {

        if (a == b) continue;
        if ((b & 0b11) == (b >> 3 & 0b11)) continue;

        for (int c = 0; c <= 0b1111111; ++c) {

          if (c == a) continue;
          if (c == b) continue;
          if ((c & 0b11) == (c >> 3 & 0b11)) continue;

          for (int d = 0; d <= 0b1111111; ++d) {

            if (d == a) continue;
            if (d == b) continue;
            if (d == c) continue;
            if ((d & 0b11) == (d >> 3 & 0b11)) continue;

            bool conflict = false;
            for (int val = 0; val <= 0b1111; ++val) {
              if ((Evaluate(a, val) & Evaluate(b, val)) != (!Evaluate(c, val) || Evaluate(d, val))) {
                conflict = true;
                break;
              }
            }

            if (!conflict) {
              cout << "(";
              PrintSym(a);
              cout << ") & (";
              PrintSym(b);
              cout << ") <=> !(";
              PrintSym(c);
              cout << ") | (";
              PrintSym(d);
              cout << ")\n";
              ++cnt;
            }

          }
        }
      }
    }

    cout << cnt << '\n';
  }

}
#endif




































































/*





*/