#pragma once

#include "resource_manager_config.hpp"

namespace NArtEngine {

struct TGameEngineConfig {
    struct TResourceManagerConfig resource_manager_config;
    float max_fps = 500.0;
};

}  // namespace NArtEngine
