#include "shader.hpp"

#include <cstring>
#include <iostream>
#include <iterator>

// clang-format off
#include <glad/gl.h>
#include <GL/gl.h>
// clang-format on

#include "resource_manager.hpp"

namespace NArtEngine {

static constexpr const char* kShaderDelimiter = "// SHADER DELIMITER\n";

static GLuint create_shader(GLenum shader_type, const char* shader_program) {
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

static inline GLuint create_vertex_shader(const std::string& program) {
    return create_shader(GL_VERTEX_SHADER, program.c_str());
}

static inline GLuint create_fragment_shader(const std::string& program) {
    return create_shader(GL_FRAGMENT_SHADER, program.c_str());
}

EResourceLoadStatus create_shader_program(
    std::string filecontent, TShaderProgramComponent& program
) {
    auto pos = filecontent.find(kShaderDelimiter);
    if (pos == std::string::npos) {
        return EResourceLoadStatus::BROKEN_FILE_CONTENT;
    }

    program.program_id     = glCreateProgram();
    GLuint vertex_shader   = create_vertex_shader(filecontent.substr(0, pos));
    GLuint fragment_shader = create_fragment_shader(filecontent.substr(
        pos + std::strlen(kShaderDelimiter), std::string::npos
    ));

    glAttachShader(program.program_id, vertex_shader);
    glAttachShader(program.program_id, fragment_shader);
    glLinkProgram(program.program_id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int success;
    char info_log[512];
    glGetProgramiv(program.program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(
            program.program_id, sizeof(info_log), NULL, info_log
        );
        std::cerr << "Shader program link failed: " << info_log << std::endl;
        std::exit(4);
    }
    return EResourceLoadStatus::OK;
}

EResourceLoadStatus load_glsl_shader_resource(
    std::istream& in, TShaderProgramComponent& program
) {
    std::string filecontent{
        std::istreambuf_iterator<std::string::value_type>(in),
        std::istreambuf_iterator<std::string::value_type>()
    };

    return create_shader_program(filecontent, program);
}

}  // namespace NArtEngine
