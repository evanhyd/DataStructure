#include "debug.h"
#include <iostream>

DebugClass::DebugClass() : id_(global_id_++)
{
    std::cout << "Default ctr: " << id_ << std::endl;
}

DebugClass::DebugClass(int id) : id_(id)
{
    std::cout << "Direct ctr: " << id_ << std::endl;
}

DebugClass::DebugClass(const DebugClass& rhs) : id_(rhs.id_)
{
    std::cout<<"Copy ctr: " << id_ << std::endl;
}

DebugClass::DebugClass(DebugClass&& rhs) noexcept : id_(rhs.id_)
{
    std::cout << "Move ctr: " << id_ << std::endl;
    rhs.id_ = -rhs.id_;
}

DebugClass::~DebugClass()
{
    std::cout << "Dtr: " << id_ << std::endl;
}

DebugClass& DebugClass::operator=(const DebugClass& rhs)
{
    std::cout << "Copy asm: " << id_ << " <= " << rhs.id_ << std::endl;
    id_ = rhs.id_;

    return *this;
}

DebugClass& DebugClass::operator=(DebugClass&& rhs) noexcept
{
    std::cout << "Move asm: " << id_ << " <= " << rhs.id_ << std::endl;
    id_ = rhs.id_;
    rhs.id_ = -rhs.id_;

    return *this;
}

int DebugClass::global_id_ = 1;


std::ostream& operator<<(std::ostream& output, const DebugClass& obj)
{
    output << obj.id_;
    return output;
}