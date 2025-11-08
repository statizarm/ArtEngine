#pragma once

#include <array>

namespace NArtEngineUtils {

template <std::size_t N>
struct string_literal {
    std::array<char, N> data;

    constexpr string_literal(const char (&s)[N]) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] = s[i];
        }
    }
};

}  // namespace NArtEngineUtils
