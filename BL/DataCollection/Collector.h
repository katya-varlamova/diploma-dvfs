#pragma once
#include <vector>
#include <string>
#include <memory>
#include "core/PmuCollector.h"
#include "core/SystemController.h"
#include <memory>
#include <csignal>
#include <unistd.h>
#include "workloads/IWorkload.h"
#include <sys/wait.h>
#include "IDAFacade.h"
class Collector {
public:
    Collector() = default;
    std::vector<CollectedDataType> Collect(const std::shared_ptr<IWorkload> &workload, int attempts = 15);
    static std::shared_ptr<ISystemController> m_controller;
    static std::shared_ptr<IPmuCollector> m_collector;
    static double energy;
private:
    std::vector<long> m_availableFreqs = {2701000, 2700000, 2600000, 2500000, 2300000, 2100000, 1900000, 1700000, 1600000, 1400000, 1200000, 1100000, 900000, 700000, 600000, 400000};
};
