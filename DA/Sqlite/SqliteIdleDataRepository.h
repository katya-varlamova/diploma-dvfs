#pragma once

#include "SqliteConnection.h"
#include "DataCollection/DataTypes.h"
#include "logger/LoggerFactory.h"
class SqliteIdleDataRepository {
public:
    explicit SqliteIdleDataRepository(const std::shared_ptr<SqliteConnection> &connectionCreator) : m_connectionCreator(connectionCreator) {}
    void StoreData(const std::vector<CollectedDataType> &data);
private:
    std::shared_ptr<SqliteConnection> m_connectionCreator;
};
