#include "rendering_system.hpp"

// clang-format off
#include <glad/gl.h>
#include <GL/gl.h>
// clang-format on

#include "mesh.hpp"
#include "shader.hpp"

namespace NArtEngine {

void TRenderingSystem::do_run(
    const TRenderingContext& context, const TEntitiesView& entities
) {
    auto [width, height] = context.window->window_size();
    glViewport(0, 0, width, height);
    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& entity : entities) {
        if (entity.has_component<TMeshComponent>() &&
            entity.has_component<TShaderProgramComponent>()) {
            draw_entity(entity);
        }
    }
    context.window->swap_buffers();
}

void TRenderingSystem::draw_entity(const TEntity& entity) const {
    const auto& mesh   = entity.get_component<TMeshComponent>();
    const auto& shader = entity.get_component<TShaderProgramComponent>();

    glUseProgram(shader.program_id);
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices_count);
    glBindVertexArray(0);
}

}  // namespace NArtEngine
