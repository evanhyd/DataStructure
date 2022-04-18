#include "debug.h"






//DEBUG CLASS
DebugClass::DebugClass() : id_(global_id_++)
{
    std::cout << "Def-Ctr: " << id_ << std::endl;
}

DebugClass::DebugClass(int id) : id_(id)
{
    std::cout << "Dir_Ctr: " << id_ << std::endl;
}

DebugClass::DebugClass(const DebugClass& rhs) : id_(rhs.id_)
{
    std::cout<<"Cpy_Ctr: " << id_ << std::endl;
}

DebugClass::DebugClass(DebugClass&& rhs) noexcept : id_(rhs.id_)
{
    std::cout << "Mov_Ctr: " << id_ << std::endl;
    rhs.id_ = -rhs.id_;
}

DebugClass::~DebugClass()
{
    std::cout << "Dtr: " << id_ << std::endl;
}

DebugClass& DebugClass::operator=(const DebugClass& rhs)
{
    std::cout << "Cpy_Asg: " << id_ << " <= " << rhs.id_ << std::endl;
    id_ = rhs.id_;

    return *this;
}

DebugClass& DebugClass::operator=(DebugClass&& rhs) noexcept
{
    std::cout << "Mov_Asg: " << id_ << " <= " << rhs.id_ << std::endl;
    id_ = rhs.id_;
    rhs.id_ = -rhs.id_;

    return *this;
}

std::ostream& operator<<(std::ostream& output, const DebugClass& obj)
{
    output << obj.id_;
    return output;
}

int DebugClass::global_id_ = 1;


//TIMER
using namespace std::chrono;

void Timer::Start()
{
    begin_ = high_resolution_clock::now();
}
void Timer::Stop()
{
    end_ = high_resolution_clock::now();
}
void Timer::Result() const
{
    std::cout << duration_cast<milliseconds>(end_ - begin_).count() << " ms\n";
}