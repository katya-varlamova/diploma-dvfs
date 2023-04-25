#pragma once
#include <sqlite3.h>
#include <string>
#include "exceptions/database_exceptions.h"
#include "config/base_config.h"

using SQLiteConnPtr = sqlite3 *;
class SqliteConnection {
public:
    SqliteConnection(const std::shared_ptr<BaseConfig>& conf) { // TODO config
        m_connStr = conf->GetStringField( { DbNameSection } ) ;
    }
    SQLiteConnPtr GetConnect() {
        SQLiteConnPtr ptr;
        int rc = sqlite3_open(m_connStr.c_str(), &ptr);
        if (rc)
            throw DatabaseConnectException( "unable to connect" );
        return ptr;
    }
    void CloseConnect(SQLiteConnPtr conn) {
        sqlite3_close(conn);
    }
private:
    std::string m_connStr;
};
