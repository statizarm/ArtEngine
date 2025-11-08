#pragma once

#include <concepts>
#include <cstddef>
#include <functional>

namespace NArtEngineUtils {

struct TIdentifier {
    size_t monotonic_id;
    const char* human_readable_id;

    template <std::integral T>
    explicit operator T() const {
        return static_cast<T>(monotonic_id);
    }

    bool operator==(const TIdentifier& other) const {
        return monotonic_id == other.monotonic_id;
    }
};

}  // namespace NArtEngineUtils

template <>
struct std::hash<NArtEngineUtils::TIdentifier> {
    size_t operator()(const NArtEngineUtils::TIdentifier& identifier) const {
        return std::hash<decltype(identifier.monotonic_id)>()(
            identifier.monotonic_id
        );
    }
};
