#pragma once

#include "component.hpp"

namespace NArtEngine {

struct TLifetime : public TComponent<TLifetime> {
    int64_t frames = 0;
    double time    = 0.0f;
};

}  // namespace NArtEngine
