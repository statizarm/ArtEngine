#include <iostream>

#include "ecs_engine.hpp"
#include "game_engine.hpp"
#include "system.hpp"

class TPrintFrameTimeSystem : public NArtEngine::TSystem {
  protected:
    void do_run(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntitiesView& entities
    ) override {
        std::cout << "frame_time: " << context.dt << std::endl;
        std::cout << "fps: " << 1 / context.dt << std::endl;
    }
};

class TGame : public NArtEngine::IGame {
  public:
    TGame(NArtEngine::TGameEngine* game_engine)
        : game_engine_(game_engine) {
    }

    void init() override {
        auto& ecs_engine = game_engine_->get_ecs_engine();

        ecs_engine.add_system(std::make_unique<TPrintFrameTimeSystem>());
    }
    void update(const NArtEngine::TRenderingContext& context) override {
    }
    void deinit() override {
    }

  private:
    NArtEngine::TGameEngine* game_engine_;
    NArtEngine::TEntityID triangle_entity_;
};

int main() {
    NArtEngine::TGameEngine engine;
    TGame game(&engine);

    engine.init();
    engine.run(&game);
    engine.deinit();

    return 0;
}
