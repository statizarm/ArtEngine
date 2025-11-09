#include "input_event_system.hpp"

namespace NArtEngine {

struct THandledEventComponent : public TComponent<THandledEventComponent> {};

void TInputEventSystem::mark_event_handled(const TEntity& entity) {
    entity.add_component<THandledEventComponent>();
}

void TRemoveHandledEventSystem::do_run(
    const TRenderingContext& context, const TEntitiesView& entities
) {
    for (const auto& entity : entities) {
        if (entity.has_component<THandledEventComponent>()) {
            entity.remove();
        }
    }
}

}  // namespace NArtEngine
