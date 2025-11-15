#include "movement_system.hpp"

#include "movement_component.hpp"
#include "position_component.hpp"

namespace NArtEngine {

void TMovementSystem::do_run(
    const TRenderingContext& context, const TEntitiesView& entities
) {
    for (const auto& entity : entities) {
        if (entity.has<TMovement>() && entity.has<TPosition>()) {
            auto& movement = entity.get<TMovement>();
            auto& position = entity.get<TPosition>();

            position.position += static_cast<float>(context.dt) *
                                 movement.velocity * movement.direction;
        }
    }
}

}  // namespace NArtEngine
