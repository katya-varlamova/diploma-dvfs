#pragma once

#include <vector>
#include <functional>

typedef struct
{
    int type, config;
} event_t;

class IPmuCollector {
public:
    virtual void StartCollection(const std::vector<event_t> &events, int pid = 0, int cpu = -1) = 0;
    virtual void StopCollection() = 0;
    virtual std::vector<double> GetVals() = 0;
};