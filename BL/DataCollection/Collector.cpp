
#include "Collector.h"
#include "common_macros.h"
std::shared_ptr<IPmuCollector> Collector::m_collector;
std::shared_ptr<ISystemController> Collector::m_controller;
double Collector::energy = 0;
static void handler(int sn) {
    UNUSED_VAR(sn)
    Collector::energy += (0.01 * Collector::m_controller->GetCurrent()) * Collector::m_controller->GetVoltage();
}
std::vector<CollectedDataType> Collector::Collect(const std::shared_ptr<IWorkload> &workload, int attempts)
{
    std::vector<CollectedDataType> collected;
    for (auto & freq : m_availableFreqs) {
        for (int attempt = 0; attempt < attempts; attempt++) {
            for (int j = 0; j < 8; j++) {
                m_controller->SetCpuFreqGovernor(j, USERSPACE);
                m_controller->SetCpuFreq(j, 2701000); // 2701000 400000
            }
            m_controller->SetCpuFreq(0, freq);
            m_controller->SetCpuFreq(4, freq);
            signal(SIGALRM, handler);
            std::vector<event_t> events = {{.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_INSTRUCTIONS},
                                           {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CPU_CYCLES},
                                           {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CACHE_MISSES},
                                           {.type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CPU_CLOCK}};
            int pid = fork();
            if (pid == 0)
            {
                workload->RunWorkload();
                exit(0);
            }
            m_controller->SetAffinity(pid, std::vector<int>({0}));

            m_collector->StartCollection(events, pid, 0);
            ualarm(1000, 10000);
            int status;
            wait(&status);
            auto vals = m_collector->GetVals();
            int descr = workload->GetDescriptor();
            collected.push_back({descr, attempt,
                                       m_controller->GetCpuFreq(0),
                                       vals[3] / 1000000000,
                                       energy,
                                       vals[0] / vals[1],
                                       vals[2] / vals[0],
                                       vals[0], vals[1], vals[2]});

            m_collector->StopCollection();
            energy = 0;
        }
    }
    return collected;
}

