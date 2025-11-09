#include "lifetime_system.hpp"

#include "lifetime_component.hpp"

namespace NArtEngine {

void TLifetimeSystem::do_run(
    const TRenderingContext& context, const TEntitiesView& entities
) {
    for (const auto& entity : entities) {
        if (entity.has_component<TLifetimeComponent>()) {
            auto& lifetime_component =
                entity.get_component<TLifetimeComponent>();
            lifetime_component.frames -= 1;
            lifetime_component.time -= context.dt;
            if (lifetime_component.frames < 0 && lifetime_component.time < 0) {
                entity.remove();
            }
        }
    }
}

}  // namespace NArtEngine
