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
    float number;
    for (size_t i = 0; (in >> number); ++i) {
        if (i % 3 == 0) {
            vertices.emplace_back();
        }
        vertices[i / 3][i % 3] = number;
    }

    GLuint vbo;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(*vertices.data()),
        static_cast<void*>(vertices.data()),
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        kPositionAttribLocation,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        static_cast<void*>(0)
    );
    glEnableVertexAttribArray(kPositionAttribLocation);
    glBindVertexArray(0);

    mesh.vertices_count = vertices.size();

    return EResourceLoadStatus::OK;
}

}  // namespace NArtEngine
