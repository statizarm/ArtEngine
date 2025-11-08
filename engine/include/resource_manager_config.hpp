#pragma once

#include <filesystem>

namespace NArtEngine {

struct TResourceManagerConfig {
    std::filesystem::path resources_directory = "";
};

}  // namespace NArtEngine
