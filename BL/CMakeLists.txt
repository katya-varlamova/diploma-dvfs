set( PROJECT_NAME BL )
project( ${PROJECT_NAME} CXX )

add_library_( ${PROJECT_NAME} STATIC
        DEPENDS
        base
        )

target_sources( ${PROJECT_NAME}
        PRIVATE
        core/ISystemController.h
        core/IPmuCollector.h
        workloads/IWorkload.cpp
        core/PmuCollector.cpp
        core/PmuCollector.h
        core/SystemController.cpp
        core/SystemController.h
        workloads/IWorkload.h
        DataCollection/Collector.cpp
        DataCollection/Collector.h
        Runner/IRunner.h
        core/RunnerCollector.cpp
        core/RunnerCollector.h
        workloads/IdleWorkload.cpp
        workloads/IdleWorkload.h
        workloads/PortionWorkload.cpp
        workloads/PortionWorkload.h
        Runner/IOptimizer.h
        Runner/DemoRunner.cpp
        Runner/DemoRunner.h
        Runner/Optimizer.cpp
        Runner/Optimizer.h
        DataCollection/DataTypes.h
        IDAFacade.h
        Runner/BasicRunner.cpp
        Runner/BasicRunner.h)
