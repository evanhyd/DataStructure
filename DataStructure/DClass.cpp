#include "DClass.h"
#include <iostream>

DClass::DClass() : a_(0), b_(0)
{
    std::cout << "DClass default constructor\n";
}
DClass::DClass(int a, int b) : a_(a), b_(b)
{
    std::cout << "DClass direct constructor\n";
}
DClass::DClass(const DClass& rhs) : a_(rhs.a_), b_(rhs.b_)
{
    std::cout << "DClass copy constructor\n";
}
DClass::DClass(DClass&& rhs) noexcept : a_(rhs.a_), b_(rhs.b_)
{
    std::cout << "DClass move constructor\n";
}
DClass::~DClass()
{
    std::cout << "DClass destructor\n";
}

DClass& DClass::operator=(const DClass& rhs)
{
    std::cout << "DClass copy assignment\n";
    a_ = rhs.a_;
    b_ = rhs.b_;

    return *this;
}
DClass& DClass::operator=(DClass&& rhs) noexcept
{
    std::cout << "DClass move assignment\n";
    a_ = rhs.a_;
    b_ = rhs.b_;

    return *this;
}