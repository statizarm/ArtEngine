#include <iostream>

#include "ecs_engine.hpp"
#include "engine.hpp"
#include "system.hpp"

class TPrintEntitySystem : public NArtEngine::TSystem {
  protected:
    void do_run(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntitiesView& entities
    ) override {
        auto time_delta = context.current_time - context.start_time;
        for (const auto& entity : entities) {
            if (time_delta - static_cast<int>(time_delta) > 0.9)
                std::cout << &entity << std::endl;
        }
    }
};

class TGame : public NArtEngine::IGame {
  public:
    TGame()
        : engine_() {
    }
    void init() override {
        triangle_entity_ = engine_.add_entity();
        engine_.add_system(std::make_unique<TPrintEntitySystem>());
    }
    void update(const NArtEngine::TRenderingContext& context) override {
        engine_.update(context);
    }
    void deinit() override {
        engine_.remove_entity(triangle_entity_);
    }

  private:
    NArtEngine::TECSEngine engine_;
    NArtEngine::TEntityID triangle_entity_;
};

int main() {
    NArtEngine::TGameEngine engine;
    TGame game;

    engine.init();
    engine.run(&game);
    engine.deinit();

    return 0;
}
