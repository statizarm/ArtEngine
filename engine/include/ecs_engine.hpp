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
    inline TComponent& get_entity_component(TEntityID entity_id) {
        return *static_cast<TComponent*>(
            get_entity_component(entity_id, TComponent::get_component_type_id())
        );
    }
    template <CComponent TComponent>
    inline bool has_entity_component(TEntityID entity_id) {
        return has_entity_component(
            entity_id, TComponent::get_component_type_id()
        );
    }
    template <CComponent TComponent>
    TComponent& add_entity_component(TEntityID entity_id) {
        auto component_type_id = TComponent::get_component_type_id();
        auto component_meta    = get_component_meta<TComponent>();
        return *static_cast<TComponent*>(
            add_entity_component(entity_id, component_type_id, component_meta)
        );
    }
    template <CComponent TComponent>
    inline void remove_entity_component(TEntityID entity_id) {
        remove_entity_component(entity_id, TComponent::get_component_type_id());
    }

    void* get_entity_component(TEntityID, TComponentTypeID);
    bool has_entity_component(TEntityID, TComponentTypeID);
    void remove_entity_component(TEntityID, TComponentTypeID);

    TEntityID add_entity();
    void remove_entity(TEntityID);

    void add_system(std::unique_ptr<TSystem> system);

    void update(const TRenderingContext& context);

  private:
    using TConstructComponent = void (*)(void*);
    using TDestructComponent  = void (*)(void*);

  private:
    struct TComponentMeta {
        size_t component_size;
        TConstructComponent constructor;
        TDestructComponent destructor;
    };
    struct TComponentStorage {
        TComponentMeta meta;
        std::unique_ptr<uint8_t> memory;
    };

  private:
    void* add_entity_component(TEntityID, TComponentTypeID, TComponentMeta);

    template <CComponent TComponent>
    TComponentMeta get_component_meta() {
        return TComponentMeta{
            .component_size = sizeof(TComponent),
            .constructor =
                [](void* memory) {
                    *static_cast<TComponent*>(memory) = TComponent();
                },
            .destructor =
                [](void* memory) {
                    static_cast<TComponent*>(memory)->~TComponent();
                },
        };
    }

  private:
    std::vector<std::bitset<kMaxComponents>> components_mask_;
    std::unordered_map<TComponentTypeID, TComponentStorage> components_;
    std::vector<std::unique_ptr<TSystem>> systems_;
};

}  // namespace NArtEngine
