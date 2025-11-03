cmake_minimum_required(VERSION 3.15)

project(gachiball)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(engine)
add_subdirectory(examples)

target_link_libraries(window engine)
