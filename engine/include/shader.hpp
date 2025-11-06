#pragma once

#include "component.hpp"

namespace NArtEngine {

struct TShaderProgramComponent : public TComponent<TShaderProgramComponent> {
  public:
    uint32_t program_id;

  public:
    static TComponentTypeID get_component_type_id() {
        return 3;
    }
};

}  // namespace NArtEngine
