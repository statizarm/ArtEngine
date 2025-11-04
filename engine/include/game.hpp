#pragma once

#include "rendering_context.hpp"

namespace NArtEngine {

class IGame {
  public:
    virtual ~IGame() = default;

    virtual void update(const TRenderingContext& context) = 0;

    virtual void init();
    virtual void deinit();
};

}  // namespace NArtEngine
