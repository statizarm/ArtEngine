#pragma once

#include <memory>

#include "ecs_engine.hpp"
#include "window.hpp"

namespace NArtEngine {

class TInputEngine {
  public:
    class TImpl;

  public:
    TInputEngine();
    ~TInputEngine();

    // register callbacks
    void init(TWindow* window, TECSEngine* event_dispatcher);
    // unregister callbacks
    void deinit();

  private:
    std::unique_ptr<TImpl> impl_;
};

}  // namespace NArtEngine
