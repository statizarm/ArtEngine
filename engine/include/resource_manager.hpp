#pragma once

#include <filesystem>
#include <unordered_map>

#include "reosource_id.hpp"
#include "resource_manager_config.hpp"

namespace NArtEngine {

enum EResourceLoadStatus {
    OK,
    FILE_DONT_EXISTS,
    UNSUPPORTED_FORMAT,
    UNSUPPORTED_FILE_TYPE,
    UNKNOWN_FORMAT,
    CANNOT_OPEN_FILE,
    BROKEN_FILE_CONTENT,
    UNKNOWN_RESOURCE_ID,
};

enum EResourceFormat {
    UNKNOWN,
    TEXT,
    GLSL,
};

struct TResourceLoadResult {
    TResourceID resource_id;
    EResourceLoadStatus status;
};

template <typename TResource, EResourceFormat Fromat>
EResourceLoadStatus load_resource(std::istream&, TResource&);

class TResourceManager {
  public:
    TResourceManager();
    TResourceManager(const TResourceManagerConfig&);

  public:
    template <typename TResource>
    TResourceLoadResult load(
        std::filesystem::path filepath, TResource& resource
    );

    TResourceLoadResult load(const TResourceID&, void*);

  private:
    using TLoadFunction = EResourceLoadStatus (*)(std::filesystem::path, void*);
    using TCopyFunction = void (*)(void*, void*);
    struct TResourceControlBlock {
        TLoadFunction load;
        TCopyFunction copy;
        std::filesystem::path filepath;
        size_t count;
    };

  private:
    static EResourceFormat get_format(std::filesystem::path filepath);

    template <typename TResource>
    static EResourceLoadStatus load_resource_from_file(
        std::filesystem::path filepath, TResource& resource
    );

    template <typename TResource>
    TResourceID register_resource(std::filesystem::path filepath);

  private:
    TResourceManagerConfig config_;
    std::unordered_map<TResourceID, TResourceControlBlock>
        registered_resources_;
    std::unordered_map<std::filesystem::path, TResourceID>
        resource_id_by_filepath_;
};

}  // namespace NArtEngine

#include "resource_manager-inl.hpp"
