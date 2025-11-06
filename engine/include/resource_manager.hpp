#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

namespace NArtEngine {

enum EResourceLoadStatus {
    OK,
    FILE_DONT_EXISTS,
    UNSUPPORTED_FORMAT,
    UNSUPPORTED_FILE_TYPE,
    UNKNOWN_FORMAT,
    CANNOT_OPEN_FILE,
    BROKEN_FILE_CONTENT,
};

enum EResourceFormat {
    UNKNOWN,
    TEXT,
};

template <typename TResource, EResourceFormat Fromat>
EResourceLoadStatus load_resource(std::istream&, TResource&) {
    return EResourceLoadStatus::UNSUPPORTED_FORMAT;
}

class TResourceManager {
  public:
    template <typename TResource>
    EResourceLoadStatus load(
        std::filesystem::path filepath, TResource& resource
    ) {
        if (!std::filesystem::exists(filepath)) {
            return EResourceLoadStatus::FILE_DONT_EXISTS;
        } else if (!std::filesystem::is_regular_file(filepath)) {
            return EResourceLoadStatus::UNSUPPORTED_FILE_TYPE;
        }
        std::ifstream input_file{filepath};
        if (!input_file.is_open()) {
            return EResourceLoadStatus::CANNOT_OPEN_FILE;
        }

        auto format = get_format(filepath);
        switch (format) {
            case EResourceFormat::TEXT:
                return load_resource<TResource, EResourceFormat::TEXT>(
                    input_file, resource
                );
            default:
                return EResourceLoadStatus::UNKNOWN_FORMAT;
        }
    }

  private:
    EResourceFormat get_format(std::filesystem::path filepath);

  private:
};

}  // namespace NArtEngine
