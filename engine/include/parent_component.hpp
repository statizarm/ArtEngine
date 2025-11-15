#pragma once

#include "entity.hpp"

namespace NArtEngine {

struct TParent : public TComponent<TParent> {
    TEntity parent;
};

}  // namespace NArtEngine
