#pragma once

#include "system.hpp"

namespace NArtEngine {

class TResourceManagerSystem : public TSystem {
  public:
    ~TResourceManagerSystem() = default;

  protected:
    void do_run(const TRenderingContext&, const TEntitiesView&) override;
};

}  // namespace NArtEngine
