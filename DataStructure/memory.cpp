#include "memory.h"

using std::cin;
using std::cout;
using std::endl;
using namespace cug::memory;

//DEBUG CLASS
DebugClass::DebugClass() : id_(global_id_++)
{
    cout << "Def-Ctr: " << id_ << endl;
}

DebugClass::DebugClass(int id) : id_(id)
{
    cout << "Dir_Ctr: " << id_ << endl;
}

DebugClass::DebugClass(const DebugClass& rhs) : id_(rhs.id_)
{
    cout << "Cpy_Ctr: " << id_ << endl;
}

DebugClass::DebugClass(DebugClass&& rhs) noexcept : id_(rhs.id_)
{
    cout << "Mov_Ctr: " << id_ << endl;
    rhs.id_ = -rhs.id_;
}

DebugClass::~DebugClass()
{
    cout << "Dtr: " << id_ << endl;
}

DebugClass& DebugClass::operator=(const DebugClass& rhs)
{
    cout << "Cpy_Asg: " << id_ << " <= " << rhs.id_ << endl;
    id_ = rhs.id_;

    return *this;
}

DebugClass& DebugClass::operator=(DebugClass&& rhs) noexcept
{
    cout << "Mov_Asg: " << id_ << " <= " << rhs.id_ << endl;
    id_ = rhs.id_;
    rhs.id_ = -rhs.id_;

    return *this;
}

std::ostream& cug::memory::operator<<(std::ostream& output, const DebugClass& obj)
{
    output << obj.id_;
    return output;
}

int DebugClass::global_id_ = 1;
