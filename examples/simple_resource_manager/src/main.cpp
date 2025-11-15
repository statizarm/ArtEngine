#include "ecs_engine.hpp"
#include "game_engine.hpp"
#include "mesh.hpp"
#include "resource_manager.hpp"
#include "shader.hpp"

class TGame : public NArtEngine::IGame {
  public:
    TGame(NArtEngine::TGameEngine* game_engine)
        : game_engine_(game_engine) {
    }

    void init() override {
        auto& ecs_engine       = game_engine_->get_ecs_engine();
        auto& resource_manager = game_engine_->get_resource_manager();

        entity_id_  = ecs_engine.add_entity();
        auto entity = ecs_engine.get_entity(entity_id_);

        auto& shader = entity.add<NArtEngine::TShaderProgram>();
        auto& mesh   = entity.add<NArtEngine::TMesh>();

        auto res = resource_manager.load("resources/triangle_mesh.txt", mesh);
        res = resource_manager.load("resources/triangle_shader.glsl", shader);
    }

    void update(const NArtEngine::TRenderingContext& context) override {
    }

    void deinit() override {
    }

  private:
    NArtEngine::TGameEngine* game_engine_;
    NArtEngine::TEntityID entity_id_;
};

int main() {
    NArtEngine::TGameEngine engine;
    TGame game(&engine);

    engine.init(NArtEngine::TGameEngineConfig{
        .resource_manager_config =
            NArtEngine::TResourceManagerConfig{
                .resources_directory =
                    std::filesystem::path(__FILE__).parent_path().parent_path(),
            },
    });
    engine.run(&game);
    engine.deinit();

    return 0;
}
