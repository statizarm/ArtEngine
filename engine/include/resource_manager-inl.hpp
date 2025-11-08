#pragma once

#include <fstream>

#include "resource_manager.hpp"

namespace NArtEngine {
template <typename TResource>
TResourceLoadResult TResourceManager::load(
    std::filesystem::path filepath, TResource& resource
) {
    // FIXME: resource leak
    if (!filepath.is_absolute()) {
        filepath = config_.resources_directory / filepath;
    }
    if (!std::filesystem::exists(filepath)) {
        return TResourceLoadResult{
            .status = EResourceLoadStatus::FILE_DONT_EXISTS,
        };
    } else if (!std::filesystem::is_regular_file(filepath)) {
        return TResourceLoadResult{
            .status = EResourceLoadStatus::UNSUPPORTED_FILE_TYPE,
        };
    }
    auto resource_id = register_resource<TResource>(filepath);
    return load(resource_id, resource);
}

template <typename TResource>
TResourceLoadResult TResourceManager::load(
    const TResourceID& resource_id, TResource& resource
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
            resource_conrol_block.filepath, static_cast<void*>(&resource)
        ),
    };
}

template <typename TResource>
EResourceLoadStatus TResourceManager::load_resource_from_file(
    std::filesystem::path filepath, TResource& resource
) {
    std::ifstream input_file{filepath};
    if (!input_file.is_open()) {
        return EResourceLoadStatus::CANNOT_OPEN_FILE;
    }

    auto format = get_format(filepath);
    EResourceLoadStatus load_status;
    switch (format) {
        case EResourceFormat::TEXT:
            load_status = load_resource<TResource, EResourceFormat::TEXT>(
                input_file, resource
            );
        case EResourceFormat::GLSL:
            load_status = load_resource<TResource, EResourceFormat::GLSL>(
                input_file, resource
            );
        default:
            load_status = EResourceLoadStatus::UNKNOWN_FORMAT;
    }
    return load_status;
}

template <typename TResource>
TResourceID TResourceManager::register_resource(std::filesystem::path filepath
) {
    // FIXME: resource leak
    auto size = resource_id_by_filepath_.size();
    if (!resource_id_by_filepath_.contains(filepath)) {
        resource_id_by_filepath_[filepath] = TResourceID{.id = size};
    }
    auto& resource_id = resource_id_by_filepath_[filepath];

    if (!registered_resources_.contains(resource_id)) {
        TResourceControlBlock control_block{
            .load =
                [](std::filesystem::path filepath, void* resource) {
                    return load_resource_from_file<TResource>(
                        filepath, *static_cast<TResource*>(resource)
                    );
                },
            .copy =
                [](void* src, void* dst) {
                    *(static_cast<TResource*>(dst)) =
                        *(static_cast<TResource*>(src));
                },
            .filepath = filepath,
            .count    = 1,
        };
        registered_resources_[resource_id] = std::move(control_block);
    }

    return resource_id;
}

template <typename TResource, EResourceFormat Fromat>
EResourceLoadStatus load_resource(std::istream&, TResource&) {
    return UNSUPPORTED_FORMAT;
}

}  // namespace NArtEngine
