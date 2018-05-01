cmake_minimum_required (VERSION 2.8.1)
project (cpplogger_test)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_SYSTEM_PROCESSOR .)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -O0 -Wall -DNDEBUG -fprofile-arcs -ftest-coverage -fmessage-length=0 ")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -W -DNDEBUG -fprofile-arcs -ftest-coverage ")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage ")

set (LOGGER_VERSION_MAJOR 0)
set (LOGGER_VERSION_MINOR 1)

find_library(
    GMOCK_LIB
    NAMES gmock
    PATHS "../../../test/lib/${CMAKE_SYSTEM_PROCESSOR}"
)

include_directories(
    "../../../cpplogger/"  
)

add_executable(cpplogger_test
	../../../cpplogger/Logger.cpp
    ../Logger_test.cpp
)

target_link_libraries (
    cpplogger_test
    pthread 
    ${GMOCK_LIB}
    gcov
)
