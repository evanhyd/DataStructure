#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <climits>
#include <tuple>
#include <cassert>
#include <numeric>

using namespace std;

using ll = long long;
using ull = unsigned long long ;


//set<set<char>> game[10];
//
//char grid[3][3] = {};
//
//void Win(vector<pair<int, int>> vec)
//{
//    set<char> st;
//    for (auto& [a, b] : vec)
//    {
//        char l = grid[a][b];
//        st.insert(l);
//    }
//
//    game[st.size()].insert(st);
//}
//
//int main()
//{
//#if not defined _WIN32 && not defined _WIN64
//    const string kFileName = "tttt";
//    ifstream cin(kFileName + ".in");
//    ofstream cout(kFileName + ".out");
//#endif
//
//
//
//    vector<char> cow;
//
//    for (int i = 0; i < 3; ++i)
//    {
//        for (int j = 0; j < 3; ++j)
//        {
//            cin >> grid[i][j];
//        }
//    }
//
//
//    int cnt = 0, cnt_d = 0;
//
//    for (int i = 0; i < 3; ++i)
//    {
//        Win({ { i, 0 }, { i, 1 }, { i, 2 } });
//    }
//
//    for (int i = 0; i < 3; i++) {
//        Win({ {i, 0}, {i, 1}, {i, 2} });
//    }
//
//    for (int i = 0; i < 3; i++)
//    {
//        Win({ {0, i}, {1, i}, {2, i} });
//    }
//
//    Win({ {0, 0}, {1, 1}, {2, 2} });
//    Win({ {2, 0}, {1, 1}, {0, 2} });
//
//    cout << game[1].size() << '\n' << game[2].size();
//
//}