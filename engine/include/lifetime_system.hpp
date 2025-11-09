#pragma once

#include "system.hpp"

namespace NArtEngine {

class TLifetimeSystem : public TSystem {
  protected:
    void do_run(const TRenderingContext& context, const TEntitiesView& entities)
        override;
};

}  // namespace NArtEngine
