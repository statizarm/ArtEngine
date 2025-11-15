#pragma once

#include "ecs_engine.hpp"

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
    auto component_meta    = get_component_meta<TComponent>();
    return *static_cast<TComponent*>(
        add_entity_component(entity_id, component_type_id, component_meta)
    );
}

template <CComponent TComponent>
inline void TECSEngine::remove_entity_component(TEntityID entity_id) {
    remove_entity_component(entity_id, TComponent::get_type_id());
}

template <CComponent TComponent>
TECSEngine::TComponentMeta TECSEngine::get_component_meta() {
    return TComponentMeta{
        .component_size = sizeof(TComponent),
        .constructor    = [](void* memory) { new (memory) TComponent(); },
        .destructor     = [](void* memory
                      ) { static_cast<TComponent*>(memory)->~TComponent(); },
    };
}

}  // namespace NArtEngine
