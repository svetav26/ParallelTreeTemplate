project(parallel_tree_example)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB sources "*.cpp")

add_executable(vector_example ${sources})

target_include_directories(vector_example PRIVATE "../include")

# 64 bit
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(TARGET_PROC "x86_64")
else()
    set(TARGET_PROC "x86")
endif()

set(TARGET_OS "Linux")
set(TARGET_EXT ".so")

message("Detected ${TARGET_OS} ${TARGET_PROC}")

get_filename_component(
    PARALLEL_TREE_PATH
    "../lib/${TARGET_OS}_${TARGET_PROC}/libParallelTree${TARGET_EXT}"
    REALPATH BASE_DIR "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(vector_example PRIVATE ${PARALLEL_TREE_PATH})
