#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <tuple>
#include <cassert>
#include <numeric>
#include <cmath>

using namespace std;

using ll = long long;
using ull = unsigned long long;


//struct P
//{
//    ll x, y;
//};
//
//int main()
//{   
//#if not defined _WIN32 && not defined _WIN64
//    const string kFileName = "circlecross";
//    ifstream cin(kFileName + ".in");
//    ofstream cout(kFileName + ".out");
//#endif
//
//    string line;
//    cin >> line;
//
//    vector<P> letter(26, {-1, -1});
//    for (int i = 0; i < 52; ++i)
//    {
//        char c = line[i] - 'A';
//        if (letter[c].x == -1) letter[c].x = i;
//        else letter[c].y = i;
//    }
//
//    int cnt = 0;
//    for (int i = 0; i < 26; ++i)
//    {
//        for (int j = 0; j < 26; ++j)
//        {
//            if (i == j) continue;
//            //cout << char(i + 'A') << " vs " << char(j + 'A') << '\n';
//            if (letter[i].x < letter[j].x && letter[j].x < letter[i].y && letter[i].y < letter[j].y) ++cnt;
//        }
//    }
//
//    cout << cnt << '\n';
//}