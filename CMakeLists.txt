cmake_minimum_required(VERSION 3.13)

# Set the CMP0079 policy to NEW
cmake_policy(SET CMP0079 NEW)

project(jspec2)

add_subdirectory(src)

include_directories(include)

link_directories(lib)

# add_executable(jspec2 src/main.cc)

target_link_libraries(jspec2 muparser)
target_link_libraries(jspec2 gsl)
target_link_libraries(jspec2 cblas)