#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include "component.hpp"

namespace NArtEngine {

struct TPositionComponent : public TComponent<TPositionComponent> {
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    glm::quat rotation = glm::identity<glm::quat>();
};

}  // namespace NArtEngine
