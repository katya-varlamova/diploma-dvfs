#pragma once
struct CollectedDataType {
    int descriptor;
    int attempt;
    long cpufreq;
    double ct,power, ipc, mrpi, inst, cycles, l3;
};
struct ResultDataType {
    double mrpi;
    long cpufreq;
};