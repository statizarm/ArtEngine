#pragma once

#include <cstddef>
#include <cstdint>

#include "component.hpp"

namespace NArtEngine {

struct TMeshComponent : public TComponent<TMeshComponent> {
  public:
    uint32_t vao;
    size_t vertices_count;

  public:
    static TComponentTypeID get_component_type_id() {
        return 2;
    }
};

}  // namespace NArtEngine
