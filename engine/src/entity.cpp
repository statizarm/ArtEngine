#include "entity.hpp"

namespace NArtEngine {

TEntity::TEntity(TEntityID entity_id, TECSEngine* engine)
    : entity_id_(entity_id), engine_(engine) {
}

void* TEntity::get_component(TComponentTypeID component_type_id) const {
    return engine_->get_entity_component(entity_id_, component_type_id);
}

bool TEntity::has_component(TComponentTypeID component_type_id) const {
    return engine_->has_entity_component(entity_id_, component_type_id);
}

void TEntity::remove_component(TComponentTypeID component_type_id) const {
    engine_->remove_entity_component(entity_id_, component_type_id);
}

void TEntity::remove() const {
    engine_->remove_entity(entity_id_);
}

}  // namespace NArtEngine
