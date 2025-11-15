#pragma once

#include "entities_view.hpp"
#include "rendering_context.hpp"

namespace NArtEngine {

class TRenderingSystem {
  public:
    void run(const TRenderingContext&, const TEntitiesView&);
};

}  // namespace NArtEngine
