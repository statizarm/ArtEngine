#pragma once

#include "component.hpp"
#include "entity_id.hpp"

namespace NArtEngine {

class TECSEngine;

class TEntity {
  public:
    friend TECSEngine;

    TEntity() = default;

  public:
    template <CComponent T>
    inline T& get() const {
        return *static_cast<T*>(get_component(T::get_type_id()));
    }
    template <CComponent T>
    inline bool has() const {
        return has_component(T::get_type_id());
    }
    template <CComponent T>
    inline T& add() const {
        return *static_cast<T*>(add_component(TComponentMeta::get<T>()));
    }
    template <CComponent T>
    inline void remove() const {
        remove_component(T::get_type_id());
    }

    void* get_component(TComponentTypeID) const;
    bool has_component(TComponentTypeID) const;
    void* add_component(TComponentMeta) const;
    void remove_component(TComponentTypeID) const;

    void remove() const;

  private:
    TEntity(TEntityID entity_id, TECSEngine* engine);

  private:
    TEntityID entity_id_;
    TECSEngine* engine_;
};

}  // namespace NArtEngine
