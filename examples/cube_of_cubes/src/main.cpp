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

class TControlCameraSystem : public NArtEngine::TInputEventSystem {
  protected:
    void do_run(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntitiesView& entities
    ) {
        const NArtEngine::TEntity* camera = nullptr;

        for (const auto& entity : entities) {
            if (entity.has_component<NArtEngine::TCameraComponent>()) {
                camera = &entity;
                break;
            }
        }
        if (!camera) {
            return;
        }

        for (const auto& entity : entities) {
            if (entity.has_component<NArtEngine::TEventComponent>()) {
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
        if (event.has_component<NArtEngine::TInputMouseMovedEventComponent>()) {
            handle_mouse_moved_event(
                context,
                camera,
                event.get_component<NArtEngine::TInputMouseMovedEventComponent>(
                )
            );
        }
        if (event.has_component<NArtEngine::TInputKeyboardKeyEventComponent>(
            )) {
            handle_keyboard_event(
                context,
                camera,
                event
                    .get_component<NArtEngine::TInputKeyboardKeyEventComponent>(
                    )
            );
        }
    }

    void handle_mouse_moved_event(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntity& camera,
        const NArtEngine::TInputMouseMovedEventComponent& event
    ) {
        if (!camera.has_component<NArtEngine::TPositionComponent>()) {
            return;
        }
        float dx     = event.curr_xpos - event.prev_xpos;
        float dy     = event.curr_ypos - event.prev_ypos;
        float xangle = glm::radians(dx) * 0.01;
        float yangle = glm::radians(dy) * 0.01;
        auto& camera_position =
            camera.get_component<NArtEngine::TPositionComponent>();
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
        const NArtEngine::TEntity& camera,
        const NArtEngine::TInputKeyboardKeyEventComponent& event
    ) {
        // FIXME:: there is bullshit
        if (event.key_action == NArtEngine::EKeyAction::RELEASED) {
            switch (event.key) {
                case NArtEngine::EKey::KEY_W:
                case NArtEngine::EKey::KEY_S:
                case NArtEngine::EKey::KEY_D:
                case NArtEngine::EKey::KEY_A:
                    camera.remove_component<NArtEngine::TMovementComponent>();
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
        auto& movement = camera.add_component<NArtEngine::TMovementComponent>();
        movement.velocity = 5.f;
        movement.direction += direction_delta;
    }
};

class TRotationSystem : public NArtEngine::TSystem {
  protected:
    void do_run(
        const NArtEngine::TRenderingContext& context,
        const NArtEngine::TEntitiesView& entities
    ) {
        for (const auto& entity : entities) {
            if (entity.has_component<NArtEngine::TPositionComponent>() &&
                !entity.has_component<NArtEngine::TCameraComponent>()) {
                auto& position =
                    entity.get_component<NArtEngine::TPositionComponent>();
                float angle = glm::radians(context.current_time) * 20.f;

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

        NArtEngine::TShaderProgramComponent shader;
        NArtEngine::TMeshComponent mesh;
        auto res = resource_manager.load("resources/mesh.txt", mesh);
        res      = resource_manager.load("resources/shader.glsl", shader);

        auto cube = ecs_engine.add_entity();
        ecs_engine.add_entity_component<NArtEngine::TShaderProgramComponent>(
            cube
        ) = shader;
        ecs_engine.add_entity_component<NArtEngine::TMeshComponent>(cube) =
            mesh;

        camera_ = ecs_engine.add_entity();
        ecs_engine.add_entity_component<NArtEngine::TCameraComponent>(camera_);
        auto& camera_position =
            ecs_engine.add_entity_component<NArtEngine::TPositionComponent>(
                camera_
            );

        camera_position.position.z = 5.0f;

        create_cube_of_cubes(shader, mesh, 16);

        ecs_engine.add_system(std::make_unique<TControlCameraSystem>());
        ecs_engine.add_system(std::make_unique<TRotationSystem>());
        ecs_engine.add_system(std::make_unique<TPrintFrameTimeSystem>());
    }

    void update(const NArtEngine::TRenderingContext& context) override {
    }

    void deinit() override {
    }

  private:
    NArtEngine::TEntity create_cube_of_cubes(
        const NArtEngine::TShaderProgramComponent& shader,
        const NArtEngine::TMeshComponent& mesh, int distance
    ) {
        static constexpr int kRowCubeCount = 3;
        auto& ecs_engine                   = game_engine_->get_ecs_engine();
        if (distance <= 1) {
            auto cube = ecs_engine.get_entity(ecs_engine.add_entity());
            cube.add_component<NArtEngine::TMeshComponent>()          = mesh;
            cube.add_component<NArtEngine::TShaderProgramComponent>() = shader;
            cube.add_component<NArtEngine::TPositionComponent>();
            return cube;
        }

        auto cube_set_id = ecs_engine.add_entity();
        std::cout << cube_set_id << std::endl;
        auto cube_set = ecs_engine.get_entity(cube_set_id);
        for (int i = 0; i < kRowCubeCount; ++i) {
            for (int j = 0; j < kRowCubeCount; ++j) {
                for (int k = 0; k < kRowCubeCount; ++k) {
                    auto cube =
                        create_cube_of_cubes(shader, mesh, distance >> 1);
                    cube.add_component<NArtEngine::TParentComponent>().parent =
                        cube_set;
                    cube.add_component<NArtEngine::TPositionComponent>()
                        .position = glm::vec3{
                        static_cast<float>((i - kRowCubeCount / 2) * distance),
                        static_cast<float>((j - kRowCubeCount / 2) * distance),
                        static_cast<float>((k - kRowCubeCount / 2) * distance),
                    };
                }
            }
        }
        cube_set.add_component<NArtEngine::TPositionComponent>();
        return cube_set;
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
