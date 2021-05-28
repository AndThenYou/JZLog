
cmake_minimum_required(VERSION 2.8.12)

#output dir
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output) 

#include
set(EXTERN_INCLUDE_DIR ${EXTERN_INCLUDE_DIR} "./")
set(EXTERN_INCLUDE_DIR ${EXTERN_INCLUDE_DIR} "../include")
set(EXTERN_INCLUDE_DIR ${EXTERN_INCLUDE_DIR} "../include/glog")
include_directories(${EXTERN_INCLUDE_DIR})

#source
set(file_list "./JZCLogLibrary.cpp")
foreach(f IN LISTS file_list)
file(GLOB TMP ${f})
set(SRC_LIST ${SRC_LIST} ${TMP})
endforeach()

#define
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

#link
link_directories(../lib)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
set(DEFIMITION_FOR_SHARED ${DEFIMITION_FOR_SHARED} JZ_XEPORT)
set(DEFIMITION_FOR_STATIC ${DEFIMITION_FOR_STATIC} _LIB JZLOG_API=)
set(STATIC_DEPEND_LIB_FOR_SHARED_TARGET ${STATIC_DEPEND_LIB_FOR_SHARED_TARGET} glog)
set(STATIC_DEPEND_LIB_FOR_STATIC_TARGET ${STATIC_DEPEND_LIB_FOR_STATIC_TARGET} glog)
if(CMAKE_BUILD_TYPE MATCHES "Debug")
link_libraries(glogd)
else()
link_libraries(glog)
endif()

else()
link_libraries(-lglog -lpthread)
endif()
#target
add_library(JZLog SHARED ${SRC_LIST})
