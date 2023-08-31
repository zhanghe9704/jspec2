cmake_minimum_required(VERSION $ENV{ACC_CMAKE_VERSION})
project(JSPEC_Fortran LANGUAGES CXX Fortran)


set (LIBNAME JSPEC)

set(INC_DIRS 
  include
  ../include
)

set(SRC_DIRS
  src
  ../src
)

set(EXE_SPECS 
  cmake.jspec
)

#include($ENV{ACC_BUILD_SYSTEM}/Master.cmake)