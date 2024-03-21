# Set the minimum required version of cmake for a project
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(jspec2_tests)

# Optionally set the C++ standard explicitly (e.g., C++11, C++14, C++17)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable for the test
add_executable(tests tests.cc catch_main.cc)

# If jspec2 or muparser require includes, add them here
target_include_directories(tests PRIVATE 
    ../include 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# If you're using features from C++11 or later in your tests
target_compile_features(tests PRIVATE cxx_std_11)
