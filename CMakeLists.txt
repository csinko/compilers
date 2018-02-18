project(mc CXX)
cmake_minimum_required(VERSION 3.5)

add_subdirectory(mc-compiler)

include_directories(.)
add_subdirectory(run)
