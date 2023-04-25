set( PROJECT_NAME DA )
project( ${PROJECT_NAME} CXX )

add_library_( ${PROJECT_NAME} STATIC
        DEPENDS
        SQLite::SQLite3
        base
        BL
        )

target_sources( ${PROJECT_NAME}
        PRIVATE
        Sqlite/SqliteConnection.cpp 
        Sqlite/SqliteConnection.h 
        Sqlite/SqlitePortionDataRepository.cpp 
        Sqlite/SqlitePortionDataRepository.h 
        Sqlite/SqliteIdleDataRepository.cpp 
        Sqlite/SqliteIdleDataRepository.h
        Sqlite/DASqliteFacade.cpp
        Sqlite/DASqliteFacade.h)
