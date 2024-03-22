cmake_minimum_required(VERSION 3.10)
project(jspec2 VERSION 2.0 LANGUAGES CXX)

# Set the C++ standard you are using
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()



# Set a default CMAKE_INSTALL_PREFIX depending on the OS
if(WIN32)
    # For Windows, you might want to install in the user's AppData or a similar location
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "..." FORCE)
elseif(UNIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)

    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
      set(CMAKE_INSTALL_PREFIX "$ENV{HOME}" CACHE PATH "..." FORCE)
    endif()

    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()



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
if(WIN32)
    # Installation rules for the executable
    install(TARGETS jspec2
        RUNTIME DESTINATION ${PROJECT_NAME})

    # Installation rules for the DLLs
    install(FILES ${CMAKE_SOURCE_DIR}/build/muparser/libmuparser.dll
                  ${CMAKE_SOURCE_DIR}/build/gsl.dll
                  ${CMAKE_SOURCE_DIR}/build/gslcblas.dll
        DESTINATION ${PROJECT_NAME})

    install(CODE "
        execute_process(COMMAND \${CMAKE_COMMAND} -E remove_directory \"\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/bin\")
        execute_process(COMMAND \${CMAKE_COMMAND} -E remove_directory \"\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/lib\")
        execute_process(COMMAND \${CMAKE_COMMAND} -E remove_directory \"\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/include\")
    ")
elseif(UNIX)
    install(TARGETS jspec2
        RUNTIME DESTINATION bin)
        
    include(InstallRequiredSystemLibraries)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.md")
    set(CPACK_PACKAGE_VERSION_MAJOR "${jspec2_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${jspec2_VERSION_MINOR}")
    include(CPack)
endif()

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

    if(WIN32)
       add_custom_target(copy_gsl_lib ALL
           COMMAND ${CMAKE_COMMAND} -E copy_if_different
               "${CMAKE_SOURCE_DIR}/build/gsl.dll"
               "${CMAKE_SOURCE_DIR}/tests/"
           COMMAND ${CMAKE_COMMAND} -E copy_if_different
               "${CMAKE_SOURCE_DIR}/build/gslcblas.dll"
               "${CMAKE_SOURCE_DIR}/tests/"
       )
       add_dependencies(copy_gsl_lib GSL::gsl GSL::gslcblas)
   endif()
                      
    add_subdirectory(tests)
    
    add_custom_target(copy_tests_exec ALL
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:tests>
            ${CMAKE_SOURCE_DIR}/tests/)
    add_dependencies(copy_tests_exec tests)

   

endif()
