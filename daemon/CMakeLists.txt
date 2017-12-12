cmake_minimum_required(VERSION 3.0)
project(daemon)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
include_directories (${OpenCV_INCLUDE_DIRS})

configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h)

add_executable (daemon autodocd.cpp)

target_link_libraries(daemon ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(daemon clog)
target_link_libraries(daemon Node)
target_link_libraries(daemon lp)
target_link_libraries(daemon ${OpenCV_LIBRARIES})


