set( PROJECT_NAME Bench )
project( ${PROJECT_NAME} CXX )

add_executable_( ${PROJECT_NAME} )

target_sources( ${PROJECT_NAME}
    PRIVATE
        main.cpp 
)
