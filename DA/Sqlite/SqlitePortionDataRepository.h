#pragma once

#include "SqliteConnection.h"
#include "DataCollection/DataTypes.h"
#include "logger/LoggerFactory.h"
class SqlitePortionDataRepository {
public:
    explicit SqlitePortionDataRepository(const std::shared_ptr<SqliteConnection> &connectionCreator) : m_connectionCreator(connectionCreator)
    {}
    std::vector<ResultDataType> GetEuclideanMetricData();
    void StoreData(const std::vector<CollectedDataType> &data);
private:
    std::shared_ptr<SqliteConnection> m_connectionCreator;
};
