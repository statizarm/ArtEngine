#pragma once

#include <cstddef>
#include <cstdint>

#include "component.hpp"
#include "resource_manager.hpp"

namespace NArtEngine {

struct TMesh : public TComponent<TMesh> {
    uint32_t vao;
    size_t vertices_count;
};

EResourceLoadStatus load_text_mesh(std::istream& in, TMesh& mesh);

template <>
inline EResourceLoadStatus load_resource<TMesh, EResourceFormat::TEXT>(
    std::istream& in, TMesh& mesh
) {
    return load_text_mesh(in, mesh);
}

}  // namespace NArtEngine
