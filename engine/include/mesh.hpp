#pragma once

#include <cstddef>
#include <cstdint>

#include "component.hpp"
#include "resource_manager.hpp"

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

EResourceLoadStatus load_text_mesh(std::istream& in, TMeshComponent& mesh);

template <>
inline EResourceLoadStatus load_resource<TMeshComponent, EResourceFormat::TEXT>(
    std::istream& in, TMeshComponent& mesh
) {
    return load_text_mesh(in, mesh);
}

}  // namespace NArtEngine
