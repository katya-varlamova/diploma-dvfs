#include "BasicRunner.h"
#include "common_macros.h"
#include "logger/LoggerFactory.h"
#include "core/SystemController.h"

std::shared_ptr<IPmuCollector> BasicRunner::m_collector;
std::shared_ptr<ISystemController> BasicRunner::m_controller;
double BasicRunner::energy = 0;
double BasicRunner::time = 0;
double BasicRunner::inst = 0;
double BasicRunner::cycles = 0;
governor_t BasicRunner::m_governor;
static void handler(int sn) {
    UNUSED_VAR(sn)
    auto vals = BasicRunner::m_collector->GetVals();
    BasicRunner::energy += (0.01 * BasicRunner::m_controller->GetCurrent()) * BasicRunner::m_controller->GetVoltage();
    BasicRunner::time += vals[3] / 1000000000;
    BasicRunner::inst += vals[0];
    BasicRunner::cycles += vals[1];

    LoggerFactory::GetLogger()->LogInfo(
            ( std::string("FreqLog,") + GovernerToString(BasicRunner::m_governor) + "," + std::to_string(IRunner::run_id) + "," +std::to_string(BasicRunner::m_controller->GetCpuFreq(0)) ).c_str()); //printf("%ld\n\n",  BasicRunner::m_controller->GetCpuFreq(0));
}
static void dummy(int a) {UNUSED_VAR(a)}
stats_t BasicRunner::run(const std::string &path, governor_t governor) {
    m_governor = governor;
    for (int j = 0; j < 8; j++) {
        m_controller->SetCpuFreqGovernor(j, governor);
        if (j == 0 || j == 4)
            m_controller->NormalizeCpuFreq(j, 400000, 2700000);
        else
            m_controller->SetCpuFreq(j, 2700000); // 2701000 400000
    }

    std::vector<event_t> events = {{.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_INSTRUCTIONS},
                                   {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CPU_CYCLES},
                                   {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CACHE_MISSES},
                                   {.type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CPU_CLOCK}};
    int pid = fork();
    if (pid == 0)
    {
        execv(path.c_str(), NULL);
        exit(0);
    }
    signal(SIGALRM, handler);
    m_controller->SetAffinity(pid, std::vector<int>({0}));

    m_collector->StartCollection(events, pid, 0);
    ualarm(1000, 20000);
    int status;
    wait(&status);
    signal(SIGALRM, dummy);
    m_collector->StopCollection();
    stats_t stats = {time, energy, inst, cycles, run_id};
    energy = 0;
    time = 0;
    inst = 0;
    cycles = 0;
    run_id += 1;
    return stats;
}