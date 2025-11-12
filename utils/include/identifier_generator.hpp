#pragma once

#include <map>
#include <string>
#include <unordered_set>

#include "identifier.hpp"
#include "string_literal.hpp"

namespace NArtEngineUtils {

class TIdentifierGenerator {
  public:
    TIdentifierGenerator();

  public:
    TIdentifier get_identifier(const std::string&);

  private:
    std::unordered_set<size_t> used_;
    std::map<std::string, TIdentifier> ids_;
};

template <string_literal s>
inline TIdentifier get_identifier() {
    static size_t id = 0;
    return TIdentifier{.monotonic_id = id++};
}

template <typename T, string_literal s>
TIdentifier get_identifier() {
    static TIdentifier real_id = get_identifier<s>();
    return real_id;
}

}  // namespace NArtEngineUtils
