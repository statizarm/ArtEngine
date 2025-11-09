#pragma once

#include "system.hpp"
namespace NArtEngine {

class TMovementSystem : public TSystem {
  protected:
    void do_run(const TRenderingContext& context, const TEntitiesView& entities)
        override;
};

}  // namespace NArtEngine
