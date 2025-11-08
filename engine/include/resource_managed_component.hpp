#pragma once

#include "component.hpp"
#include "component_type_id.hpp"
#include "reosource_id.hpp"
#include "resource_manager.hpp"

namespace NArtEngine {

struct TResourceManagedComponent
    : public TComponent<TResourceManagedComponent> {
    std::unordered_map<TComponentTypeID, TResourceID> component_resources;
    TResourceManager* resource_manager;
};

}  // namespace NArtEngine
