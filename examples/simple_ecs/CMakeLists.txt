set(INCLUDES_DIR include)

set(
    SOURCES
    src/main.cpp
)

add_executable(simple-ecs ${SOURCES})

target_include_directories(
  simple-ecs
  PUBLIC
)
