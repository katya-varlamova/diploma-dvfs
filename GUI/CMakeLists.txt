set( PROJECT_NAME GUI )
project( ${PROJECT_NAME} CXX )

add_executable_( ${PROJECT_NAME}
        DEPENDS
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        BL
        DA
        )
target_sources( ${PROJECT_NAME}
        PRIVATE
        main.cpp
        view.cpp
        view.h
        ui_view.h
        view.ui
        )
