#pragma once
#include "IRunner.h"
#include "core/PmuCollector.h"
#include "core/ISystemController.h"
#include <csignal>
#include <unistd.h>
#include "workloads/IWorkload.h"
#include <sys/wait.h>
class BasicRunner : public IRunner {
public:
    stats_t run(const std::string &path, governor_t governor) override;

    static std::shared_ptr<IPmuCollector> m_collector;
    static std::shared_ptr<ISystemController> m_controller;
    static double energy;
    static double time;
    static double inst;
    static double cycles;
    static governor_t m_governor;
};
