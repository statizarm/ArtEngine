#pragma once

#include <bitset>
#include <memory>
#include <unordered_map>
#include <vector>

#include "component.hpp"
#include "entity_id.hpp"
#include "rendering_context.hpp"

namespace NArtEngine {

class TTypeErasedSystem;
class TEntity;

class TECSEngine {
    friend TEntity;

  public:
    static constexpr size_t kMaxEntities   = 1 << 20;
    static constexpr size_t kMaxComponents = 256;

  public:
    TECSEngine();
    ~TECSEngine();

  public:
    TEntityID add_entity();
    void remove_entity(TEntityID);
    TEntity get_entity(TEntityID);

    template <typename TSystem>
    void add_system(TSystem&&);

    void update(const TRenderingContext& context);

  private:
    template <CComponent TComponent>
    inline TComponent& get_entity_component(TEntityID entity_id);
    template <CComponent TComponent>
    inline bool has_entity_component(TEntityID entity_id);
    template <CComponent TComponent>
    TComponent& add_entity_component(TEntityID entity_id);
    template <CComponent TComponent>
    inline void remove_entity_component(TEntityID entity_id);

  private:
    void* get_entity_component(TEntityID, TComponentTypeID);
    bool has_entity_component(TEntityID, TComponentTypeID);
    void* add_entity_component(TEntityID, TComponentMeta);
    void remove_entity_component(TEntityID, TComponentTypeID);

  private:
    struct TComponentStorage {
        TComponentMeta meta;
        std::unique_ptr<uint8_t> memory;
    };

  private:
    std::vector<std::bitset<kMaxComponents>> components_mask_;
    std::unordered_map<TComponentTypeID, TComponentStorage> components_;
    std::vector<TTypeErasedSystem> systems_;
    TEntityID first_free_entity_ = 0;
};

}  // namespace NArtEngine

#include "ecs_engine-inl.hpp"
