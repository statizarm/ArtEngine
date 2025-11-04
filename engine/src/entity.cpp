#include "entity.hpp"

namespace NArtEngine {

TEntity::TEntity(TEntityID entity_id, TECSEngine* engine)
    : entity_id_(entity_id), engine_(engine) {
}

}  // namespace NArtEngine
