#pragma once

#include "system.hpp"

namespace NArtEngine {

class TLifetimeSystem {
  public:
    void run(const TRenderingContext& context, const TEntitiesView& entities);
};

}  // namespace NArtEngine
