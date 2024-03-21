cmake_minimum_required(VERSION 3.10)
project(jspec2 VERSION 2.0 LANGUAGES CXX)

# Set the C++ standard you are using
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INSTALL_PREFIX /usr/local)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "$ENV{HOME}" CACHE PATH "..." FORCE)
endif()

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")


# Optionally, set a variable for the include directory
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Check if GSL is installed
find_package(GSL)

if(GSL_FOUND)
    message(STATUS "GSL found")
    include_directories(${GSL_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "GSL not found. Please install GSL.")
endif()

option(USE_OPENMP "Enable OpenMP" ON)

if(USE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        message(STATUS "OpenMP found")
    else()
        message(FATAL_ERROR "OpenMP not found")
    endif()
endif()

# Add the subdirectory for muparser
add_subdirectory(muparser)

# Specify the include directory
include_directories(${PROJECT_INCLUDE_DIR})

# Add all source files in the src directory to a variable
file(GLOB SOURCES "src/*.cc")

# Define your executable and its sources
add_executable(${PROJECT_NAME} ${SOURCES})

# Link GSL and muparser libraries to your project
if(GSL_FOUND)
    target_link_libraries(${PROJECT_NAME} GSL::gsl GSL::gslcblas muparser)
endif()

# Installation rules
install(TARGETS jspec2
        RUNTIME DESTINATION bin)
        
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${jspec2_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${jspec2_VERSION_MINOR}")
include(CPack)

option(BUILD_TESTS "Build the tests" OFF)
if(BUILD_TESTS)
    # Assuming a Unix-like system; adapt the command for Windows or others as necessary
    add_custom_command(TARGET jspec2 POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy
                       $<TARGET_FILE:jspec2> ${CMAKE_SOURCE_DIR}/tests/)
                       
    # Add a post-build command to copy the muparser shared library
    add_custom_target(copy_muparser_lib ALL
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:muparser>
            ${CMAKE_SOURCE_DIR}/tests/)

	add_dependencies(copy_muparser_lib muparser)
                      
    add_subdirectory(tests)
    
    add_custom_target(copy_tests_exec ALL
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:tests>
            ${CMAKE_SOURCE_DIR}/tests/)
    add_dependencies(copy_tests_exec tests)
endif()
