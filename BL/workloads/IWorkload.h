#pragma once

#include <cstdio>

class IWorkload {
public:
    virtual void RunWorkload() = 0;
    virtual int GetDescriptor() = 0;
};
