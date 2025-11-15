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

}  // namespace NArtEngine
