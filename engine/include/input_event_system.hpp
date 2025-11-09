#pragma once

#include "entities_view.hpp"
#include "rendering_context.hpp"
#include "system.hpp"

namespace NArtEngine {

class TInputEventSystem : public TSystem {
  protected:
    void mark_event_handled(const TEntity& entity);
};

class TRemoveHandledEventSystem : public TSystem {
  protected:
    void do_run(
        const TRenderingContext& context, const TEntitiesView& entities
    );
};

}  // namespace NArtEngine
