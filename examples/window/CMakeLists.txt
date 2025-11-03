set(INCLUDES_DIR include)

set(
    SOURCES
    src/main.cpp
)

add_executable(window ${SOURCES})

target_include_directories(
  window
  PUBLIC
)
