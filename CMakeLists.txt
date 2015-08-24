cmake_minimum_required (VERSION 2.8)
project (pseudosem)

option(PROJECT_STATIC_RUNTIME "Build with static runtime libs (/MT)" ON)

find_package (GTest REQUIRED)

set (TEST_SRC "${CMAKE_SOURCE_DIR}/include/pseudosem.h"
              "${CMAKE_SOURCE_DIR}/test/main.cpp")

include_directories ("${CMAKE_SOURCE_DIR}/include"
                    ${GTEST_INCLUDE_DIRS})

##############################
# System-Specific Settings
##############################

# GCC and MinGW settings.
IF (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    IF (PROJECT_STATIC_RUNTIME)
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
        set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
        set (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
    ENDIF ()
ENDIF ()

# MSVC settings.
IF (MSVC)
    # Force static C++ runtime linkage.
    IF (PROJECT_STATIC_RUNTIME)
        FOREACH(flag
            CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG_INIT
            CMAKE_CXX_FLAGS_RELEASE  CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_DEBUG  CMAKE_CXX_FLAGS_DEBUG_INIT)
            STRING(REPLACE "/MD"  "/MT" "${flag}" "${${flag}}")
            SET("${flag}" "${${flag}} /EHsc")
        ENDFOREACH()
    ENDIF ()
ENDIF ()

##############################
# Define Targets
##############################

add_executable        (tests ${TEST_SRC})
target_link_libraries (tests ${GTEST_BOTH_LIBRARIES})
