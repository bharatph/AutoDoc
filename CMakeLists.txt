cmake_minimum_required(VERSION 3.0)
project(AutoDoc)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-fPIC -Wall -std=c++11")

option(ENABLE_CLOG "Enables logging" ON)
set (BUFFER_SIZE 256)

add_subdirectory(clog ${CMAKE_SOURCE_DIR}/build/clog)
add_subdirectory(Node ${CMAKE_SOURCE_DIR}/build/Node)

#daemon
add_subdirectory(${PROJECT_SOURCE_DIR}/daemon ${CMAKE_SOURCE_DIR}/build/daemon)

#UniversalUI
add_subdirectory(UniversalUI ${CMAKE_SOURCE_DIR}/build/UniversalUI)

# modules to use
add_subdirectory(UniversalUI/uui_modules/FLTKUI ${CMAKE_SOURCE_DIR}/build/FLTKUI)
add_subdirectory(UniversalUI/uui_modules/CharacterUI ${CMAKE_SOURCE_DIR}/build/CharacterUI)

#client application
include_directories(${clog_SOURCE_DIR} ${Node_SOURCE_DIR})
add_subdirectory(${PROJECT_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/build/client)
