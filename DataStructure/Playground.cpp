#include "Playground.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;
using std::flush;
using std::vector;

namespace {

  void Hello(int id) {
    cout << "hello: " << id << '\n';
  }


  std::atomic<int> money = 0;
  void Earn(int total) {
    for (int i = 0; i < total; ++i) {
      money += 1;
    }
  }



  //not understand
  std::mutex m;
  std::condition_variable cv;
  bool ended = false;
  bool found_prime = false;
  int last_prime = 0;

  void FindPrime(int begin, int end) {

    for (; begin != end; ++begin) {
      std::unique_lock g(m);
      for (int low = 2, up = static_cast<int>(sqrt(begin)); low <= up; ++low) {
        if (begin % low == 0) {
          found_prime = true;
          last_prime = begin;
          cv.notify_one();
          break;
        }
      }
    }

    ended = true;
  }

  void ReportPrime() {

    while (!ended) {
      std::unique_lock g(m);
      if (found_prime) {
        cv.wait(g);
      }

      cout << last_prime << '\n';
    }
  }




  //future
  void Fib(int x, int n, unsigned long long & res) {
    auto fib = [](int n, auto&& fib) {
      if (n <= 2) return 1;
      else return fib(n - 1, fib) + fib(n - 2, fib);
    };
    res = fib(n, fib);
  }
}

void playground::Entry() {

  constexpr int kSize = 5;
  std::vector<std::thread> f;
  std::vector<unsigned long long> f1(kSize, 0);
  for (int i = 0; i < kSize; ++i) {
    f.push_back(std::thread(Fib, i, 47, std::ref(f1[i])));
  }
  
  cout << "calculating...\n";
  for (int i = 0; i < kSize; ++i) {
    f[i].join();
    cout << f1[i]<<'\n';
  }
}