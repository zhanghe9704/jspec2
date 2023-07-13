cmake_minimum_required(VERSION 3.13)

# Set the CMP0079 policy to NEW
cmake_policy(SET CMP0079 NEW)

project(jspec2)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

include_directories(include)

# Specify the path to the library folder
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(src)

# Set the output directories for the debug and release versions
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/output/release)

set(JSPEC "jspec")
if(UNIX) 
    # Find the library
    find_library(MUPARSER_LIB NAMES libmuparser.so.2 PATHS ${CMAKE_SOURCE_DIR}/lib)
    target_link_libraries(${JSPEC} gsl gslcblas m ${MUPARSER_LIB})
elseif(WIN32)
    target_link_libraries(${JSPEC} gsl cblas muparser)
endif()

# Set compiler flags for Debug configuration
target_compile_options(${JSPEC}  PRIVATE $<$<CONFIG:Debug>:-O0 -g -Wall -Wextra>)

# Set compiler flags for Release configuration
target_compile_options(${JSPEC}  PRIVATE $<$<CONFIG:Release>:-O3>)

# Specify the output directory based on the build type
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set_target_properties(${JSPEC} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
else()
    set_target_properties(${JSPEC} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
endif()

# Check if the OpenMP flag is enabled
option(USE_OPENMP "Enable OpenMP support" OFF)

if(USE_OPENMP)
  find_package(OpenMP REQUIRED)
  if(OpenMP_CXX_FOUND)
    # Enable OpenMP support for C++
    target_compile_options(${JSPEC} PRIVATE ${OpenMP_CXX_FLAGS})
    target_link_libraries(${JSPEC} PRIVATE ${OpenMP_CXX_LIBRARIES})
  endif()
endif()
