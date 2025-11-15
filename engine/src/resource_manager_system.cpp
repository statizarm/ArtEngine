#include "resource_manager_system.hpp"

#include "resource_managed_component.hpp"

namespace NArtEngine {

void TResourceManagerSystem::run(
    const TRenderingContext& context, const TEntitiesView<TEntity>& entities
) {
    for (const auto& entity : entities) {
        if (entity.has<TResourceManaged>()) {
            const auto& component = entity.get<TResourceManaged>();
            for (const auto& [component_id, resource_id] :
                 component.component_resources) {
                const auto& res = component.resource_manager->load(
                    resource_id, entity.get_component(component_id)
                );
            }
        }
    }
}

}  // namespace NArtEngine
