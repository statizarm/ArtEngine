#pragma once

#include "system.hpp"

namespace NArtEngine {

template <CSystem TSystem>
TTypeErasedSystem TTypeErasedSystem::create(TSystem&& system) {
    TTypeErasedSystem res;
    res.memory_  = new TSystem(std::forward<TSystem>(system));
    res.deleter_ = [](void* memory) { delete static_cast<TSystem*>(memory); };
    res.call_    = [](void* memory,
                   const TRenderingContext& context,
                   const TEntitiesView<TEntity>& entities) {
        static_cast<TSystem*>(memory)->run(context, entities);
    };
    return res;
}

}  // namespace NArtEngine
