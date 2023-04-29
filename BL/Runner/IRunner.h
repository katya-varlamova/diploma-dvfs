#pragma once
#include <string>
#include "IOptimizer.h"
#include "core/ISystemController.h"
typedef struct {
    double time;
    double energy;
    double inst;
    double cycles;
} stats_t;
class IRunner {
public:
    virtual stats_t run(const std::string &path, governor_t governor) = 0;
};
