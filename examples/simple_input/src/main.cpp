#include <iostream>

#include "ecs_engine.hpp"
#include "event_component.hpp"
#include "game_engine.hpp"
#include "input_event.hpp"
#include "input_event_system.hpp"

class TPrintInputEventSystem : public NArtEngine::TInputEventSystem {
  protected:
    void do_run(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntitiesView& entities
    ) override {
        for (const auto& entity : entities) {
            if (!entity.has_component<NArtEngine::TEventComponent>()) {
                continue;
            }

            if (entity
                    .has_component<NArtEngine::TInputKeyboardKeyEventComponent>(
                    )) {
                auto& component = entity.get_component<
                    NArtEngine::TInputKeyboardKeyEventComponent>();
                std::cout << "Keyboard key event: {action = "
                          << static_cast<size_t>(component.key_action)
                          << ", key = " << static_cast<size_t>(component.key)
                          << "}" << std::endl;
            }
            mark_event_handled(entity);
        }
    }
};

class TGame : public NArtEngine::IGame {
  public:
    TGame(NArtEngine::TGameEngine* game_engine)
        : game_engine_(game_engine) {
    }

    void init() override {
        auto& ecs_engine = game_engine_->get_ecs_engine();

        ecs_engine.add_system(std::make_unique<TPrintInputEventSystem>());
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
