#pragma once
#include <memory>
class IOptimizer {
public:
    virtual long Optimize(double mrpi) = 0;
};
