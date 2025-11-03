#include "engine.hpp"

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "window.hpp"

namespace NArtEngine {

class TGameEngineImpl {
  public:
    TGameEngineImpl() = default;

    void init();
    void deinit();
    void run(IGame *game);

  public:
    // NOTE: Various callbacks
    void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

  private:
    std::unique_ptr<TWindow> window_;
};

void TGameEngineImpl::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw" << std::endl;
        std::exit(1);
    }

    window_ = TWindow::MakeGLFWWindow();
    if (!window_) {
        std::cerr << "Failed to create window" << std::endl;
    }
    window_->bindCurrentContext();

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad" << std::endl;
        std::exit(5);
    }
}

void TGameEngineImpl::deinit() {
    window_.reset();
    glfwTerminate();
}

void TGameEngineImpl::run(IGame *game) {
    glEnable(GL_DEPTH_TEST);
    game->init();
    auto start = glfwGetTime();
    while (!window_->shouldClose()) {
        ///////////////////////////////////////////////////////////////////////
        // NOTE: DRAW
        auto [width, height] = window_->window_size();
        glViewport(0, 0, width, height);
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(width) / static_cast<float>(height),
            0.1f,
            100.f
        );
        auto vp = projection;

        window_->swapBuffers();
        glfwPollEvents();

        auto duration = glfwGetTime() - start;
        ///////////////////////////////////////////////////////////////////////
        // NOTE: update physics

        ///////////////////////////////////////////////////////////////////////
        // NOTE: update game

        game->update(duration);

        start = glfwGetTime();
    }
    game->deinit();
}

TGameEngine::TGameEngine() {
}

TGameEngine::~TGameEngine() {
}

void TGameEngine::init() {
    assert(!impl_);

    impl_ = std::make_unique<TGameEngineImpl>();
    impl_->init();
}

void TGameEngine::deinit() {
    assert(impl_);

    impl_.reset();
}

void TGameEngine::run(IGame *game) {
    assert(impl_);

    impl_->run(game);
}

};  // namespace NArtEngine
