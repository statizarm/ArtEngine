#pragma once

#include <memory>

namespace NArtEngine {

using TKeyboardKeyCallback =
    void(void*, int key, int scancode, int action, int mods);
using TMouseKeyCallback       = void(void*, int key, int action, int mods);
using TCursorPositionCallback = void(void*, double xpos, double ypos);

class TWindowImpl;

class TWindow {
  public:
    TWindow();
    ~TWindow();

  public:
    static std::unique_ptr<TWindow> MakeGLFWWindow();

  public:
    // getters
    int width() const;
    int height() const;
    std::pair<int, int> window_size() const;

    std::pair<double, double> cursor_position() const;

  public:
    bool should_close();

    void bind_current_context();
    void swap_buffers();

    void grab_cursor() const;
    void ungrab_cursor() const;

    void register_keyboard_key_callback(TKeyboardKeyCallback callback);
    void register_mouse_key_callback(TMouseKeyCallback callback);
    void register_cursor_position_callback(TCursorPositionCallback callback);

  private:
    TWindow(std::unique_ptr<TWindowImpl> impl);

  private:
    std::unique_ptr<TWindowImpl> impl_;
};

}  // namespace NArtEngine
