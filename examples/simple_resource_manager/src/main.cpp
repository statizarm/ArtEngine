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
        entity_                = ecs_engine.add_entity();
        auto& shader_component =
            ecs_engine
                .add_entity_component<NArtEngine::TShaderProgramComponent>(
                    entity_
                );
        auto& mesh_component =
            ecs_engine.add_entity_component<NArtEngine::TMeshComponent>(entity_
            );

        auto res = resource_manager.load(
            "resources/triangle_mesh.txt", mesh_component
        );
        res = resource_manager.load(
            "resources/triangle_shader.glsl", shader_component
        );
    }

    void update(const NArtEngine::TRenderingContext& context) override {
    }

    void deinit() override {
    }

  private:
    NArtEngine::TGameEngine* game_engine_;
    NArtEngine::TEntityID entity_;
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
