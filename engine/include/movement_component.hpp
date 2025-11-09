#pragma once

#include <glm/vec3.hpp>

#include "component.hpp"

namespace NArtEngine {

struct TMovementComponent : public TComponent<TMovementComponent> {
    glm::vec3 direction;
    float velocity;
};

}  // namespace NArtEngine
