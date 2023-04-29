#pragma once

#include "ISystemController.h"
#include <vector>
#include <fcntl.h>
#include <unistd.h>

governor_t StringToGoverner(const std::string &str);
std::string GovernerToString(governor_t governor);

class SystemController : public ISystemController {
public:
    explicit SystemController(int cpuNum = 8) : m_cpuNum(cpuNum) { OpenFDs(); }

    int SetAffinity(int pid, const std::vector<int> &cpus) override;

    long GetCpuFreq(int cpu) override;
    void SetCpuFreq(int cpu, long freq) override;

    governor_t GetCpuFreqGovernor(int cpu) override;
    void SetCpuFreqGovernor(int cpu, governor_t governor) override;
    void NormalizeCpuFreq(int cpu, long min, long max) override;

    double GetCurrent() override;
    double GetVoltage() override;

    ~SystemController() { CloseFDs(); }
private:
    void OpenFDs();
    void CloseFDs();
    std::vector<int> m_min_cpufreq_fds;
    std::vector<int> m_max_cpufreq_fds;
    std::vector<int> m_cur_cpufreq_fds;
    std::vector<int> m_governor_fds;

    int m_current_fd = 0, m_voltage_fd = 0;
    int m_cpuNum;
};
