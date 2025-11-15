#pragma once

#include "component.hpp"
#include "ecs_engine.hpp"
#include "entity_id.hpp"

namespace NArtEngine {

class TEntity {
  public:
    friend TECSEngine;

    TEntity() = default;

  public:
    template <CComponent T>
    inline T& get() const {
        return engine_->get_entity_component<T>(entity_id_);
    }
    template <CComponent T>
    inline bool has() const {
        return engine_->has_entity_component<T>(entity_id_);
    }
    template <CComponent T>
    inline T& add() const {
        return engine_->add_entity_component<T>(entity_id_);
    }
    template <CComponent T>
    inline void remove() const {
        engine_->remove_entity_component<T>(entity_id_);
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
