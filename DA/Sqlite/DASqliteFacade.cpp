#include "DASqliteFacade.h"

std::vector<ResultDataType> DASqliteFacade::GetEuclideanMetricResult()
{
    return m_portionRepository->GetEuclideanMetricData();
}
void DASqliteFacade::StoreIdleCollectionData(const std::vector<CollectedDataType> &data)
{
    m_idleRepository->StoreData(data);
}
void DASqliteFacade::StorePortionCollectionData(const std::vector<CollectedDataType> &data) {
    m_portionRepository->StoreData(data);
}