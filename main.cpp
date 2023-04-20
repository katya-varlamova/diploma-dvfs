// #define _GNU_SOURCE
#include <iostream>
#include "core/PmuCollector.h"
#include "core/SystemController.h"
#include <memory>
#include <csignal>
#include "workloads/workloads.h"

std::shared_ptr<IPmuCollector> collector;
std::shared_ptr<ISystemController> controller;
void handler(int sn) {
    auto vals = collector->GetVals();
    for (auto v : vals)
        printf("%lf,", v);
    printf("%ld,%ld,%ld,%s\n", controller->GetCpuFreq(0),
           controller->GetCurrent(),
           controller->GetVoltage(),
           GovernerToString(controller->GetCpuFreqGovernor(0)).c_str());
}
int main() {
    controller = std::shared_ptr<ISystemController>(new SystemController());
    collector = std::shared_ptr<IPmuCollector>(new PmuCollector());
    controller->SetAffinity(getpid(), std::vector<int> ({0}));
    signal(SIGALRM, handler);
    std::vector<event_t> events = {{.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_INSTRUCTIONS},
                                   {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CPU_CYCLES},
                                   {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CACHE_MISSES},
                                   {.type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_PAGE_FAULTS_MAJ}};
    collector->StartCollection(events);
    ualarm(10000, 100000);

    io_bound();

    double time;
    collector->StopCollection(time);

    return 0;
}
