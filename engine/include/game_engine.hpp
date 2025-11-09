#pragma once

#include <memory>

#include "ecs_engine.hpp"
#include "game.hpp"
#include "game_engine_config.hpp"
#include "resource_manager.hpp"
#include "window.hpp"

namespace NArtEngine {

class TGameEngineImpl;

class TGameEngine {
  public:
    TGameEngine();
    ~TGameEngine();

    void init();
    void init(const TGameEngineConfig& config);
    void deinit();
    void run(IGame* game);

    TECSEngine& get_ecs_engine();
    TResourceManager& get_resource_manager();
    const TWindow& get_window();

  private:
    std::unique_ptr<TGameEngineImpl> impl_;
};

}  // namespace NArtEngine
