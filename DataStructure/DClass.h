#pragma once
class DClass
{
    int a_;
    int b_;

public:

    DClass();
    DClass(int a, int b);
    DClass(const DClass& rhs);
    DClass(DClass&& rhs) noexcept;
    ~DClass();

    DClass& operator=(const DClass& rhs);
    DClass& operator=(DClass&& rhs) noexcept;
};

