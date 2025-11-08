cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(INCLUDES_DIR include)
set(
    INCLUDES
    ${INCLUDES_DIR}/string_literal.hpp
    ${INCLUDES_DIR}/identifier_generator.hpp
    ${INCLUDES_DIR}/identifier.hpp
)
set(
    SOURCES
    src/identifier_generator.cpp
)

add_library(utils ${SOURCES} ${INCLUDES})

target_include_directories(
  utils
  PUBLIC
  include
)
