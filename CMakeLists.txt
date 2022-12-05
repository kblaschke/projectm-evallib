cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_POSITION_INDEPENDENT_CODE YES)

project(projectM-EvalLib
    LANGUAGES C CXX
    VERSION 1.0
    )

set(PROJECTM_LIB_DIR "lib" CACHE STRING "Library installation directory, relative to the install prefix.")
set(PROJECTM_INCLUDE_DIR "include" CACHE STRING "Header installation directory, relative to the install prefix.")

option(ENABLE_DEBUG_POSTFIX "Add \"d\" after library names for debug builds" ON)
if(ENABLE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

find_package(BISON)

add_subdirectory(projectm-evallib)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()