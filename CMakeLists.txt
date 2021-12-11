cmake_minimum_required(VERSION 3.2)
set(CMAKE_C_COMPILER "/usr/local/bin/mpicc")

project(mpi_proj C CXX)

find_package(MPI REQUIRED)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

include_directories(SYSTEM ${MPI_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/include)

#basic utilities
add_library(utils src/utils.c)
target_link_libraries(utils -lm)

#core functions
add_library(core_fun src/core_func.c)
target_link_libraries(core_fun utils -lm)

add_executable(sprogram src/sprogram.c)
target_link_libraries(sprogram ${MPI_C_LIBRARIES} core_fun utils)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY out DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ses.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
