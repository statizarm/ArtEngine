set(INCLUDES_DIR include)

set(
    SOURCES
    src/main.cpp
)

add_executable(cube-of-cubes ${SOURCES})

target_include_directories(
  cube-of-cubes
  PUBLIC
)
