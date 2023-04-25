set( PROJECT_NAME Main )
project( ${PROJECT_NAME} CXX )

add_executable_( ${PROJECT_NAME}
    DEPENDS
        BL
        DA
)

target_sources( ${PROJECT_NAME}
    PRIVATE
        main.cpp 
)