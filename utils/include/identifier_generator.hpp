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
inline TIdentifier get_identifier(const std::string& id) {
    static TIdentifierGenerator generator;
    return generator.get_identifier(id);
}

TIdentifier get_identifier(const std::string& id);

}  // namespace NArtEngineUtils
