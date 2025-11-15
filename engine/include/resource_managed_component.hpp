#pragma once

#include "component.hpp"
#include "component_type_id.hpp"
#include "reosource_id.hpp"
#include "resource_manager.hpp"

namespace NArtEngine {

struct TResourceManaged : public TComponent<TResourceManaged> {
    std::unordered_map<TComponentTypeID, TResourceID> component_resources;
    TResourceManager* resource_manager;
};

}  // namespace NArtEngine
