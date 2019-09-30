cmake_minimum_required(VERSION 3.12)
project(LearningModernCpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB source "source/*.cpp")
file(GLOB gTestSource "test/*.cpp")

add_executable(LearningModernCpp main.cpp ${source} ${gTestSource})
target_link_libraries(LearningModernCpp gtest pthread)