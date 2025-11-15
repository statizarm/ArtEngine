#pragma once

#include "ecs_engine.hpp"
#include "system.hpp"

namespace NArtEngine {

template <CComponent TComponent>
inline TComponent& TECSEngine::get_entity_component(TEntityID entity_id) {
    return *static_cast<TComponent*>(
        get_entity_component(entity_id, TComponent::get_type_id())
    );
}

template <CComponent TComponent>
inline bool TECSEngine::has_entity_component(TEntityID entity_id) {
    return has_entity_component(entity_id, TComponent::get_type_id());
}

template <CComponent TComponent>
TComponent& TECSEngine::add_entity_component(TEntityID entity_id) {
    auto component_type_id = TComponent::get_type_id();
    auto component_meta    = TComponentMeta::get<TComponent>();
    return *static_cast<TComponent*>(
        add_entity_component(entity_id, component_meta)
    );
}

template <CComponent TComponent>
inline void TECSEngine::remove_entity_component(TEntityID entity_id) {
    remove_entity_component(entity_id, TComponent::get_type_id());
}

template <typename TSystem>
void TECSEngine::add_system(TSystem&& system) {
    systems_.emplace_back(TTypeErasedSystem::create(std::forward<TSystem>(system
    )));
}

}  // namespace NArtEngine
