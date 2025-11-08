#pragma once

#include "component.hpp"
#include "resource_manager.hpp"

namespace NArtEngine {

struct TShaderProgramComponent : public TComponent<TShaderProgramComponent> {
  public:
    uint32_t program_id;

  public:
    static TComponentTypeID get_component_type_id() {
        return 3;
    }
};

EResourceLoadStatus
load_glsl_shader_resource(std::istream&, TShaderProgramComponent&);

template <>
inline EResourceLoadStatus
load_resource<TShaderProgramComponent, EResourceFormat::GLSL>(
    std::istream& in, TShaderProgramComponent& program
) {
    return load_glsl_shader_resource(in, program);
}

}  // namespace NArtEngine
