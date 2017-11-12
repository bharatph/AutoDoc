cmake_minimum_required (VERSION 3.0)
project (AutoDoc)

option(ENABLE_LOG "Enables logging" OFF)

set (CMAKE_CXX_FLAGS "-fPIC -Wall -std=c++11")

##version number
set (autodoc_VERSION_MAJOR 0)
set (autodoc_VERSION_MINOR 4)

#miscellaneous
set (BUFFER_SIZE 256)

#configure file to add CMake settings to it
configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
	)

include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_BINARY_DIR}")

if(ENABLE_LOG)
	add_definitions(-DENABLE_LOG)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/clog ${PROJECT_SOURCE_DIR}/clog/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/Node ${PROJECT_SOURCE_DIR}/Node/build)

include_directories(${clog_SOURCE_DIR} ${Node_SOURCE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/daemon ${PROJECT_SOURCE_DIR}/daemon/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/client ${PROJECT_SOURCE_DIR}/client/build)

