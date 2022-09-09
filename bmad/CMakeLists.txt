cmake_minimum_required(VERSION $ENV{ACC_CMAKE_VERSION})
project(C CXX)

set (LIBNAME cpp_bmad_interface)

set(INC_DIRS 
  include
)

set(SRC_DIRS
  code
)

set(EXE_SPECS 
  cmake.jspec_bmad
)

#include($ENV{ACC_BUILD_SYSTEM}/Master.cmake)