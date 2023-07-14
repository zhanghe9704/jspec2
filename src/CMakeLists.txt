# CMakeLists.txt in the "src" directory

# include_directories(../include)

# link_directories(../lib)

file(GLOB_RECURSE sources *.cc)

# # Specify the path to the library folder
# link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(${JSPEC} ${sources})
