#include <iostream>
#include <numeric>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <execution>
#include <source_location>

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

using namespace cug::io;
using namespace cug::memory;
using namespace cug::time;

template <typename... Ts>
void log(Ts&&... ts, const source_location& loc = source_location::current())
{
    std::cout << loc.function_name() << " line " << loc.line() << ": ";

    ((std::cout << std::forward<Ts>(ts) << " "), ...);
    std::cout << '\n';
}

struct Bar
{
    int a;
    double b;
    string c;
};

void Test(auto* p)
{
    Pool::Deallocate(p);
    Pool::Deallocate(p);
}

int main()
{

    while (true)
    {

        try
        {
            auto* p = Pool::Allocate<Bar>(2, Bar(1, 2.0, "cat"), Bar(3, 4.5, "dog"));
            Print(',', p->a, p->b, p->c,'\n');
            Print(',', p[1].a, p[1].b, p[1].c, '\n');

            Pool::PrintPool();

            Test(p);

            Pool::PrintPool();

        }
        catch (const std::exception& error)
        {
            cout << error.what() << '\n';
        }
    }

}