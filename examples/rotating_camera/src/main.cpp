#include "camera_component.hpp"
#include "ecs_engine.hpp"
#include "game_engine.hpp"
#include "mesh.hpp"
#include "position_component.hpp"
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

        auto entity = ecs_engine.add_entity();

        auto& shader_component =
            ecs_engine
                .add_entity_component<NArtEngine::TShaderProgramComponent>(
                    entity
                );

        auto& mesh_component =
            ecs_engine.add_entity_component<NArtEngine::TMeshComponent>(entity);

        auto res = resource_manager.load("resources/mesh.txt", mesh_component);
        res = resource_manager.load("resources/shader.glsl", shader_component);

        camera_ = ecs_engine.add_entity();
        ecs_engine.add_entity_component<NArtEngine::TCameraComponent>(camera_);
        auto& camera_position =
            ecs_engine.add_entity_component<NArtEngine::TPositionComponent>(
                camera_
            );

        camera_position.position.z = 5.0f;
    }

    void update(const NArtEngine::TRenderingContext& context) override {
        auto& ecs_engine = game_engine_->get_ecs_engine();
        auto& camera_position =
            ecs_engine.get_entity_component<NArtEngine::TPositionComponent>(
                camera_
            );
        float angle                = glm::radians(context.current_time) * 100.f;
        camera_position.position.x = 5.0 * glm::sin(angle);
        camera_position.position.z = 5.0 * glm::cos(angle);
        camera_position.rotation   = glm::rotate(
            glm::identity<glm::quat>(), -angle, glm::vec3{0.f, 1.f, 0.f}
        );
    }

    void deinit() override {
    }

  private:
    NArtEngine::TGameEngine* game_engine_;
    NArtEngine::TEntityID camera_;
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
