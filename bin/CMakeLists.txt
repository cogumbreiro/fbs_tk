cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(test)

###
# CPM dependencies
###

include(${CMAKE_CURRENT_SOURCE_DIR}/../init-cpm.cmake) # download and initialize CPM

CPM_AddModule("flatbuffers"
  GIT_REPOSITORY "https://github.com/cogumbreiro/cpm-flatbuffers")

CPM_AddModule("fbs_tk"
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

CPM_Finish()

#####

function(my_add_executable target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} ${CPM_LIBRARIES})
	add_dependencies(${target} ${CPM_LIBRARIES})
endfunction()

##
# Project details:
##

add_definitions (
    -Wall -pedantic -g
    -std=c++11
)

my_add_executable(fbs2jsonl fbs2jsonl.cpp)
my_add_executable(jsonl2fbs jsonl2fbs.cpp)

