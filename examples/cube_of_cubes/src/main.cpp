#include <iostream>

#include "camera_component.hpp"
#include "ecs_engine.hpp"
#include "event_component.hpp"
#include "game_engine.hpp"
#include "input_event.hpp"
#include "input_event_system.hpp"
#include "mesh.hpp"
#include "movement_component.hpp"
#include "parent_component.hpp"
#include "position_component.hpp"
#include "resource_manager.hpp"
#include "shader.hpp"

class TPrintFrameTimeSystem {
  public:
    void run(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntitiesView<NArtEngine::TEntity>& entities
    ) {
        std::cout << "frame_time: " << context.dt << std::endl;
        std::cout << "fps: " << 1 / context.dt << std::endl;
    }
};

class TControlCameraSystem : public NArtEngine::TInputEventSystem {
  public:
    void run(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntitiesView<NArtEngine::TEntity>& entities
    ) {
        const NArtEngine::TEntity* camera = nullptr;

        for (const auto& entity : entities) {
            if (entity.has<NArtEngine::TCamera>()) {
                camera = &entity;
                break;
            }
        }
        if (!camera) {
            return;
        }

        for (const auto& entity : entities) {
            if (entity.has<NArtEngine::TEvent>()) {
                handle_event(context, *camera, entity);
                mark_event_handled(entity);
            }
        }
    }

  private:
    void handle_event(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntity& camera, const NArtEngine::TEntity& event
    ) {
        if (event.has<NArtEngine::TMouseMovedEvent>()) {
            handle_mouse_moved_event(
                context, camera, event.get<NArtEngine::TMouseMovedEvent>()
            );
        }
        if (event.has<NArtEngine::TKeyEvent>()) {
            handle_keyboard_event(
                context, camera, event.get<NArtEngine::TKeyEvent>()
            );
        }
    }

    void handle_mouse_moved_event(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntity& camera,
        const NArtEngine::TMouseMovedEvent& event
    ) {
        if (!camera.has<NArtEngine::TPosition>()) {
            return;
        }
        float dx              = event.curr_xpos - event.prev_xpos;
        float dy              = event.curr_ypos - event.prev_ypos;
        float xangle          = glm::radians(dx) * 0.01;
        float yangle          = glm::radians(dy) * 0.01;
        auto& camera_position = camera.get<NArtEngine::TPosition>();
        // FIXME: rorate around z
        camera_position.rotation = glm::rotate(
            camera_position.rotation, yangle, glm::vec3{1.f, 0.f, 0.f}
        );
        camera_position.rotation = glm::rotate(
            camera_position.rotation, xangle, glm::vec3{0.f, 1.f, 0.f}
        );
    }

    void handle_keyboard_event(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntity& camera, const NArtEngine::TKeyEvent& event
    ) {
        // FIXME:: there is bullshit
        if (event.key_action == NArtEngine::EKeyAction::RELEASED) {
            switch (event.key) {
                case NArtEngine::EKey::KEY_W:
                case NArtEngine::EKey::KEY_S:
                case NArtEngine::EKey::KEY_D:
                case NArtEngine::EKey::KEY_A:
                    camera.remove<NArtEngine::TMovement>();
                    break;
                default:
                    break;
            }
            return;
        }

        glm::vec3 direction_delta;
        switch (event.key) {
            case NArtEngine::EKey::KEY_W:
                direction_delta = glm::vec3{0.f, 0.f, -1.f};
                break;
            case NArtEngine::EKey::KEY_S:
                direction_delta = glm::vec3{0.f, 0.f, 1.f};
                break;
            case NArtEngine::EKey::KEY_D:
                direction_delta = glm::vec3{1.f, 0.f, 0.f};
                break;
            case NArtEngine::EKey::KEY_A:
                direction_delta = glm::vec3{-1.f, 0.f, 0.f};
                break;
            default:
                return;
        }
        auto& movement    = camera.add<NArtEngine::TMovement>();
        movement.velocity = 5.f;
        movement.direction += direction_delta;
    }
};

class TRotationSystem {
  public:
    void run(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntitiesView<NArtEngine::TEntity>& entities
    ) {
        for (const auto& entity : entities) {
            if (entity.has<NArtEngine::TPosition>() &&
                !entity.has<NArtEngine::TCamera>()) {
                auto& position = entity.get<NArtEngine::TPosition>();
                float angle    = glm::radians(context.current_time) * 20.f;

                position.rotation = glm::rotate(
                    glm::identity<glm::quat>(), angle, glm::vec3{0.f, 1.f, 0.f}
                );
            }
        }
    }
};

class TGame : public NArtEngine::IGame {
  public:
    TGame(NArtEngine::TGameEngine* game_engine)
        : game_engine_(game_engine) {
    }

    void init() override {
        auto& ecs_engine       = game_engine_->get_ecs_engine();
        auto& resource_manager = game_engine_->get_resource_manager();
        const auto& window     = game_engine_->get_window();
        window.grab_cursor();

        NArtEngine::TShaderProgram shader;
        NArtEngine::TMesh mesh;
        auto res = resource_manager.load("resources/mesh.txt", mesh);
        res      = resource_manager.load("resources/shader.glsl", shader);

        auto cube_id = ecs_engine.add_entity();
        auto cube    = ecs_engine.get_entity(cube_id);

        cube.add<NArtEngine::TShaderProgram>() = shader;
        cube.add<NArtEngine::TMesh>()          = mesh;

        camera_id_  = ecs_engine.add_entity();
        auto camera = ecs_engine.get_entity(camera_id_);
        camera.add<NArtEngine::TCamera>();
        camera.add<NArtEngine::TPosition>().position.z = 5.0f;

        create_cube_of_cubes(shader, mesh, 8);
        ecs_engine.add_system(TControlCameraSystem());
        ecs_engine.add_system(TRotationSystem());
        ecs_engine.add_system(TPrintFrameTimeSystem());
    }

    void update(const NArtEngine::TRenderingContext& context) override {
    }

    void deinit() override {
    }

  private:
    NArtEngine::TEntity create_cube_of_cubes(
        const NArtEngine::TShaderProgram& shader, const NArtEngine::TMesh& mesh,
        int distance
    ) {
        static constexpr int kRowCubeCount = 3;
        auto& ecs_engine                   = game_engine_->get_ecs_engine();
        if (distance <= 1) {
            auto cube = ecs_engine.get_entity(ecs_engine.add_entity());
            cube.add<NArtEngine::TMesh>()          = mesh;
            cube.add<NArtEngine::TShaderProgram>() = shader;
            cube.add<NArtEngine::TPosition>();
            return cube;
        }

        auto cube_set_id = ecs_engine.add_entity();
        auto cube_set    = ecs_engine.get_entity(cube_set_id);
        for (int i = 0; i < kRowCubeCount; ++i) {
            for (int j = 0; j < kRowCubeCount; ++j) {
                for (int k = 0; k < kRowCubeCount; ++k) {
                    auto cube =
                        create_cube_of_cubes(shader, mesh, distance >> 1);
                    cube.add<NArtEngine::TParent>().parent     = cube_set;
                    cube.add<NArtEngine::TPosition>().position = glm::vec3{
                        static_cast<float>((i - kRowCubeCount / 2) * distance),
                        static_cast<float>((j - kRowCubeCount / 2) * distance),
                        static_cast<float>((k - kRowCubeCount / 2) * distance),
                    };
                }
            }
        }
        cube_set.add<NArtEngine::TPosition>();
        return cube_set;
    }

  private:
    NArtEngine::TGameEngine* game_engine_;
    NArtEngine::TEntityID camera_id_;
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
