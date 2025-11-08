#include "game_engine.hpp"

class TGame : public NArtEngine::IGame {
  public:
    void update(const NArtEngine::TRenderingContext&) override {
    }
};

int main() {
    NArtEngine::TGameEngine engine;
    TGame game;

    engine.init();
    engine.run(&game);
    engine.deinit();

    return 0;
}
