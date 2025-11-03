#pragma once

#include <memory>

#include "game.hpp"

namespace NArtEngine {

class TGameEngineImpl;

class TGameEngine {
  public:
    TGameEngine();
    ~TGameEngine();

    void init();
    void deinit();
    void run(IGame* game);

  private:
    std::unique_ptr<TGameEngineImpl> impl_;
};

}  // namespace NArtEngine
