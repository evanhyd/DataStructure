#include "time.h"

using namespace std;
using namespace std::chrono;

void box::Timer::Start() {
  _begin = high_resolution_clock::now(); 
}

void box::Timer::Stop() { 
  _end = high_resolution_clock::now(); 
}