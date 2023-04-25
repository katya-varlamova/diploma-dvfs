cmake_minimum_required(VERSION 3.23)


list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
include( common )

project(demo)

include( libs )
add_subdirectory( base )
add_subdirectory( BL )
add_subdirectory( DA )
add_subdirectory( Main )
add_subdirectory( Bench )