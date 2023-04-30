#include "SqlitePortionDataRepository.h"
#include "common_macros.h"
static int callback(void *data, int argc, char **argv, char **cols) {
    UNUSED_VAR(cols)
    UNUSED_VAR(argc)
    auto res = (std::vector<ResultDataType> *) data;
    std::setlocale(LC_NUMERIC, "C");
    ResultDataType type = {std::stof(argv[1]), std::stoi(argv[0])};
    res->push_back(type);
    return 0;
}
std::vector<ResultDataType> SqlitePortionDataRepository::GetEuclideanMetricData() {
    auto conn = m_connectionCreator->GetConnect();
    std::string query = "with sleepdataavg as\n"
                        "(select avg(ct) as ct, avg(mrpi) as mrpi, avg(power) as power, rt, cpufreq\n"
                        "from sleepdata\n"
                        "group by cpufreq)\n"
                        ",\n"
                        "costs as\n"
                        "(select cpufreq, power / 2 as system_cost\n"
                        "from sleepdataavg \n"
                        "where cpufreq != 2701000)\n"
                        ",\n"
                        "dataavg as\n"
                        "(select avg(ct) as ct, avg(mrpi) as mrpi, avg(power) as power, cpuportion, cpufreq\n"
                        "from data\n"
                        "group by cpuportion, cpufreq)\n"
                        ",\n"
                        "tab as\n"
                        "(select ct, mrpi, power, system_cost, system_cost * ct as cost, (power - system_cost * ct) * (power - system_cost * ct) + ct * ct  as edp, cpuportion, dataavg.cpufreq\n"
                        "from dataavg join costs on dataavg.cpufreq = costs.cpufreq)\n"
                        "select cpufreq, mrpi\n"
                        "from tab as t1\n"
                        "where edp = (select min(edp) from tab as t2 where t1.cpuportion = t2.cpuportion)\n"
                        "order by cpuportion;";
    auto res = std::vector<ResultDataType>();
    char *err;
    int rc = sqlite3_exec(conn, query.c_str(), callback, (void *) &res, &err);
    std::string errStr = err ? err : "";
    sqlite3_free(err);

    if( rc != SQLITE_OK ){
        LoggerFactory::GetLogger()->LogInfo(
                ( std::string( "SqlitePortionDataRepository: GetEuclideanMetric: " ) + errStr).c_str()
        );
        throw DatabaseExecutionException( errStr);
    }
    m_connectionCreator->CloseConnect(conn);
    return res;
}
void SqlitePortionDataRepository::StoreData(const std::vector<CollectedDataType> &data)
{
    auto conn = m_connectionCreator->GetConnect();
    std::string query =  "INSERT into Data (cpuportion, attempt, cpufreq, ct, power, ipc, mrpi, inst, cycles, l3) values ";
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
                ( std::string( "SqlitePortionDataRepository: StoreData: " ) + errStr).c_str()
        );
        throw DatabaseExecutionException( errStr);
    }
    m_connectionCreator->CloseConnect(conn);
}