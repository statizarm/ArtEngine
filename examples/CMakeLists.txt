cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(resource_manager_autoreload)
add_subdirectory(rotating_camera)
add_subdirectory(simple_ecs)
add_subdirectory(simple_input)
add_subdirectory(simple_resource_manager)
add_subdirectory(window)
