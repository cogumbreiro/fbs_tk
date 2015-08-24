cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(fbs_tk)
set(CPM_MODULE_NAME "fbs_tk")
set(CPM_LIB_TARGET_NAME ${CPM_MODULE_NAME})

###
# CPM dependencies
###

include(init-cpm.cmake) # download and initialize CPM

CPM_AddModule("flatbuffers"
  GIT_REPOSITORY "https://github.com/cogumbreiro/cpm-flatbuffers")

CPM_InitModule(${CPM_MODULE_NAME})

function(my_add_test target)
	my_add_executable(${target} ${ARGN})
    add_test(${target} ${target})
endfunction()

function(my_add_executable target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} ${FBS_TO_JSONL})
    target_link_libraries(${target} ${CPM_LIBRARIES})
endfunction()

##
# Project details:
##

add_definitions (
    -Wall -pedantic -g
    -std=c++11
)

set(SRC
    fbs_tk/src/io-util.cpp
    fbs_tk/src/fbs-to-jsonl.cpp
)

include_directories(fbs_tk)
CPM_ExportAdditionalIncludeDir(fbs_tk)

add_library(${CPM_LIB_TARGET_NAME} ${SRC})
target_link_libraries(${CPM_LIB_TARGET_NAME} ${CPM_LIBRARIES})
CPM_ExportAdditionalLibraryTarget(${CPM_MODULE_NAME})

# Generate programs:
#my_add_executable(fbs2jsonl src/fbs2jsonl.cpp)
#my_add_executable(jsonl2fbs src/jsonl2fbs.cpp)
