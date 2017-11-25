cmake_minimum_required(VERSION 3.0)
project(client)

find_package(Threads REQUIRED)
find_package(FLTK REQUIRED)

configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h)

add_executable (client autodoc.cpp)

target_link_libraries(client ${FLTK_LIBRARIES})
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(client clog)
target_link_libraries(client Node)
target_link_libraries(client UniversalUI)
target_link_libraries(client FLTKUI)
