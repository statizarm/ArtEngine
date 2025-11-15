#include "lifetime_system.hpp"

#include "lifetime_component.hpp"

namespace NArtEngine {

void TLifetimeSystem::run(
    const TRenderingContext& context, const TEntitiesView<TEntity>& entities
) {
    for (const auto& entity : entities) {
        if (entity.has<TLifetime>()) {
            auto& lifetime_component = entity.get<TLifetime>();
            lifetime_component.frames -= 1;
            lifetime_component.time -= context.dt;
            if (lifetime_component.frames < 0 && lifetime_component.time < 0) {
                entity.remove();
            }
        }
    }
}

}  // namespace NArtEngine
