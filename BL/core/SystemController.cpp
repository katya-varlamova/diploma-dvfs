#include "SystemController.h"
std::string MAX_CPU_FREQ_PATH(int cpuNum) {
    return std::string("/sys/devices/system/cpu/cpu") + std::to_string(cpuNum) + "/cpufreq/scaling_max_freq";
}
std::string MIN_CPU_FREQ_PATH(int cpuNum) {
    return std::string("/sys/devices/system/cpu/cpu") + std::to_string(cpuNum) + "/cpufreq/scaling_min_freq";
}
std::string CUR_CPU_FREQ_PATH(int cpuNum) {
    return std::string("/sys/devices/system/cpu/cpu") + std::to_string(cpuNum) + "/cpufreq/cpuinfo_cur_freq";
}
std::string CPU_FREQ_GOVERNOR_PATH(int cpuNum) {
    return std::string("/sys/devices/system/cpu/cpu") + std::to_string(cpuNum) + "/cpufreq/scaling_governor";
}
std::string CURRENT_NOW_PATH() {
    return "/sys/class/power_supply/BAT0/current_now";
}
std::string VOLTAGE_NOW_PATH() {
    return "/sys/class/power_supply/BAT0/voltage_now";
}
std::string GovernerToString(governor_t governor) {
    switch (governor) {
        case POWERSAVE: return "powersave";
        case PERFORMANCE: return "performance";
        case ONDEMAND: return "ondemand";
        case SCHEDUTIL: return "schedutil";
        case CONSERVATIVE: return "conservative";
        default: return "userspace";
    }
}
governor_t StringToGoverner(const std::string &str) {
    if (str == "powersave\n") return POWERSAVE;
    if (str == "performance\n") return PERFORMANCE;
    if (str == "ondemand\n") return ONDEMAND;
    if (str == "schedutil\n") return SCHEDUTIL;
    if (str == "conservative\n") return CONSERVATIVE;
    if (str == "userspace\n") return USERSPACE;
    return USERSPACE;
}

void SystemController::OpenFDs() {
    for (int i = 0; i < m_cpuNum; i++) {
        m_cur_cpufreq_fds.push_back(open(CUR_CPU_FREQ_PATH(i).c_str(), O_RDONLY));
        m_min_cpufreq_fds.push_back(open(MIN_CPU_FREQ_PATH(i).c_str(), O_WRONLY|O_CREAT|O_TRUNC));
        m_max_cpufreq_fds.push_back(open(MAX_CPU_FREQ_PATH(i).c_str(), O_WRONLY|O_CREAT|O_TRUNC));
        m_governor_fds.push_back(open(CPU_FREQ_GOVERNOR_PATH(i).c_str(), O_RDWR|O_TRUNC));
    }
    m_current_fd = open(CURRENT_NOW_PATH().c_str(), O_RDONLY);
    m_voltage_fd = open(VOLTAGE_NOW_PATH().c_str(), O_RDONLY);
}

void SystemController::CloseFDs() {
    for (int i = 0; i < m_cpuNum; i++) {
        close(m_cur_cpufreq_fds[i]);
        close(m_min_cpufreq_fds[i]);
        close(m_max_cpufreq_fds[i]);
        close(m_governor_fds[i]);
    }
    m_min_cpufreq_fds.clear();
    m_max_cpufreq_fds.clear();
    m_cur_cpufreq_fds.clear();
    m_governor_fds.clear();
    close(m_current_fd);
    close(m_voltage_fd);
}

void SystemController::NormalizeCpuFreq(int cpu, long min, long max) {
    auto buf  = std::to_string(max);
    lseek(m_max_cpufreq_fds[cpu], 0, SEEK_SET);
    write(m_max_cpufreq_fds[cpu], buf.c_str(), 100);
    lseek(m_max_cpufreq_fds[cpu], 0, SEEK_SET);

    buf  = std::to_string(min);
    lseek(m_min_cpufreq_fds[cpu], 0, SEEK_SET);
    write(m_min_cpufreq_fds[cpu], buf.c_str(), 100);
    lseek(m_min_cpufreq_fds[cpu], 0, SEEK_SET);
}
long SystemController::GetCpuFreq(int cpu)
{
    char buf[100];
    lseek(m_cur_cpufreq_fds[cpu], 0, SEEK_SET);
    read(m_cur_cpufreq_fds[cpu], buf, 100);
    lseek(m_cur_cpufreq_fds[cpu], 0, SEEK_SET);
    return strtol(buf, nullptr, 10);
}
void SystemController::SetCpuFreq(int cpu, long freq)
{
    auto buf  = std::to_string(freq);
    lseek(m_min_cpufreq_fds[cpu], 0, SEEK_SET);
    write(m_min_cpufreq_fds[cpu], buf.c_str(), 100);
    lseek(m_min_cpufreq_fds[cpu], 0, SEEK_SET);

    lseek(m_max_cpufreq_fds[cpu], 0, SEEK_SET);
    write(m_max_cpufreq_fds[cpu], buf.c_str(), 100);
    lseek(m_max_cpufreq_fds[cpu], 0, SEEK_SET);
}

double SystemController::GetCurrent()
{
    char buf[100];
    lseek(m_current_fd, 0, SEEK_SET);
    read(m_current_fd, buf, 100);
    lseek(m_current_fd, 0, SEEK_SET);
    return strtol(buf, nullptr, 10) * 1e-6;
}
double SystemController::GetVoltage()
{
    char buf[100];
    lseek(m_voltage_fd, 0, SEEK_SET);
    read(m_voltage_fd, buf, 100);
    lseek(m_voltage_fd, 0, SEEK_SET);
    return strtol(buf, nullptr, 10) * 1e-6;
}

governor_t SystemController::GetCpuFreqGovernor(int cpu)
{
    char buf[100];
    lseek(m_governor_fds[cpu], 0, SEEK_SET);
    read(m_governor_fds[cpu], buf, 100);
    lseek(m_governor_fds[cpu], 0, SEEK_SET);
    return StringToGoverner(buf);
}
void SystemController::SetCpuFreqGovernor(int cpu, governor_t governor)
{
    auto buf  = GovernerToString(governor);
    lseek(m_governor_fds[cpu], 0, SEEK_SET);
    write(m_governor_fds[cpu], buf.c_str(), 100);
    lseek(m_governor_fds[cpu], 0, SEEK_SET);
}

int SystemController::SetAffinity(int pid, const std::vector<int> &cpus)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    for (auto c : cpus)
        CPU_SET(c, &set);
    int lim = 0;
    int err;
    while ((err = sched_setaffinity(pid, sizeof(set), &set)) == -1 && lim++ < 10);
    return err;
}