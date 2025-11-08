#pragma once

#include "system.hpp"

namespace NArtEngine {

class TRenderingSystem : public TSystem {
  public:
    ~TRenderingSystem() = default;

  protected:
    void do_run(const TRenderingContext&, const TEntitiesView&) override;

  private:
    void draw_entity(const TEntity& entity) const;
};

}  // namespace NArtEngine
