
cmake_minimum_required(VERSION 2.8.12)

project(demo)

#output dir
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../output) 

#include
set(EXTERN_INCLUDE_DIR ${EXTERN_INCLUDE_DIR} "../include")
include_directories(${EXTERN_INCLUDE_DIR})

#source
set(file_list "./main.cpp")
foreach(f IN LISTS file_list)
file(GLOB TMP ${f})
set(SRC_LIST ${SRC_LIST} ${TMP})
endforeach()

#link
if(CMAKE_SYSTEM_NAME MATCHES "Windows")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
link_directories(../output/Debug)
else()
link_directories(../output/Release)
endif()
link_libraries(JZLog.dll)
else()
link_directories(../output)
link_libraries(-lJZLog -lpthread -ldl -lrt)
endif()
#target
add_executable(demo ${SRC_LIST})
