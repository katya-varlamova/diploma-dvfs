#pragma once
#include "IRunner.h"
#include "core/PmuCollector.h"
#include "core/ISystemController.h"
#include <csignal>
#include <unistd.h>
#include "workloads/IWorkload.h"
#include <sys/wait.h>

class DemoRunner : public IRunner {
public:
    DemoRunner(const std::shared_ptr<IOptimizer> &optimizer) {
        DemoRunner::m_optimizer = optimizer;
    }
    stats_t run(const std::string &path, governor_t governor = USERSPACE) override;

    static std::shared_ptr<IPmuCollector> m_collector;
    static std::shared_ptr<ISystemController> m_controller;
    static std::shared_ptr<IOptimizer> m_optimizer;
    static double energy;
    static double time;
    static double inst;
    static double cycles;
private:
};
