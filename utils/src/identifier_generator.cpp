#include "identifier_generator.hpp"

namespace NArtEngineUtils {

TIdentifierGenerator::TIdentifierGenerator() {
}

TIdentifier TIdentifierGenerator::get_identifier(const std::string& id) {
    auto [it, created] = ids_.emplace(id, TIdentifier{});
    if (created) {
        it->second.human_readable_id = it->first.c_str();
        it->second.monotonic_id      = ids_.size() - 1;
    }

    return it->second;
}

TIdentifier get_identifier(const std::string& id) {
    static TIdentifierGenerator generator;
    return generator.get_identifier(id);
}

}  // namespace NArtEngineUtils
