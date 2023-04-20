#include "PmuCollector.h"
struct read_format {
    uint64_t nr;
    struct {
        uint64_t value;
        uint64_t id;
    } values[];
};

void PmuCollector::InitEvent(event_t event) {
    struct perf_event_attr pea;
    memset(&pea, 0, sizeof(struct perf_event_attr));
    pea.type = event.type;
    pea.size = sizeof(struct perf_event_attr);
    pea.config = event.config;
    pea.disabled = 1;
    pea.exclude_kernel = 1;
    pea.exclude_hv = 1;
    pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
    if (fds.empty())
        fds.push_back(syscall(__NR_perf_event_open, &pea, m_pid, m_cpu, -1, 0));
    else
        fds.push_back(syscall(__NR_perf_event_open, &pea, m_pid, m_cpu, fds[0], 0));
    ids.push_back(0);
    vals.push_back(0);
    ioctl(fds[fds.size() - 1], PERF_EVENT_IOC_ID, &ids[fds.size() - 1]);
}

std::vector<double> PmuCollector::GetVals()
{
    char buf[4096];
    std::vector<double> deltas = vals;
    struct read_format* rf = (struct read_format*) buf;
    read(fds[0], buf, sizeof(buf));
    for (int i = 0; i < rf->nr; i++) {
        for (int j = 0; j < fds.size(); j++) {
            if (rf->values[i].id == ids[j]) {
                deltas[j] = rf->values[i].value - deltas[j];
                vals[j] = rf->values[i].value;
            }
        }
    }
    return deltas;
}
void PmuCollector::StartCollection(const std::vector<event_t> &events) {
    if (events.empty())
        return;
    for (const auto & e : events)
        InitEvent(e);
    ioctl(fds[0], PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
    ioctl(fds[0], PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
}

void PmuCollector::StopCollection(double &cpuTime) {
    ioctl(fds[0], PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
    fds.clear();
    ids.clear();
    vals.clear();
}