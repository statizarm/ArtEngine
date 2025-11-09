#pragma once

#include "component.hpp"

namespace NArtEngine {

struct TLifetimeComponent : public TComponent<TLifetimeComponent> {
    int64_t frames = 0;
    double time    = 0.0f;
};

}  // namespace NArtEngine
