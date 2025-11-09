#pragma once

#include "component.hpp"
#include "ecs_engine.hpp"
#include "entity_id.hpp"

namespace NArtEngine {

class TEntity {
  public:
    friend TECSEngine;

  public:
    TEntity() = delete;

  public:
    template <CComponent TComponent>
    inline TComponent& get_component() const {
        return engine_->get_entity_component<TComponent>(entity_id_);
    }
    template <CComponent TComponent>
    inline bool has_component() const {
        return engine_->has_entity_component<TComponent>(entity_id_);
    }
    template <CComponent TComponent>
    inline TComponent& add_component() const {
        return engine_->add_entity_component<TComponent>(entity_id_);
    }
    template <CComponent TComponent>
    inline void remove_component() const {
        engine_->remove_entity_component<TComponent>(entity_id_);
    }

    void* get_component(TComponentTypeID) const;
    bool has_component(TComponentTypeID) const;
    void remove_component(TComponentTypeID) const;

    void remove() const;

  private:
    TEntity(TEntityID entity_id, TECSEngine* engine);

  private:
    TEntityID entity_id_;
    TECSEngine* engine_;
};

}  // namespace NArtEngine
