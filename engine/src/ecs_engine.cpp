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

void* TECSEngine::get_entity_component(
    TEntityID entity_id, TComponentTypeID component_type_id
) {
    const auto& storage = components_.at(component_type_id);
    return static_cast<void*>(
        storage.memory.get() + entity_id * storage.meta.component_size
    );
}

bool TECSEngine::has_entity_component(
    TEntityID entity_id, TComponentTypeID component_type_id
) {
    return components_mask_[entity_id].test(component_type_id);
}

void* TECSEngine::add_entity_component(
    TEntityID entity_id, TComponentTypeID component_type_id, TComponentMeta meta
) {
    if (!components_.contains(component_type_id)) {
        auto& storage = components_[component_type_id];

        storage.meta   = meta;
        storage.memory = std::unique_ptr<uint8_t>(
            new uint8_t[meta.component_size * kMaxEntities]
        );
    }
    components_mask_[entity_id].set(component_type_id);
    void* component_memory = get_entity_component(entity_id, component_type_id);
    meta.constructor(component_memory);
    return get_entity_component(entity_id, component_type_id);
}

void TECSEngine::remove_entity_component(
    TEntityID entity_id, TComponentTypeID component_type_id
) {
    const auto& meta = components_[component_type_id].meta;
    meta.destructor(get_entity_component(entity_id, component_type_id));
    components_mask_[component_type_id][entity_id] = 0;
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
