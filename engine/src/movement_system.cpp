#include "movement_system.hpp"

#include "movement_component.hpp"
#include "position_component.hpp"

namespace NArtEngine {

void TMovementSystem::do_run(
    const TRenderingContext& context, const TEntitiesView& entities
) {
    for (const auto& entity : entities) {
        if (entity.has_component<TMovementComponent>() &&
            entity.has_component<TPositionComponent>()) {
            auto& movement = entity.get_component<TMovementComponent>();
            auto& position = entity.get_component<TPositionComponent>();

            position.position += static_cast<float>(context.dt) *
                                 movement.velocity * movement.direction;
        }
    }
}

}  // namespace NArtEngine
