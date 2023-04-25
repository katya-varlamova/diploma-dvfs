#pragma once
#include <vector>
#include <memory>
#include "DataCollection/DataTypes.h"
class IDAFacade {
public:
    virtual void StorePortionCollectionData(const std::vector<CollectedDataType> &data) = 0;
    virtual void StoreIdleCollectionData(const std::vector<CollectedDataType> &data) = 0;

    virtual std::vector<ResultDataType> GetEuclideanMetricResult() = 0;
};
using IDAFacadePtr = std::shared_ptr<IDAFacade>;