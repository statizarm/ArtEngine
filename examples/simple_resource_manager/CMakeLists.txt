set(INCLUDES_DIR include)

set(
    SOURCES
    src/main.cpp
)

add_executable(simple-resource-manager ${SOURCES})

target_include_directories(
  simple-resource-manager
  PUBLIC
)
