#include "rendering_system.hpp"

// clang-format off
#include <glad/gl.h>
#include <GL/gl.h>
// clang-format on

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera_component.hpp"
#include "mesh.hpp"
#include "position_component.hpp"
#include "shader.hpp"

namespace NArtEngine {

static void draw_entity(
    const TEntity& entity, const TPositionComponent* camera_position,
    glm::mat4 projection
) {
    const auto& mesh   = entity.get_component<TMeshComponent>();
    const auto& shader = entity.get_component<TShaderProgramComponent>();

    glm::mat4 view = glm::identity<glm::mat4>();
    if (camera_position) {
        view = glm::translate(
            glm::mat4_cast(camera_position->rotation),
            -camera_position->position
        );
    }
    glm::mat4 model = glm::identity<glm::mat4>();
    if (entity.has_component<TPositionComponent>()) {
        const auto& position = entity.get_component<TPositionComponent>();

        model = glm::translate(
            glm::mat4_cast(position.rotation), position.position
        );
    }

    auto mvp = projection * view * model;

    auto mvp_location = glGetUniformLocation(shader.program_id, "mvp");
    glUseProgram(shader.program_id);
    if (mvp_location >= 0) {
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
    }
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices_count);
    glBindVertexArray(0);
}

void TRenderingSystem::do_run(
    const TRenderingContext& context, const TEntitiesView& entities
) {
    auto [width, height] = context.window->window_size();
    glViewport(0, 0, width, height);
    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    TPositionComponent* camera_position_component = nullptr;

    glm::mat4 projection = glm::perspective(
        glm::radians(45.f),
        static_cast<float>(width) / static_cast<float>(height),
        0.1f,
        100.f
    );

    for (const auto& entity : entities) {
        if (entity.has_component<TCameraComponent>() &&
            entity.has_component<TPositionComponent>()) {
            camera_position_component =
                &entity.get_component<TPositionComponent>();
            break;
        }
    }

    for (const auto& entity : entities) {
        if (entity.has_component<TMeshComponent>() &&
            entity.has_component<TShaderProgramComponent>()) {
            draw_entity(entity, camera_position_component, projection);
        }
    }
    context.window->swap_buffers();
}

}  // namespace NArtEngine
