#include "rendering_system.hpp"

// clang-format off
#include <glad/gl.h>
#include <GL/gl.h>
// clang-format on

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera_component.hpp"
#include "mesh.hpp"
#include "parent_component.hpp"
#include "position_component.hpp"
#include "shader.hpp"

namespace NArtEngine {

static constexpr const char* kInstanceMatrixName = "instanceMatrix";

static glm::mat4 get_entity_model(TEntity entity) {
    glm::mat4 model = glm::identity<glm::mat4>();
    if (entity.has<TPosition>()) {
        const auto& position = entity.get<TPosition>();

        model = glm::translate(
            glm::mat4_cast(position.rotation), position.position
        );
    }
    if (entity.has<TParent>()) {
        model = get_entity_model(entity.get<TParent>().parent) * model;
    }
    return model;
}

static glm::mat4 get_view(const TPosition* camera_position) {
    glm::mat4 view = glm::identity<glm::mat4>();
    if (camera_position) {
        view = glm::translate(
            glm::mat4_cast(camera_position->rotation),
            -camera_position->position
        );
    }
    return view;
}

static void draw_entity(
    const TEntity& entity, const TPosition* camera_position,
    glm::mat4 projection
) {
    const auto& mesh   = entity.get<TMesh>();
    const auto& shader = entity.get<TShaderProgram>();

    auto model = get_entity_model(entity);
    auto view  = get_view(camera_position);
    auto mvp   = projection * view * model;

    auto mvp_location = glGetUniformLocation(shader.program_id, "mvp");
    glUseProgram(shader.program_id);
    if (mvp_location >= 0) {
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
    }
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices_count);
    glBindVertexArray(0);
}

struct TInstanceData {
    const TMesh* mesh                     = nullptr;
    const TShaderProgram* shader          = nullptr;
    std::vector<glm::mat4> model_matrices = {};
};

static bool has_instance_matrix(const TShaderProgram* shader) {
    auto location =
        glGetAttribLocation(shader->program_id, kInstanceMatrixName);

    return location >= 0;
}

static void draw_entities(
    const TEntitiesView& entities, const TPosition* camera_position,
    glm::mat4 projection
) {
    auto view = get_view(camera_position);
    // FIXME: bug with shader per vao
    std::unordered_map<GLuint, TInstanceData> instances;
    for (const auto& entity : entities) {
        if (entity.has<TMesh>() && entity.has<TShaderProgram>()) {
            auto* mesh   = &entity.get<TMesh>();
            auto* shader = &entity.get<TShaderProgram>();
            if (has_instance_matrix(shader)) {
                instances[mesh->vao].mesh   = mesh;
                instances[mesh->vao].shader = shader;
                instances[mesh->vao].model_matrices.push_back(
                    projection * view * get_entity_model(entity)
                );
            } else {
                draw_entity(entity, camera_position, projection);
            }
        }
    }

    for (const auto& [vao, instance_data] : instances) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindVertexArray(instance_data.mesh->vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            instance_data.model_matrices.size() * sizeof(glm::mat4),
            instance_data.model_matrices.data(),
            GL_DYNAMIC_DRAW
        );

        auto instance_matrix_location = glGetAttribLocation(
            instance_data.shader->program_id, kInstanceMatrixName
        );
        for (int i = 0; i < 4; ++i) {
            auto location = instance_matrix_location + i;
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(
                location,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glm::mat4),
                reinterpret_cast<void*>(i * sizeof(glm::vec4))
            );
            glVertexAttribDivisor(location, 1);
        }

        glUseProgram(instance_data.shader->program_id);

        glDrawArraysInstanced(
            GL_TRIANGLES,
            0,
            instance_data.mesh->vertices_count,
            instance_data.model_matrices.size()
        );
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
    }
}

void TRenderingSystem::do_run(
    const TRenderingContext& context, const TEntitiesView& entities
) {
    auto [width, height] = context.window->window_size();
    glViewport(0, 0, width, height);
    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    TPosition* camera_position = nullptr;
    glm::mat4 projection       = glm::perspective(
        glm::radians(45.f),
        static_cast<float>(width) / static_cast<float>(height),
        0.1f,
        100.f
    );
    for (const auto& entity : entities) {
        if (entity.has<TCamera>() && entity.has<TPosition>()) {
            camera_position = &entity.get<TPosition>();
            break;
        }
    }
    draw_entities(entities, camera_position, projection);
    context.window->swap_buffers();
}

}  // namespace NArtEngine
