#pragma once

#include "component.hpp"
#include "resource_manager.hpp"

namespace NArtEngine {

struct TShaderProgram : public TComponent<TShaderProgram> {
    uint32_t program_id;
};

EResourceLoadStatus load_glsl_shader_resource(std::istream&, TShaderProgram&);

template <>
inline EResourceLoadStatus load_resource<TShaderProgram, EResourceFormat::GLSL>(
    std::istream& in, TShaderProgram& program
) {
    return load_glsl_shader_resource(in, program);
}

}  // namespace NArtEngine
