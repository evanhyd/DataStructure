#include "debug.h"
#include <iostream>

DebugClass::DebugClass() : id_(rand())
{
    std::cout << "Default Ctr: " << id_ << std::endl;
}

DebugClass::DebugClass(int id) : id_(id)
{
    std::cout << "Direct Ctr: " << id_ << std::endl;
}

DebugClass::DebugClass(const DebugClass& rhs) : id_(rhs.id_)
{
    std::cout<<"Copy Ctr: " << id_ << std::endl;
}
DebugClass::DebugClass(DebugClass&& rhs) noexcept : id_(rhs.id_)
{
    std::cout << "Move Ctr: " << id_ << std::endl;
    rhs.id_ = INT_MAX;
}

DebugClass::~DebugClass()
{
    std::cout << "Destroyed: " << id_ << std::endl;
}

DebugClass& DebugClass::operator=(const DebugClass& rhs)
{
    std::cout << "Copy Assignment: " << rhs.id_ << " -> " << id_ << std::endl;
    id_ = rhs.id_;

    return *this;
}
DebugClass& DebugClass::operator=(DebugClass&& rhs) noexcept
{
    std::cout << "Move Assignment: " << rhs.id_ << " -> " << id_ << std::endl;
    id_ = rhs.id_;
    rhs.id_ = INT_MAX;

    return *this;
}