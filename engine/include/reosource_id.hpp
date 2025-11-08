#pragma once

#include <cstddef>
#include <functional>

namespace NArtEngine {

struct TResourceID {
    size_t id;

    bool operator==(const TResourceID& other) const {
        return id == other.id;
    }
};

}  // namespace NArtEngine

template <>
struct std::hash<NArtEngine::TResourceID> {
    std::size_t operator()(const NArtEngine::TResourceID& resource_id) const {
        return std::hash<decltype(resource_id.id)>()(resource_id.id);
    }
};
