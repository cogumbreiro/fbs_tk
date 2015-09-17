cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(fbs_tk)
set(CPM_MODULE_NAME "fbs_tk")
set(CPM_LIB_TARGET_NAME ${CPM_MODULE_NAME})

# CPM:

include(init-cpm.cmake) # download and initialize CPM

CPM_AddModule("flatbuffers"
  GIT_REPOSITORY "https://github.com/cogumbreiro/cpm-flatbuffers")

CPM_InitModule(${CPM_MODULE_NAME})

# Project details:

add_definitions (
    -Wall -pedantic -g
    -std=c++11
)

set(SRC
    fbs_tk/src/fbs-to-jsonl.cpp
)

include_directories(fbs_tk)
CPM_ExportAdditionalIncludeDir(fbs_tk)

add_library(${CPM_LIB_TARGET_NAME} ${SRC})
target_link_libraries(${CPM_LIB_TARGET_NAME} ${CPM_LIBRARIES})
CPM_ExportAdditionalLibraryTarget(${CPM_MODULE_NAME})

