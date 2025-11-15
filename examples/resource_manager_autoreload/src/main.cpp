#include <cmath>

#include "ecs_engine.hpp"
#include "game_engine.hpp"
#include "mesh.hpp"
#include "resource_managed_component.hpp"
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

        auto& shader           = entity.add<NArtEngine::TShaderProgram>();
        auto& mesh             = entity.add<NArtEngine::TMesh>();
        auto& resource_managed = entity.add<NArtEngine::TResourceManaged>();

        resource_managed.resource_manager = &resource_manager;

        auto res = resource_manager.load("resources/mesh.txt", mesh);
        if (res.status == NArtEngine::EResourceLoadStatus::OK) {
            resource_managed.component_resources[mesh.get_type_id()] =
                res.resource_id;
        }
        res = resource_manager.load("resources/shader.glsl", shader);
        if (res.status == NArtEngine::EResourceLoadStatus::OK) {
            resource_managed.component_resources[shader.get_type_id()] =
                res.resource_id;
        }
    }

    void update(const NArtEngine::TRenderingContext& context) override {
        auto filepath =
            std::filesystem::path(__FILE__).parent_path().parent_path() /
            std::filesystem::path("resources/mesh.txt");

        std::ofstream out(filepath);
        auto y = std::sin(context.current_time);
        auto x = std::cos(context.current_time);

        out << x << " " << y << " " << 0.0 << std::endl;
        out << x << " " << -y << " " << 0.0 << std::endl;
        out << -x << " " << -y << " " << 0.0 << std::endl;
        out << x << " " << y << " " << 0.0 << std::endl;
        out << -x << " " << y << " " << 0.0 << std::endl;
        out << -x << " " << -y << " " << 0.0 << std::endl;
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
