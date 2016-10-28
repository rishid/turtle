cmake_minimum_required(VERSION 2.8)
project(turtle)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
#include(GetGitRevisionDescription)
#git_describe(GIT_VERSION "--dirty")

# build output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# setup install directory paths
set(OUTPUT_BIN_DIR ${PROJECT_SOURCE_DIR}/output/bin)
set(OUTPUT_LIB_DIR ${PROJECT_SOURCE_DIR}/output/lib)
set(OUTPUT_INC_DIR ${PROJECT_SOURCE_DIR}/output/include/${CMAKE_PROJECT_NAME})

# setup compiler flags
set(C_COMMON_FLAGS "-mtune=native -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces")
set(C_DEBUG_FLAGS "-g -std=gnu99 -O0 ${C_COMMON_FLAGS}")
set(C_RELEASE_FLAGS "-g -std=gnu99 -O3 -ffast-math ${C_COMMON_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${C_DEBUG_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${C_RELEASE_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEBUG}")

#add_definitions("-DGIT_VERSION=${GIT_VERSION}")

# run tests but also show terminal output
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

macro(add_sources)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRCS "${_relPath}/${_src}")
        else()
            list (APPEND SRCS "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate SRCS to parent directory
        set (SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()

macro(add_includes)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND INCLUDES "${_relPath}/${_src}")
        else()
            list (APPEND INCLUDES "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate INCLUDES to parent directory
        set (INCLUDES ${INCLUDES} PARENT_SCOPE)
    endif()
endmacro()

enable_testing()

add_subdirectory(deps)
add_subdirectory(src)
add_subdirectory(tests)

