#pragma once

#include "position_component.hpp"
#include "system.hpp"

namespace NArtEngine {

class TRenderingSystem : public TSystem {
  public:
    ~TRenderingSystem() = default;

  protected:
    void do_run(const TRenderingContext&, const TEntitiesView&) override;
};

}  // namespace NArtEngine
