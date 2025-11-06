#include "resource_manager.hpp"

namespace NArtEngine {

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
