#include "mesh.hpp"

// clang-format off
#include <glad/gl.h>
#include <GL/gl.h>
// clang-format on

#include <glm/vec3.hpp>
#include <vector>

#include "resource_manager.hpp"

namespace NArtEngine {

static constexpr int kPositionAttribLocation = 0;

template <>
EResourceLoadStatus load_resource<TMeshComponent, EResourceFormat::TEXT>(
    std::istream& in, TMeshComponent& mesh
) {
    std::vector<glm::vec3> vertices;
    while (in) {
        vertices.emplace_back();
        for (int i = 0; i < 3; ++i) {
            if (!in) {
                return EResourceLoadStatus::BROKEN_FILE_CONTENT;
            }
            in >> vertices.back()[i];
        }
    }

    GLuint vbo;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        vbo,
        vertices.size() * sizeof(*vertices.data()),
        static_cast<void*>(vertices.data()),
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        kPositionAttribLocation,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(*vertices.data()),
        static_cast<void*>(0)
    );
    glEnableVertexAttribArray(kPositionAttribLocation);
    glBindVertexArray(0);

    mesh.vertices_count = vertices.size();

    return EResourceLoadStatus::OK;
}

}  // namespace NArtEngine
