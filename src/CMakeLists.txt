# CMakeLists.txt in the "src" directory

include_directories(../include)

link_directories(../lib)

file(GLOB_RECURSE sources *.cc)

# Add your source files
# add_executable(${JSPEC} ${sources})


# # set executive file depending on the operatong system
# if(CMAKE_HOST_SYSTEM MATCHES Linux)
#     message(STATUS "Build host runs Linux")
#     add_executable(jspec2 ${sources})  
# endif()
# # if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
# # endif()
# if(CMAKE_SYSTEM MATCHES Windows)
#     message(STATUS "Build host runs Windows")
#     add_executable(jspec2.exe ${sources})
# endif()


# include_directories(include)

# link_directories(${CMAKE_SOURCE_DIR}/lib)

# Specify the path to the library folder
link_directories(${CMAKE_SOURCE_DIR}/lib)

# # Find the library
# find_library(MUPARSER_LIB NAMES libmuparser.so.2 PATHS ${CMAKE_SOURCE_DIR}/lib)

# # set proper commands after detecting the operatong system
# if(CMAKE_HOST_SYSTEM MATCHES Linux)
#     message(STATUS "Build host runs Linux")
#     target_link_libraries(jspec2 libmuparser.so.2)
#     target_link_libraries(jspec2 gsl)
#     target_link_libraries(jspec2 gslcblas)
# endif()
# # if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
# # endif()
# if(CMAKE_SYSTEM MATCHES Windows)
#     message(STATUS "Build host runs Windows")
#     target_link_libraries(jspec2 muparser)
#     target_link_libraries(jspec2 gsl)
#     target_link_libraries(jspec2 cblas)
# endif()

# target_link_libraries(jspec2 gsl gslcblas ${MUPARSER_LIB})

if(UNIX) 
    add_executable(jspec ${sources})
elseif(WIN32)
    add_executable(jspec ${sources})
endif()