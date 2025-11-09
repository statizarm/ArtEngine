set(INCLUDES_DIR include)

set(
    SOURCES
    src/main.cpp
)

add_executable(rotating-camera ${SOURCES})

target_include_directories(
  rotating-camera
  PUBLIC
)
