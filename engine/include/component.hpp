#pragma once

#include "component_type_id.hpp"
#include "identifier_generator.hpp"

namespace NArtEngine {

template <typename T>
concept CComponent = requires(T a) { T::get_type_id(); };

template <typename T>
class TComponent {
  private:
    using TDerived = T;

  public:
    inline static TComponentTypeID get_type_id() {
        return NArtEngineUtils::get_identifier<T, "TComponent">();
    }
};

struct TComponentMeta {
  private:
    using TConstructor = void (*)(void*);
    using TDestructor  = void (*)(void*);

  public:
    TComponentTypeID type_id;
    size_t component_size;
    TConstructor constructor;
    TDestructor destructor;

  public:
    template <CComponent T>
    static inline TComponentMeta get() {
        TComponentMeta res;
        res.type_id        = T::get_type_id();
        res.component_size = sizeof(T);
        res.constructor    = [](void* memory) { new (memory) T(); };
        res.destructor = [](void* memory) { static_cast<T*>(memory)->~T(); };
        return res;
    }
};

}  // namespace NArtEngine
