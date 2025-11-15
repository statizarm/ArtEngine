#include "system.hpp"

namespace NArtEngine {

TTypeErasedSystem::TTypeErasedSystem(TTypeErasedSystem&& other) {
    *this = std::move(other);
}

TTypeErasedSystem::~TTypeErasedSystem() {
    if (memory_) {
        deleter_(memory_);
    }
}

TTypeErasedSystem& TTypeErasedSystem::operator=(TTypeErasedSystem&& other) {
    memory_        = other.memory_;
    deleter_       = other.deleter_;
    call_          = other.call_;
    other.memory_  = nullptr;
    other.deleter_ = nullptr;
    other.call_    = nullptr;
    return *this;
}

void TTypeErasedSystem::run(
    const TRenderingContext& context, const TEntitiesView<TEntity>& entities
) {
    call_(memory_, context, entities);
}

}  // namespace NArtEngine
