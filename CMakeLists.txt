cmake_minimum_required(VERSION 3.2)
set(CMAKE_C_COMPILER "/usr/local/bin/mpicc")

project(mpi_proj C CXX)

find_package(MPI REQUIRED)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

include_directories(SYSTEM ${MPI_INCLUDE_PATH})
add_executable(hello_world src/test.c)
target_link_libraries(hello_world ${MPI_C_LIBRARIES})