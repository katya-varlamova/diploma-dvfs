#include "Runner.h"
#include "common_macros.h"
std::shared_ptr<IPmuCollector> Runner::m_collector;
std::shared_ptr<ISystemController> Runner::m_controller;
std::shared_ptr<IOptimizer> Runner::m_optimizer;
double Runner::energy = 0;
double Runner::time = 0;
static void handler(int sn) {
    UNUSED_VAR(sn)
    auto vals = Runner::m_collector->GetVals();
    Runner::energy += (0.01 * Runner::m_controller->GetCurrent()) * Runner::m_controller->GetVoltage();
    Runner::time += vals[3] / 1000000000;

    long freq = Runner::m_optimizer->Optimize(vals[2] / vals[0]);

    Runner::m_controller->SetCpuFreq(0, freq);
    Runner::m_controller->SetCpuFreq(4, freq);
    printf("%lf,%ld\n\n", vals[2] / vals[0], freq);
}
void Runner::run(const std::string &path, const std::shared_ptr<IOptimizer> &optimizer) {
    for (int j = 0; j < 8; j++) {
        m_controller->SetCpuFreqGovernor(j, USERSPACE);
        m_controller->SetCpuFreq(j, 2701000); // 2701000 400000
    }
    Runner::m_optimizer = optimizer;
    signal(SIGALRM, handler);
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
    m_controller->SetAffinity(pid, std::vector<int>({0}));

    m_collector->StartCollection(events, pid, 0);
    ualarm(1000, 20000);
    int status;
    wait(&status);

    m_collector->StopCollection();
}