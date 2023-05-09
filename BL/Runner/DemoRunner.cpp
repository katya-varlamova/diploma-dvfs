#include "DemoRunner.h"
#include "common_macros.h"
#include "logger/LoggerFactory.h"

std::shared_ptr<IPmuCollector> DemoRunner::m_collector;
std::shared_ptr<ISystemController> DemoRunner::m_controller;
std::shared_ptr<IOptimizer> DemoRunner::m_optimizer;
double DemoRunner::energy = 0;
double DemoRunner::time = 0;
double DemoRunner::inst = 0;
double DemoRunner::cycles = 0;
static void handler(int sn) {
    UNUSED_VAR(sn)
    auto vals = DemoRunner::m_collector->GetVals();
    DemoRunner::energy += (0.01 * DemoRunner::m_controller->GetCurrent()) * DemoRunner::m_controller->GetVoltage();
    DemoRunner::time += vals[3] / 1000000000;

    long freq = DemoRunner::m_optimizer->Optimize(vals[2] / vals[0]);

    DemoRunner::m_controller->SetCpuFreq(0, freq);
    DemoRunner::m_controller->SetCpuFreq(4, freq);
    DemoRunner::inst += vals[0];
    DemoRunner::cycles += vals[1];
    std::setlocale(LC_NUMERIC, "C");
    auto msg = ( std::string("FreqLog,userspace,") + std::to_string(IRunner::run_id) + "," +std::to_string(DemoRunner::m_controller->GetCpuFreq(0)) + "," +  std::to_string(vals[2] / vals[0]));
    LoggerFactory::GetLogger()->LogInfo(msg.c_str());
}
static void dummy(int a) {UNUSED_VAR(a)}
stats_t DemoRunner::run(const std::string &path, std::vector<std::string> args, governor_t governor) {
    for (int j = 0; j < 8; j++) {
        m_controller->SetCpuFreqGovernor(j, governor);
        m_controller->SetCpuFreq(j, 2700000); // 2701000 400000
    }

    std::vector<event_t> events = {{.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_INSTRUCTIONS},
                                   {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CPU_CYCLES},
                                   {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CACHE_MISSES},
                                   {.type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CPU_CLOCK}};
    int pid = fork();
    if (pid == 0)
    {
        char **strs = (char **) malloc(args.size() * sizeof(char *));
        int i;
        for (i = 0; i < args.size(); i++) {
            strs[i] = (char *) malloc(args[i].size() + 1);
            strcpy(strs[i], args[i].c_str());
        }
        strs[i] = NULL;
        execv(strs[0], strs);
        exit(0);
    }
    signal(SIGALRM, handler);

    m_controller->SetAffinity(pid, std::vector<int>({0}));

    m_collector->StartCollection(events, pid, 0);
    ualarm(1000, 20000);
    wait(NULL);
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