#pragma once

#include "window.hpp"

namespace NArtEngine {

struct TRenderingContext {
    double dt;
    double start_time;
    double current_time;

    TWindow* window;
};

}  // namespace NArtEngine
