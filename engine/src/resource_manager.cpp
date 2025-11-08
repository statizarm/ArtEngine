#include "resource_manager.hpp"

namespace NArtEngine {

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
    if (filepath.extension() == ".txt") {
        return EResourceFormat::TEXT;
    }
    return EResourceFormat::UNKNOWN;
}

}  // namespace NArtEngine
