#include "resource_manager.hpp"

#include <unordered_map>

namespace NArtEngine {

static const std::unordered_map<std::string, EResourceFormat> kFormatMap = {
    {".txt", EResourceFormat::TEXT},
    {".glsl", EResourceFormat::GLSL},
};

TResourceManager::TResourceManager()
    : TResourceManager(TResourceManagerConfig{}) {
}

TResourceManager::TResourceManager(const TResourceManagerConfig& config)
    : config_(config) {
}

EResourceFormat TResourceManager::get_format(std::filesystem::path filepath) {
    if (!filepath.has_extension()) {
        return EResourceFormat::UNKNOWN;
    }
    if (!kFormatMap.contains(filepath.extension())) {
        return EResourceFormat::UNKNOWN;
    }
    return kFormatMap.at(filepath.extension());
}

}  // namespace NArtEngine
