#pragma once

#include "entities_view.hpp"
#include "rendering_context.hpp"

namespace NArtEngine {

class TResourceManagerSystem {
  public:
    void run(const TRenderingContext&, const TEntitiesView<TEntity>&);
};

}  // namespace NArtEngine
