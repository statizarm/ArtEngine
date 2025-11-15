#pragma once

#include "entities_view.hpp"
#include "entity.hpp"
#include "rendering_context.hpp"

namespace NArtEngine {

class TInputEventSystem {
  protected:
    void mark_event_handled(const TEntity& entity);
};

class TRemoveHandledEventSystem {
  public:
    void run(const TRenderingContext& context, const TEntitiesView& entities);
};

}  // namespace NArtEngine
