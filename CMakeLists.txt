cmake_minimum_required(VERSION 3.10)

project(JVM)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(source)
add_subdirectory(test)