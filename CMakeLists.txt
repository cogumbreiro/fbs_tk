cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(fbs2jsonl)

###
# CPM dependencies
###

include(init-cpm.cmake) # download and initialize CPM

CPM_AddModule("catch"
  GIT_REPOSITORY "https://github.com/bcachet/cpm-catch")

CPM_AddModule("flatbuffers"
  GIT_REPOSITORY "https://github.com/cogumbreiro/cpm-flatbuffers")

CPM_Finish()

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

enable_testing()

set(SRC
    src/io-util.cpp
    src/fbs-to-jsonl.cpp
)

set(FBS_TO_JSONL fbstojsonl)
add_library(${FBS_TO_JSONL} STATIC ${SRC})

# Generate tests:
my_add_test(test-io-util src/test-io-util.cpp)
my_add_test(test-fbs-to-jsonl src/test-fbs-to-jsonl.cpp)

# Generate programs:
my_add_executable(fbs2jsonl src/fbs2jsonl.cpp)
my_add_executable(jsonl2fbs src/jsonl2fbs.cpp)
