cmake_minimum_required(VERSION 3.15)
project(bp_detektor_tracking)

set(CMAKE_CXX_STANDARD 11)


find_package(OpenCV REQUIRED)

add_executable(bp_detektor_tracking main.cpp)
target_link_libraries(bp_detektor_tracking ${OpenCV_LIBS})