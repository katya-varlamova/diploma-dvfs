#include <cmath>
#include "Optimizer.h"

long Optimizer::Optimize(double mrpi)
{
    long freq = m_optimals[0].cpufreq;
    for (long unsigned i = 0; i < m_optimals.size() - 1; ++i) {
        if (mrpi < m_optimals[i].mrpi)
            continue;
        if (fabs(m_optimals[i].mrpi - mrpi) < fabs(m_optimals[i + 1].mrpi - mrpi)){
            freq = m_optimals[i].cpufreq;
            break;
        }
        else {
            freq = m_optimals[i + 1].cpufreq;
            break;
        }
    }
    return freq;
}