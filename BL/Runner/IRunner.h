#pragma once
#include <string>
#include "IOptimizer.h"
#include "core/ISystemController.h"
typedef struct {
    double time;
    double energy;
    double inst;
    double cycles;
    int run_id;
} stats_t;
class IRunner {
public:
    virtual stats_t run(const std::string &path, std::vector<std::string> args, governor_t governor) = 0;
    static int run_id;
};
