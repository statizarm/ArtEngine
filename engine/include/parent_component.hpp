#pragma once

#include "entity.hpp"

namespace NArtEngine {

struct TParentComponent : public TComponent<TParentComponent> {
    TEntity parent;
};

}  // namespace NArtEngine
