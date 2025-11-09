#include "game_engine.hpp"

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "ecs_engine.hpp"
#include "input_engine.hpp"
#include "input_event_system.hpp"
#include "lifetime_system.hpp"
#include "rendering_system.hpp"
#include "resource_manager.hpp"
#include "resource_manager_system.hpp"
#include "window.hpp"

namespace NArtEngine {

class TGameEngineImpl {
  public:
    TGameEngineImpl() = default;

    void init(const TGameEngineConfig &);
    void deinit();
    void run(IGame *game);

    TECSEngine &get_ecs_engine();
    TResourceManager &get_resource_manager();

  private:
    TGameEngineConfig config_;
    TECSEngine ecs_engine_;
    TResourceManager resource_manager_;
    TInputEngine input_engine_;
    std::unique_ptr<TWindow> window_;
};

void TGameEngineImpl::init(const TGameEngineConfig &config) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw" << std::endl;
        std::exit(1);
    }

    window_ = TWindow::MakeGLFWWindow();
    if (!window_) {
        std::cerr << "Failed to create window" << std::endl;
    }
    window_->bind_current_context();

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad" << std::endl;
        std::exit(5);
    }
    config_ = config;

    resource_manager_ = TResourceManager(config.resource_manager_config);
    input_engine_.init(window_.get(), &ecs_engine_);
    ecs_engine_.add_system(std::make_unique<TRenderingSystem>());
    ecs_engine_.add_system(std::make_unique<TResourceManagerSystem>());
    ecs_engine_.add_system(std::make_unique<TRemoveHandledEventSystem>());
    ecs_engine_.add_system(std::make_unique<TLifetimeSystem>());
}

void TGameEngineImpl::deinit() {
    window_.reset();
    input_engine_.deinit();
    glfwTerminate();
}

void TGameEngineImpl::run(IGame *game) {
    glEnable(GL_DEPTH_TEST);
    game->init();
    auto start = glfwGetTime();

    TRenderingContext context{
        .start_time = start,
        .window     = window_.get(),
    };
    while (!window_->should_close()) {
        context.current_time = glfwGetTime();
        context.dt           = context.current_time - start;

        glfwPollEvents();

        if (1.0 / context.dt > config_.max_fps) {
            continue;
        }

        ecs_engine_.update(context);

        ///////////////////////////////////////////////////////////////////////
        // NOTE: update game

        game->update(context);

        start = glfwGetTime();
    }
    game->deinit();
}

TECSEngine &TGameEngineImpl::get_ecs_engine() {
    return ecs_engine_;
}

TResourceManager &TGameEngineImpl::get_resource_manager() {
    return resource_manager_;
}

TGameEngine::TGameEngine() {
}

TGameEngine::~TGameEngine() {
}

void TGameEngine::init() {
    init(TGameEngineConfig{});
}

void TGameEngine::init(const TGameEngineConfig &config) {
    assert(!impl_);

    impl_ = std::make_unique<TGameEngineImpl>();
    impl_->init(config);
}

void TGameEngine::deinit() {
    assert(impl_);

    impl_.reset();
}

void TGameEngine::run(IGame *game) {
    assert(impl_);

    impl_->run(game);
}

TECSEngine &TGameEngine::get_ecs_engine() {
    assert(impl_);
    return impl_->get_ecs_engine();
}

TResourceManager &TGameEngine::get_resource_manager() {
    assert(impl_);
    return impl_->get_resource_manager();
}

};  // namespace NArtEngine
