include(FetchContent)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad
    GIT_TAG        v2.0.6
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    message("Fetching glad")
    FetchContent_MakeAvailable(glad)

    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(glad REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6 EXTENSIONS GL_ARB_bindless_texture GL_EXT_texture_compression_s3tc)
endif()

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

set(INCLUDES_DIR include)
set(
    INCLUDES
    ${INCLUDES_DIR}/component.hpp
    ${INCLUDES_DIR}/component_type_id.hpp
    ${INCLUDES_DIR}/ecs_engine.hpp
    ${INCLUDES_DIR}/entities_view.hpp
    ${INCLUDES_DIR}/entity.hpp
    ${INCLUDES_DIR}/entity_id.hpp
    ${INCLUDES_DIR}/game.hpp
    ${INCLUDES_DIR}/game_engine.hpp
    ${INCLUDES_DIR}/game_engine_config.hpp
    ${INCLUDES_DIR}/mesh.hpp
    ${INCLUDES_DIR}/rendering_context.hpp
    ${INCLUDES_DIR}/rendering_system.hpp
    ${INCLUDES_DIR}/resource_managed_component.hpp
    ${INCLUDES_DIR}/resource_manager.hpp
    ${INCLUDES_DIR}/resource_manager_config.hpp
    ${INCLUDES_DIR}/resource_manager_system.hpp
    ${INCLUDES_DIR}/shader.hpp
    ${INCLUDES_DIR}/system.hpp
    ${INCLUDES_DIR}/window.hpp
)
set(
    SOURCES
    src/ecs_engine.cpp
    src/entity.cpp
    src/game.cpp
    src/game_engine.cpp
    src/mesh.cpp
    src/rendering_system.cpp
    src/resource_manager.cpp
    src/resource_manager_system.cpp
    src/shader.cpp
    src/window.cpp
)

add_library(engine ${SOURCES} ${INCLUDES})

target_include_directories(
  engine
  PUBLIC
  include
)

target_link_libraries(
  engine
  PUBLIC utils
  PUBLIC glfw
  PUBLIC OpenGL::GL
  PRIVATE glad
)
