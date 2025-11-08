#include "ecs_engine.hpp"
#include "game_engine.hpp"
#include "mesh.hpp"
#include "resource_manager.hpp"
#include "shader.hpp"

class TGame : public NArtEngine::IGame {
  public:
    TGame(NArtEngine::TGameEngine* game_engine)
        : game_engine_(game_engine), resource_manager_() {
        NArtEngine::TResourceManagerConfig resource_manager_config{
            .resources_directory =
                std::filesystem::path(__FILE__).parent_path().parent_path()
        };
        resource_manager_ =
            NArtEngine::TResourceManager(resource_manager_config);
    }

    void init() override {
        auto& ecs_engine = game_engine_->get_ecs_engine();
        triangle_entity_ = ecs_engine.add_entity();
        auto& shader_component =
            ecs_engine
                .add_entity_component<NArtEngine::TShaderProgramComponent>(
                    triangle_entity_
                );
        auto& mesh_component =
            ecs_engine.add_entity_component<NArtEngine::TMeshComponent>(
                triangle_entity_
            );

        auto res = resource_manager_.load(
            "resources/triangle_mesh.txt", mesh_component
        );
        res = resource_manager_.load(
            "resources/triangle_shader.glsl", shader_component
        );
    }
    void update(const NArtEngine::TRenderingContext& context) override {
    }
    void deinit() override {
    }

  private:
    NArtEngine::TGameEngine* game_engine_;
    NArtEngine::TEntityID triangle_entity_;
    NArtEngine::TResourceManager resource_manager_;
};

int main() {
    NArtEngine::TGameEngine engine;
    TGame game(&engine);

    engine.init();
    engine.run(&game);
    engine.deinit();

    return 0;
}
