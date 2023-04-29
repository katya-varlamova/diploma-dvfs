// #define _GNU_SOURCE
#include "core/PmuCollector.h"
#include "core/SystemController.h"
#include "workloads/IdleWorkload.h"
#include "workloads/PortionWorkload.h"
#include "DataCollection/Collector.h"
#include "Runner/DemoRunner.h"
#include "Runner/Optimizer.h"
#include "Sqlite/DASqliteFacade.h"
#include "config/yamlcpp_config.h"
#include "Runner/BasicRunner.h"

void collection() {
    auto config = std::shared_ptr<BaseConfig> (new YamlCppConfig("/home/kate/Desktop/diploma/repo/Config/config.txt"));
    auto facade = std::shared_ptr<IDAFacade> (new DASqliteFacade(config));

    Collector collector;
    Collector::m_collector = std::shared_ptr<IPmuCollector> (new PmuCollector());
    Collector::m_controller = std::shared_ptr<ISystemController> (new SystemController());
    for (int i = 45; i < 101; i += 1) {
        auto w = std::shared_ptr<IWorkload>(new PortionWorkload(i / 100.0));
        auto collectedData = collector.Collect(w, 5);
        facade->StorePortionCollectionData(collectedData);
    }
    auto w = std::shared_ptr<IWorkload>(new IdleWorkload(2));
//    auto collectedData = collector.Collect(w, 10);
//    facade->StoreIdleCollectionData(collectedData);
}
void demo(const std::string &path) {
    DemoRunner::m_collector = std::shared_ptr<IPmuCollector> (new PmuCollector());
    DemoRunner::m_controller = std::shared_ptr<ISystemController> (new SystemController());

    auto config = std::shared_ptr<BaseConfig> (new YamlCppConfig("/home/kate/Desktop/diploma/repo/Config/config.txt"));
    auto facade = std::shared_ptr<IDAFacade> (new DASqliteFacade(config));

    auto optimizer = std::shared_ptr<IOptimizer>(new Optimizer(facade));
    DemoRunner runner(optimizer);

    stats_t stats = runner.run(path);
    printf("demo,%lf,%lf,%lf\n", stats.time, stats.energy, stats.inst/stats.cycles);
}
void os(governor_t governor, const std::string &path) {
    BasicRunner::m_collector = std::shared_ptr<IPmuCollector> (new PmuCollector());
    BasicRunner::m_controller = std::shared_ptr<ISystemController> (new SystemController());

    BasicRunner runner;

    stats_t stats = runner.run(path, governor);
    printf((GovernerToString(governor) + ",%lf,%lf,%lf\n").c_str(),stats.time, stats.energy, stats.inst/stats.cycles);
}
int main() {
    //collection();
    auto config = std::shared_ptr<BaseConfig> (new YamlCppConfig("/home/kate/Desktop/diploma/repo/Config/config.txt"));
    LoggerFactory::InitLogger(config);

    std::vector<governor_t> govs = {PERFORMANCE, POWERSAVE, SCHEDUTIL, ONDEMAND, CONSERVATIVE};
    std::vector<std::string> paths = {"../Bench/stream"};//{"../Bench/Bench", "../Bench/stream"};
    int attempts = 2;
    for (const auto &p :paths ) {
        for (const auto &g: govs) {
            for (int i = 0; i < attempts; i++)
                os(g, p);
        }
        for (int i = 0; i < attempts; i++)
            demo(p);
    }
    return 0;
}

//#include <iostream>
//#include "core/PmuCollector.h"
//#include "core/SystemController.h"
//#include <memory>
//#include <csignal>
//#include <sys/types.h>
//#include <sys/wait.h>
//#include "workloads/IWorkload.h"
//std::shared_ptr<IPmuCollector> collector;
//std::shared_ptr<ISystemController> controller;
//int a =0;
//double energy = 0;
//void printStats(int cpuPortion, int at) {
//    auto vals = collector->GetVals();
//    if (!vals.empty())
//    printf("%d,%d,%ld,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
//           cpuPortion,
//           at,
//           controller->GetCpuFreq(0),
//           vals[3] / 1000000000,
//           energy,
//           vals[0] / vals[1],
//           vals[2] / vals[0],
//           vals[0], vals[1], vals[2]);
//}
//
//void handler(int sn) {
//    energy += (0.01 * controller->GetCurrent()) * controller->GetVoltage();
//    //printStats(0, 0);
//}
//int main() {
//    // prepare
//    for (int h = 0; h < 1; h++)
//        for (int i = 0; i < 100000000; i++);
//
//    std::vector<long> m_availableFreqs = { 2701000, 2700000, 2600000, 2500000, 2300000, 2100000, 1900000, 1700000, 1600000, 1400000, 1200000, 1100000, 900000, 700000, 600000, 400000 }; //, 2700000, 2600000, 2500000, 2300000, 2100000, 1900000, 1700000, 1600000, 1400000, 1200000, 1100000, 900000, 700000, 600000, 400000};
//    for (auto & freq : m_availableFreqs) {
//
//        for (int i = 0; i < 101; i+=1) {
//            for (int at = 0; at < 15; at++) {
////                int pid = fork();
////                if (pid == 0)
////                {
////                    sleep(2);
////                    exit(0);
////                }
//                auto workloadBuilder = std::shared_ptr<WorkloadBuilder>(new WorkloadBuilder(i / 100.0));
//                controller = std::shared_ptr<ISystemController>(new SystemController());
//                collector = std::shared_ptr<IPmuCollector>(new PmuCollector());
//                controller->SetAffinity(getpid(), std::vector<int>({0}));
//                for (int j = 0; j < 8; j++) {
//                    controller->SetCpuFreqGovernor(j, USERSPACE);
//                    controller->SetCpuFreq(j, 2701000); // 2701000 400000
//                }
//                controller->SetCpuFreq(0, freq);
//                controller->SetCpuFreq(4, freq);
//                signal(SIGALRM, handler);
//                std::vector<event_t> events = {{.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_INSTRUCTIONS},
//                                               {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CPU_CYCLES},
//                                               {.type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CACHE_MISSES},
//                                               {.type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CPU_CLOCK}};
//                collector->StartCollection(events);
//                ualarm(1000, 10000);
//                int status;
//                wait(&status);
//
//                workloadBuilder->RunWorkload();
//
//                printStats(0, at);
//                collector->StopCollection();
//                energy = 0;
//            }
//        }
//    }
//
//    return 0;
//}
