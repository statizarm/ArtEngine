#pragma once

#include "entities_view.hpp"
#include "rendering_context.hpp"

namespace NArtEngine {

enum ESystemMode {
    ENABLED,
    DISABLED,
};

template <typename T>
concept CSystem =
    requires(T a) { a.run(TRenderingContext{}, TEntitiesView{}); };

class TSystem {
  public:
    virtual ~TSystem() = default;

  public:
    void run(const TRenderingContext& context, TEntitiesView& view) {
        if (mode_ == ESystemMode::DISABLED) {
            return;
        }
        do_run(context, view);
    }

    void set_mode(ESystemMode mode) {
        mode_ = mode;
    }

  protected:
    virtual void do_run(
        const TRenderingContext& context, const TEntitiesView& view
    ) = 0;

  private:
    ESystemMode mode_ = ESystemMode::ENABLED;
};

}  // namespace NArtEngine
