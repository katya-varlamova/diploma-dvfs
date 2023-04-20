#pragma once

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <stdint.h>
#include <inttypes.h>

#include "IPmuCollector.h"

class PmuCollector : public IPmuCollector {
public:
    PmuCollector(int pid = 0, int cpu = -1) : m_pid(pid), m_cpu(cpu) {};
    void StartCollection(const std::vector<event_t> &events) override;
    void StopCollection(double &cpuTime) override;
    std::vector<double> GetVals() override;
private:
    void InitEvent(event_t event);
    std::vector<int> fds;
    std::vector<uint64_t> ids;
    std::vector<double> vals;
    int m_pid, m_cpu;
};
