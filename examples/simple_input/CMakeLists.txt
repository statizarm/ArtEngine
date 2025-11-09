set(INCLUDES_DIR include)

set(
    SOURCES
    src/main.cpp
)

add_executable(simple-input ${SOURCES})

target_include_directories(
  simple-input
  PUBLIC
)
