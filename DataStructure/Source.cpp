#include <iostream>
#include <numeric>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <execution>

#include <string>
#include <deque>
#include <queue>
#include <forward_list>
#include <map>
#include <unordered_map>

#include "DynamicArray.h"
#include "ForwardList.h"
#include "BinaryHeap.h"
#include "PSDA.h"

#include "io.h"
#include "memory.h"
#include "time.h"

using namespace std;

using DC = cug::memory::DebugClass;


int main()
{
    //MEMORY_GUARD(); //address sanitizer is incompatiable with this

    LOG(" ", "my", "favorite", "cat", "is", 123, 'c', "years old");
    DC a;
    cout << a << '\n';

    cug::time::Timer t;
    t.Start();
    t.Stop();
    t.Result();

    cout << a << '\n';
}