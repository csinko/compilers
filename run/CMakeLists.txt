set(CMAKE_CXX_FLASGS "-std=c++1z")

add_executable(mc-compiler main.cpp)
target_link_libraries(mc-compiler mc)
