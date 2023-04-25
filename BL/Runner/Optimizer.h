#pragma once
#include "IOptimizer.h"
#include "DataCollection/DataTypes.h"
#include "IDAFacade.h"
#include <vector>
class Optimizer : public IOptimizer {
public:
    Optimizer(const IDAFacadePtr &facade) {
        m_optimals = facade->GetEuclideanMetricResult();
    }
    long Optimize(double mrpi) override;
private:
    std::vector<ResultDataType> m_optimals;
};
