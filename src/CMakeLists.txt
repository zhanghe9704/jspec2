# CMakeLists.txt in the "src" directory

include_directories(../include)

link_directories(../lib)

file(GLOB_RECURSE sources *.cc)

# Add your source files
add_executable(jspec2 ${sources})
