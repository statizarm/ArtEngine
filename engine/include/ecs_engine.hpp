#pragma once

#include <bitset>
#include <memory>
#include <unordered_map>
#include <vector>

#include "component.hpp"
#include "entity_id.hpp"
#include "rendering_context.hpp"

namespace NArtEngine {

class TSystem;
class TEntity;

class TECSEngine {
  public:
    static constexpr size_t kMaxEntities   = 1 << 20;
    static constexpr size_t kMaxComponents = 256;

  public:
    TECSEngine();
    ~TECSEngine();

  public:
    template <CComponent TComponent>
    inline TComponent& get_entity_component(TEntityID entity_id);
    template <CComponent TComponent>
    inline bool has_entity_component(TEntityID entity_id);
    template <CComponent TComponent>
    TComponent& add_entity_component(TEntityID entity_id);
    template <CComponent TComponent>
    inline void remove_entity_component(TEntityID entity_id);

    void* get_entity_component(TEntityID, TComponentTypeID);
    bool has_entity_component(TEntityID, TComponentTypeID);
    void remove_entity_component(TEntityID, TComponentTypeID);

    TEntityID add_entity();
    void remove_entity(TEntityID);
    TEntity get_entity(TEntityID);

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
    TComponentMeta get_component_meta();

  private:
    std::vector<std::bitset<kMaxComponents>> components_mask_;
    std::unordered_map<TComponentTypeID, TComponentStorage> components_;
    std::vector<std::unique_ptr<TSystem>> systems_;
    TEntityID first_free_entity_ = 0;
};

}  // namespace NArtEngine

#include "ecs_engine-inl.hpp"
