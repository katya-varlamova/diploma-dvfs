#pragma once
#include "IDAFacade.h"
#include "config/base_config.h"
#include "SqliteIdleDataRepository.h"
#include "SqlitePortionDataRepository.h"
#include "SqliteConnection.h"
class DASqliteFacade : public IDAFacade {
public:
    DASqliteFacade(const std::shared_ptr<BaseConfig>& config) : m_config(config)
    {
        m_connectionCreator = std::shared_ptr<SqliteConnection>(new SqliteConnection(config));
        m_portionRepository = std::shared_ptr<SqlitePortionDataRepository>(new SqlitePortionDataRepository(m_connectionCreator));
        m_idleRepository = std::shared_ptr<SqliteIdleDataRepository>(new SqliteIdleDataRepository(m_connectionCreator));

    }
    void StoreIdleCollectionData(const std::vector<CollectedDataType> &data) override;
    void StorePortionCollectionData(const std::vector<CollectedDataType> &data) override;

    std::vector<ResultDataType> GetEuclideanMetricResult() override;
private:
    std::shared_ptr<BaseConfig> m_config;
    std::shared_ptr<SqliteIdleDataRepository> m_idleRepository;
    std::shared_ptr<SqlitePortionDataRepository> m_portionRepository;
    std::shared_ptr<SqliteConnection> m_connectionCreator;
};
