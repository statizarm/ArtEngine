#include "window.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

namespace NArtEngine {

class TWindowImpl {
  public:
    virtual ~TWindowImpl() = default;

  public:
    virtual std::pair<int, int> window_size() const           = 0;
    virtual std::pair<double, double> cursor_position() const = 0;

  public:
    virtual bool should_close() = 0;

    virtual void bind_current_context() = 0;
    virtual void swap_buffers()         = 0;

    virtual void grab_cursor()   = 0;
    virtual void ungrab_cursor() = 0;

    virtual void register_keyboard_key_callback(TKeyboardKeyCallback callback
    )                                                                    = 0;
    virtual void register_mouse_key_callback(TMouseKeyCallback callback) = 0;
    virtual void register_cursor_position_callback(
        TCursorPositionCallback callback
    ) = 0;
};

class TGLFWWindow : public TWindowImpl {
  public:
    TGLFWWindow(GLFWwindow* window);
    ~TGLFWWindow();

  public:
    std::pair<int, int> window_size() const override;
    virtual std::pair<double, double> cursor_position() const override;

  public:
    bool should_close() override;

    void bind_current_context() override;
    void swap_buffers() override;

    void grab_cursor() override;
    void ungrab_cursor() override;

    void register_keyboard_key_callback(TKeyboardKeyCallback) override;
    void register_mouse_key_callback(TMouseKeyCallback) override;
    void register_cursor_position_callback(TCursorPositionCallback) override;

  private:
    GLFWwindow* window_;
};

TGLFWWindow::TGLFWWindow(GLFWwindow* window)
    : window_(window) {
}

TGLFWWindow::~TGLFWWindow() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
}

std::pair<int, int> TGLFWWindow::window_size() const {
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    return {width, height};
}

std::pair<double, double> TGLFWWindow::cursor_position() const {
    double xpos, ypos;
    glfwGetCursorPos(window_, &xpos, &ypos);
    return {xpos, ypos};
}

bool TGLFWWindow::should_close() {
    return glfwWindowShouldClose(window_);
}

void TGLFWWindow::bind_current_context() {
    glfwMakeContextCurrent(window_);
}

void TGLFWWindow::swap_buffers() {
    glfwSwapBuffers(window_);
}

void TGLFWWindow::grab_cursor() {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void TGLFWWindow::ungrab_cursor() {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void TGLFWWindow::register_keyboard_key_callback(TKeyboardKeyCallback callback
) {
    glfwSetKeyCallback(window_, reinterpret_cast<GLFWkeyfun>(callback));
}

void TGLFWWindow::register_mouse_key_callback(TMouseKeyCallback callback) {
    glfwSetMouseButtonCallback(
        window_, reinterpret_cast<GLFWmousebuttonfun>(callback)
    );
}

void TGLFWWindow::register_cursor_position_callback(
    TCursorPositionCallback callback
) {
    glfwSetCursorPosCallback(
        window_, reinterpret_cast<GLFWcursorposfun>(callback)
    );
}

static void ErrorCallback(int error, const char* description) {
    std::cerr << "Error: %s\n" << description << std::endl;
}

std::unique_ptr<TWindow> TWindow::MakeGLFWWindow() {
    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    auto window = glfwCreateWindow(640, 480, "GachiBall", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        return nullptr;
    }

    TWindow* w = new TWindow(std::make_unique<TGLFWWindow>(window));
    return std::unique_ptr<TWindow>{w};
}

TWindow::TWindow() {
}

TWindow::~TWindow() {
}

int TWindow::width() const {
    return window_size().first;
}

int TWindow::height() const {
    return window_size().second;
}

std::pair<int, int> TWindow::window_size() const {
    return impl_->window_size();
}

std::pair<double, double> TWindow::cursor_position() const {
    return impl_->cursor_position();
}

bool TWindow::should_close() {
    return impl_->should_close();
}

void TWindow::bind_current_context() {
    return impl_->bind_current_context();
}

void TWindow::swap_buffers() {
    return impl_->swap_buffers();
}

void TWindow::grab_cursor() {
    impl_->grab_cursor();
}

void TWindow::ungrab_cursor() {
    impl_->ungrab_cursor();
}

void TWindow::register_keyboard_key_callback(TKeyboardKeyCallback callback) {
    impl_->register_keyboard_key_callback(std::move(callback));
}

void TWindow::register_mouse_key_callback(TMouseKeyCallback callback) {
    impl_->register_mouse_key_callback(std::move(callback));
}

void TWindow::register_cursor_position_callback(TCursorPositionCallback callback
) {
    impl_->register_cursor_position_callback(std::move(callback));
}

TWindow::TWindow(std::unique_ptr<TWindowImpl> impl)
    : impl_(std::move(impl)) {
}

}  // namespace NArtEngine
