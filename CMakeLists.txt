cmake_minimum_required (VERSION 2.8)
project (AutoDoc)

find_package(OpenCV REQUIRED)
find_package(FLTK REQUIRED)
find_package(Threads REQUIRED)
include_directories (${OpenCV_INCLUDE_DIRS})

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

add_subdirectory(clog ${CMAKE_CURRENT_SOURCE_DIR}/clog)
add_subdirectory(Node ${CMAKE_CURRENT_SOURCE_DIR}/Node)

add_executable (autodoc autodoc.cpp)
add_executable (autodocd autodocd.cpp)

target_link_libraries(autodocd ${OpenCV_LIBS})
target_link_libraries(autodocd ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(autodoc ${FLTK_LIBRARIES})
target_link_libraries(autodoc ${OpenCV_LIBS})
target_link_libraries(autodoc ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(autodoc clog)
target_link_libraries(autodoc Node)
