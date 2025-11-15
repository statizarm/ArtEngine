#include "input_event_system.hpp"

namespace NArtEngine {

struct THandledEvent : public TComponent<THandledEvent> {};

void TInputEventSystem::mark_event_handled(const TEntity& entity) {
    entity.add<THandledEvent>();
}

void TRemoveHandledEventSystem::run(
    const TRenderingContext& context, const TEntitiesView& entities
) {
    for (const auto& entity : entities) {
        if (entity.has<THandledEvent>()) {
            entity.remove();
        }
    }
}

}  // namespace NArtEngine
