#pragma once

#include <glm/vec3.hpp>

#include "component.hpp"

namespace NArtEngine {

struct TMovement : public TComponent<TMovement> {
    glm::vec3 direction;
    float velocity;
};

}  // namespace NArtEngine
