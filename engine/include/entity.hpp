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
    TComponent& get_component() const {
        return engine_->get_entity_component<TComponent>(entity_id_);
    }

    template <CComponent TComponent>
    bool has_component() const {
        return engine_->has_entity_component<TComponent>(entity_id_);
    }

    template <CComponent TComponent>
    TComponent& add_component() const {
        return engine_->add_entity_component<TComponent>(entity_id_);
    }

  private:
    TEntity(TEntityID entity_id, TECSEngine* engine);

  private:
    TEntityID entity_id_;
    TECSEngine* engine_;
};

}  // namespace NArtEngine
