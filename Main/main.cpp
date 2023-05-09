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
int IRunner::run_id = 0;
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
std::vector<std::string> parsePath(std::string path) {
    std::vector<std::string> args;
    std::string delimiter = " ";
    size_t pos;
    while ((pos = path.find(delimiter)) != std::string::npos) {
        args.push_back(path.substr(0, pos));
        path.erase(0, pos + delimiter.length());
    }

    args.push_back(path);
    return args;
}
stats_t  demo(FILE  *f, const std::string &path) {
    DemoRunner::m_collector = std::shared_ptr<IPmuCollector> (new PmuCollector());
    DemoRunner::m_controller = std::shared_ptr<ISystemController> (new SystemController());

    auto config = std::shared_ptr<BaseConfig> (new YamlCppConfig("/home/kate/Desktop/diploma/repo/Config/config.txt"));
    auto facade = std::shared_ptr<IDAFacade> (new DASqliteFacade(config));

    auto optimizer = std::shared_ptr<IOptimizer>(new Optimizer(facade));
    DemoRunner runner(optimizer);

    auto args = parsePath(path);
    stats_t stats = runner.run(args[0], args);
    return stats;
}
stats_t  os(FILE  *f,governor_t governor, const std::string &path) {
    BasicRunner::m_collector = std::shared_ptr<IPmuCollector> (new PmuCollector());
    BasicRunner::m_controller = std::shared_ptr<ISystemController> (new SystemController());

    BasicRunner runner;

    auto args = parsePath(path);
    stats_t stats = runner.run(args[0], args, governor);
    return stats;
}
int main() {
    //collection();
    auto config = std::shared_ptr<BaseConfig> (new YamlCppConfig("/home/kate/Desktop/diploma/repo/Config/config.txt"));
    LoggerFactory::InitLogger(config);

    std::vector<governor_t> govs = {PERFORMANCE, POWERSAVE, SCHEDUTIL, ONDEMAND, CONSERVATIVE};
    std::vector<std::string> paths = {"/home/kate/Desktop/rodinia_3.1/openmp/kmeans/kmeans_serial/kmeans -i /home/kate/Desktop/rodinia_3.1/data/kmeans/kdd_cup",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/backprop/backprop 8553645",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/cfd/euler3d_cpu /home/kate/Desktop/rodinia_3.1/data/cfd/fvcorr.domn.097K",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/heartwall/heartwall /home/kate/Desktop/rodinia_3.1/data/heartwall/test.avi 5 1",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/hotspot/hotspot 1024 1024 3000 1 /home/kate/Desktop/rodinia_3.1/data/hotspot/temp_1024 /home/kate/Desktop/rodinia_3.1/data/hotspot/power_1024 /home/kate/Desktop/rodinia_3.1/data/hotspot/out.txt",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/leukocyte/OpenMP/leukocyte 3 1 /home/kate/Desktop/rodinia_3.1/data/leukocyte/testfile.avi",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/lud/omp/lud_omp -n 1 -s 2048 -i /home/kate/Desktop/rodinia_3.1/data/lud/2048.dat",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/streamcluster/sc_omp 10 20 256 5536 5536 1000 none output.txt 1",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/pathfinder/pathfinder 100000 100",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/nw/needle 10800 100000 1",
                                      "/home/kate/Desktop/rodinia_3.1/openmp/particlefilter/particle_filter -x 128 -y 128 -z 10 -np 60000"};//{"../Bench/Bench", "../Bench/stream"};
    FILE  *f = fopen("bench_res.txt", "w");
    int attempts = 25;
    for (const auto &p :paths ) {
        for (const auto &g: govs) {
            double avg_ipc = 0, avg_en = 0;
            for (int i = 0; i < attempts; i++) {
                auto stats = os(f, g, p);
                avg_en += stats.energy / stats.time;
                avg_ipc += stats.inst/stats.cycles;
            }
            avg_en /= attempts;
            avg_ipc /= attempts;
            fprintf(f, (GovernerToString(g) + ",%lf,%lf,%s\n").c_str(), avg_en, avg_ipc, p.c_str());

        }
        double avg_ipc = 0, avg_en = 0;
        for (int i = 0; i < attempts; i++) {
            auto stats = demo(f, p);
            avg_en += stats.energy / stats.time;
            avg_ipc += stats.inst/stats.cycles;
        }
        avg_en /= attempts;
        avg_ipc /= attempts;
        fprintf(f, "LLCMPI-based,%lf,%lf,%s\n", avg_en, avg_ipc, p.c_str());
    }
    fclose(f);
    return 0;
}
