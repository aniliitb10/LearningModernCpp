cmake_minimum_required(VERSION 3.12)
project(LearningModernCpp)

set(CMAKE_CXX_STANDARD 17)

set (GTEST_LIB_PATH "/home/anil/lib/googletest-release-1.8.0/googletest/libgtest.a")
set (GTEST_INCLUDE_PATH "/home/anil/lib/googletest-release-1.8.0/googletest/include/")
file(GLOB gTestSource "gTest/*.cpp")

include_directories( ${GTEST_INCLUDE_PATH} )

add_executable(LearningModernCpp main.cpp ${gTestSource})
target_link_libraries(LearningModernCpp ${GTEST_LIB_PATH} pthread)