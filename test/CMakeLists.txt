cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(test)

###
# CPM dependencies
###

include(${CMAKE_CURRENT_SOURCE_DIR}/../init-cpm.cmake) # download and initialize CPM

CPM_AddModule("catch"
  GIT_REPOSITORY "https://github.com/bcachet/cpm-catch")

CPM_AddModule("flatbuffers"
  GIT_REPOSITORY "https://github.com/cogumbreiro/cpm-flatbuffers")

CPM_AddModule("fbs_tk"
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

CPM_Finish()

#####
#

function(my_add_test target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} ${CPM_LIBRARIES})
	add_dependencies(${target} ${CPM_LIBRARIES})
    add_test(${target} ${target})
endfunction()

##
# Project details:
##

add_definitions (
    -Wall -pedantic -g
    -std=c++11
)

enable_testing()

my_add_test(test-io-util test-io-util.cpp)
my_add_test(test-fbs-to-jsonl test-fbs-to-jsonl.cpp)

