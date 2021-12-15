cmake_minimum_required(VERSION 3.1)

#download and populate the gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        97a467571a0f615a4d96e79e4399c43221ca1232
)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(utils_test src/utils_test.cpp)
target_link_libraries(utils_test PUBLIC utils gtest_main)


file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests/)

gtest_discover_tests(utils_test)
