#include "engine.hpp"

class TGame : public NArtEngine::IGame {
  public:
    void update(float) override {
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
