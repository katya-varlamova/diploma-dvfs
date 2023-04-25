#include "SqliteIdleDataRepository.h"

void SqliteIdleDataRepository::StoreData(const std::vector<CollectedDataType> &data)
{
    auto conn = m_connectionCreator->GetConnect();
    std::string query =  "INSERT into SleepData (rt, attempt, cpufreq, ct, power, ipc, mrpi, inst, cycles, l3) values ";
    for (int i = 0; i < data.size(); i++) {
        auto row = data[i];
        query += "(" + std::to_string(row.descriptor) + ", " +
                 std::to_string(row.attempt) + ", " +
                 std::to_string(row.cpufreq) + ", " +
                 std::to_string(row.ct) + ", " +
                 std::to_string(row.power) + ", " +
                 std::to_string(row.ipc) + ", " +
                 std::to_string(row.mrpi) + ", " +
                 std::to_string(row.inst) + ", " +
                 std::to_string(row.cycles) + ", " +
                 std::to_string(row.l3) + ")";
        if (i != data.size() - 1)
            query += ", ";
    }
    char *err;
    int rc = sqlite3_exec(conn, query.c_str(), NULL, 0, &err);
    std::string errStr = err ? err : "";
    sqlite3_free(err);

    if( rc != SQLITE_OK ){
        LoggerFactory::GetLogger()->LogInfo(
                ( std::string( "SqliteIdleDataRepository: StoreData: " ) + errStr).c_str()
        );
        throw DatabaseExecutionException( errStr.c_str());
    }
    m_connectionCreator->CloseConnect(conn);
}