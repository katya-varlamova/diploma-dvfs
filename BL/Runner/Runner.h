#pragma once
#include "IRunner.h"
#include "core/PmuCollector.h"
#include "core/ISystemController.h"
#include <csignal>
#include <unistd.h>
#include "workloads/IWorkload.h"
#include <sys/wait.h>
class Runner : public IRunner {
public:
    void run(const std::string &path, const std::shared_ptr<IOptimizer> &optimizer) override;

    static std::shared_ptr<IPmuCollector> m_collector;
    static std::shared_ptr<ISystemController> m_controller;
    static std::shared_ptr<IOptimizer> m_optimizer;
    static double energy;
    static double time;
private:
};
