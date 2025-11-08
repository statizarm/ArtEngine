set(INCLUDES_DIR include)

set(
    SOURCES
    src/main.cpp
)

add_executable(resource-manager-autoreload ${SOURCES})

target_include_directories(
  resource-manager-autoreload
  PUBLIC
)
