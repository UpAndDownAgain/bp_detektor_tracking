cmake_minimum_required(VERSION 3.15)
project(bp_detektor_tracking)

set(CMAKE_CXX_STANDARD 11)


find_package(OpenCV REQUIRED)

add_executable(bp_detektor_tracking main.cpp classes/YoloDetektor.cpp classes/YoloDetektor.h)
target_link_libraries(bp_detektor_tracking ${OpenCV_LIBS})