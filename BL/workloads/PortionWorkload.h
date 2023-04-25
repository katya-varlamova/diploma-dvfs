#pragma once
#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <vector>
#include <complex>
#include <map>
#include "IWorkload.h"
class PortionWorkload : public IWorkload {
public:
    PortionWorkload(double cpuPortion) ;
    void RunWorkload() override ;
    int GetDescriptor() override;
    ~PortionWorkload() {
        free(arr);
        free(rands);
    }
private:
    int *arr;
    long *rands;
    long memIters = 0, cpuIters = 0, totalIters;
    const long bufSize = 1 * 1024 * 1024 * 1024;
    const long cacheSize = 12 * 1024 * 1024;
    int EPS = 1e-7;
    double m_portion;
};
