#pragma once

#include "component_type_id.hpp"

namespace NArtEngine {

template <typename T>
concept CComponent = requires(T a) { T::get_component_type_id(); };

template <typename T>
class TComponent {
  private:
    using TDerived = T;

  public:
    static TComponentTypeID get_component_type_id() {
        return TDerived::get_component_type_id();
    }
};

}  // namespace NArtEngine
