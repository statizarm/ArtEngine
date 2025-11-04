#pragma once

#include <bitset>
#include <limits>
#include <memory>
#include <unordered_map>
#include <vector>

#include "component.hpp"
#include "entity_id.hpp"
#include "rendering_context.hpp"

namespace NArtEngine {

class TSystem;

class TECSEngine {
  public:
    static constexpr size_t kMaxEntities = 1 << 16;
    static constexpr size_t kMaxComponents =
        std::numeric_limits<TComponentTypeID>::max();

  public:
    TECSEngine();
    ~TECSEngine();

  public:
    template <CComponent TComponent>
    TComponent& get_entity_component(TEntityID entity_id) {
        auto& ptr = components_[TComponent::get_component_type_id()];
        return reinterpret_cast<TComponent*>(ptr.get())[entity_id];
    }

    template <CComponent TComponent>
    bool has_entity_component(TEntityID entity_id) {
        return components_mask_[entity_id].test(
            TComponent::get_component_type_id()
        );
    }

    template <CComponent TComponent>
    TComponent& add_entity_component(TEntityID entity_id) {
        auto component_type_id = TComponent::get_component_type_id();
        if (!components_.contains(component_type_id)) {
            components_[component_type_id] = std::unique_ptr<uint8_t>(
                reinterpret_cast<uint8_t*>(new TComponent[kMaxEntities])
            );
        }
        components_mask_[entity_id].set(component_type_id);
        return get_entity_component<TComponent>(entity_id);
    }

    template <CComponent TComponent>
    void remove_entity_component(TEntityID entity_id) {
        components_mask_[entity_id][TComponent::get_component_type_id()] = 0;
    }

    TEntityID add_entity();
    void remove_entity(TEntityID);

    void add_system(std::unique_ptr<TSystem> system);

    void update(const TRenderingContext& context);

  private:
    std::vector<std::bitset<kMaxComponents>> components_mask_;
    std::unordered_map<TComponentTypeID, std::unique_ptr<uint8_t>> components_;
    std::vector<std::unique_ptr<TSystem>> systems_;
};

}  // namespace NArtEngine
