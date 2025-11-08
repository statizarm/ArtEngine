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

TResourceLoadResult TResourceManager::load(
    const TResourceID& resource_id, void* resource
) {
    // FIXME: resource leak
    if (!registered_resources_.contains(resource_id)) {
        return TResourceLoadResult{
            .resource_id = resource_id,
            .status      = EResourceLoadStatus::UNKNOWN_RESOURCE_ID,
        };
    }

    const auto& resource_conrol_block = registered_resources_[resource_id];
    return TResourceLoadResult{
        .resource_id = resource_id,
        .status      = resource_conrol_block.load(
            resource_conrol_block.filepath, resource
        ),
    };
}

}  // namespace NArtEngine
