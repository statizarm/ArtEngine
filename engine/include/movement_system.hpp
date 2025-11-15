#pragma once

#include "entities_view.hpp"
#include "rendering_context.hpp"

namespace NArtEngine {

class TMovementSystem {
  public:
    void run(const TRenderingContext& context, const TEntitiesView& entities);
};

}  // namespace NArtEngine
