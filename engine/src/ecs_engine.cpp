#include "ecs_engine.hpp"

#include "system.hpp"

namespace NArtEngine {

class TRemovedEntityComponent : public TComponent<TRemovedEntityComponent> {
  public:
    static TComponentTypeID get_component_type_id() {
        return TComponentTypeID{0};
    }
};

TECSEngine::TECSEngine()
    : components_mask_(kMaxEntities, std::bitset<kMaxComponents>(0)) {
    for (size_t i = 0; i < components_mask_.size(); ++i) {
        add_entity_component<TRemovedEntityComponent>(TEntityID{i});
    }
}

TECSEngine::~TECSEngine() {
}

TEntityID TECSEngine::add_entity() {
    for (size_t i = 0; i < components_mask_.size(); ++i) {
        if (has_entity_component<TRemovedEntityComponent>(TEntityID{i})) {
            remove_entity_component<TRemovedEntityComponent>(TEntityID{i});
            return i;
        }
    }
    return -1;
}

void TECSEngine::remove_entity(TEntityID entity_id) {
    components_mask_[entity_id].reset();
    add_entity_component<TRemovedEntityComponent>(entity_id);
}

void TECSEngine::add_system(std::unique_ptr<TSystem> system) {
    systems_.emplace_back(std::move(system));
}

void TECSEngine::update(const TRenderingContext& context) {
    TEntitiesView entities;
    for (int i = 0; i < components_mask_.size(); ++i) {
        if (!has_entity_component<TRemovedEntityComponent>(i)) {
            entities.emplace_back(TEntity(i, this));
        }
    }
    for (auto& system : systems_) {
        system->run(context, entities);
    }
}

}  // namespace NArtEngine
