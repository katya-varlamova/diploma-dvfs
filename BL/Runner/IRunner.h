#pragma once
#include <string>
#include "IOptimizer.h"
class IRunner {
public:
    virtual void run(const std::string &path, const std::shared_ptr<IOptimizer> &optimizer) = 0;
};
