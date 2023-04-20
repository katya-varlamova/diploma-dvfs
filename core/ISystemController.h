#pragma once

#include <string>
#include <vector>

typedef enum
{
    POWERSAVE,
    PERFORMANCE,
    ONDEMAND,
    SCHEDUTIL,
    CONSERVATIVE,
    USERSPACE
} governor_t;

class ISystemController {
public:
    virtual int SetAffinity(int pid, const std::vector<int> &cpus) = 0;
    virtual long GetCpuFreq(int cpu) = 0;
    virtual void SetCpuFreq(int cpu, long freq) = 0;

    virtual governor_t GetCpuFreqGovernor(int cpu) = 0;
    virtual void SetCpuFreqGovernor(int cpu, governor_t governor) = 0;

    virtual long GetCurrent() = 0;
    virtual long GetVoltage() = 0;
};
