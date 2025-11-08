#pragma once

#include <cstddef>
#include <cstdint>

#include "component.hpp"
#include "resource_manager.hpp"

namespace NArtEngine {

struct TMeshComponent : public TComponent<TMeshComponent> {
    uint32_t vao;
    size_t vertices_count;
};

EResourceLoadStatus load_text_mesh(std::istream& in, TMeshComponent& mesh);

template <>
inline EResourceLoadStatus load_resource<TMeshComponent, EResourceFormat::TEXT>(
    std::istream& in, TMeshComponent& mesh
) {
    return load_text_mesh(in, mesh);
}

}  // namespace NArtEngine
