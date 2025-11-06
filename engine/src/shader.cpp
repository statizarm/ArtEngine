#include "shader.hpp"

// clang-format off
#include <glad/gl.h>
#include <GL/gl.h>
#include <iostream>
// clang-format on

#include "resource_manager.hpp"

namespace NArtEngine {

static GLuint CreateShader(GLenum shader_type, const char* shader_program) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_program, NULL);
    glCompileShader(shader);

    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        std::cerr << "Shader compile failed: " << info_log << std::endl;
        std::exit(3);
    }

    return shader;
}

static const char* kVertexShaderProgram =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "out vec4 fColor;\n"
    "void main() {\n"
    "    fColor = vec4(position, 1);\n"
    "    gl_Position = vec4(position, 1);\n"
    "}\n";
static inline GLuint CreateVertexShader() {
    return CreateShader(GL_VERTEX_SHADER, kVertexShaderProgram);
}

static const char* kFragmentShaderProgram =
    "#version 330 core\n"
    "in vec4 fColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = fColor;\n"
    "}\n";
static inline GLuint CreateFragmentShader() {
    return CreateShader(GL_FRAGMENT_SHADER, kFragmentShaderProgram);
}

GLuint CreateShaderProgram() {
    GLuint shader_program  = glCreateProgram();
    GLuint vertex_shader   = CreateVertexShader();
    GLuint fragment_shader = CreateFragmentShader();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        std::cerr << "Shader program link failed: " << info_log << std::endl;
        std::exit(4);
    }
    return shader_program;
}

template <>
EResourceLoadStatus
load_resource<TShaderProgramComponent, EResourceFormat::TEXT>(
    std::istream& in, TShaderProgramComponent& program
) {
    program.program_id = CreateShaderProgram();
    return EResourceLoadStatus::OK;
}

}  // namespace NArtEngine
