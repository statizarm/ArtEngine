#include "input_engine.hpp"

#include <GLFW/glfw3.h>

#include <cassert>

#include "event_component.hpp"
#include "input_event.hpp"
#include "lifetime_component.hpp"

namespace NArtEngine {

static constexpr int64_t kEventFramesLifetime = 5;

namespace NPrivate {

static void key_callback(void*, int key, int scancode, int action, int mods);
static void mouse_key_callback(void*, int key, int action, int mods);
static void cursor_position_callback(void*, double xpos, double ypos);

}  // namespace NPrivate

class TInputEngine::TImpl {
  public:
    TImpl(TWindow* window, TECSEngine* ecs_engine);

    void init();
    void deinit();

  public:
    void key_callback(int key, int scancode, int action, int mods);
    void cursor_position_callback(double xpos, double ypos);
    void mouse_key_callback(int key, int action, int mods);

  private:
    template <typename TEvent>
    void register_event(const TEvent& event);

  private:
    TECSEngine* ecs_engine_;
    TWindow* window_;

    double prev_cursor_xpos_;
    double prev_cursor_ypos_;
};

TInputEngine::TImpl::TImpl(TWindow* window, TECSEngine* ecs_engine)
    : window_(window), ecs_engine_(ecs_engine) {
}

///////////////////////////////////////////////////////////////////////////////
// Bind and UnBind methods
///////////////////////////////////////////////////////////////////////////////

void TInputEngine::TImpl::init() {
    window_->register_keyboard_key_callback(NPrivate::key_callback);
    window_->register_mouse_key_callback(NPrivate::mouse_key_callback);
    window_->register_cursor_position_callback(
        NPrivate::cursor_position_callback
    );

    std::tie(prev_cursor_xpos_, prev_cursor_ypos_) = window_->cursor_position();
}

void TInputEngine::TImpl::deinit() {
    window_->register_keyboard_key_callback(NULL);
    window_->register_mouse_key_callback(NULL);
    window_->register_cursor_position_callback(NULL);
}

////////////////////////////////////////////////////////////////////////////////
// Event helpers
////////////////////////////////////////////////////////////////////////////////

template <typename TEvent>
void TInputEngine::TImpl::register_event(const TEvent& event) {
    auto entity_id = ecs_engine_->add_entity();
    ecs_engine_->add_entity_component<TEvent>(entity_id) = event;
    ecs_engine_->add_entity_component<TEventComponent>(entity_id);
    auto& lifetime =
        ecs_engine_->add_entity_component<TLifetimeComponent>(entity_id);
    lifetime.frames = kEventFramesLifetime;
    lifetime.time   = 0.0;
}

////////////////////////////////////////////////////////////////////////////////
// Translate Helpers
////////////////////////////////////////////////////////////////////////////////

EKey TranslateGLFWKey(int key) {
#define CASE_KEY(k)    \
    case GLFW_KEY_##k: \
        return EKey::KEY_##k
#define CASE_MOUSE_KEY(key)       \
    case GLFW_MOUSE_BUTTON_##key: \
        return EKey::MOUSE_##key

    switch (key) {
        CASE_KEY(A);
        CASE_KEY(B);
        CASE_KEY(C);
        CASE_KEY(D);
        CASE_KEY(E);
        CASE_KEY(F);
        CASE_KEY(G);
        CASE_KEY(H);
        CASE_KEY(I);
        CASE_KEY(J);
        CASE_KEY(K);
        CASE_KEY(L);
        CASE_KEY(M);
        CASE_KEY(N);
        CASE_KEY(O);
        CASE_KEY(P);
        CASE_KEY(Q);
        CASE_KEY(R);
        CASE_KEY(S);
        CASE_KEY(T);
        CASE_KEY(U);
        CASE_KEY(V);
        CASE_KEY(W);
        CASE_KEY(X);
        CASE_KEY(Y);
        CASE_KEY(Z);
        CASE_KEY(SPACE);
        CASE_MOUSE_KEY(LEFT);
        CASE_MOUSE_KEY(RIGHT);
        CASE_MOUSE_KEY(MIDDLE);
        default:
            return EKey::UNKNOWN;
    }

#undef CASE_MOUSE_KEY
#undef CASE_KEY
}

EKeyAction TranslateGLFWKeyAction(int action) {
    switch (action) {
        case GLFW_PRESS:
            return EKeyAction::PRESSED;
        case GLFW_RELEASE:
            return EKeyAction::RELEASED;
        default:
            return EKeyAction::UNKNOWN;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Callback implementations
////////////////////////////////////////////////////////////////////////////////

void TInputEngine::TImpl::key_callback(
    int key, int scancode, int action, int mods
) {
    TInputKeyboardKeyEventComponent key_event = {
        .key        = TranslateGLFWKey(key),
        .key_action = TranslateGLFWKeyAction(action),
    };
    register_event(key_event);
}

void TInputEngine::TImpl::mouse_key_callback(int key, int action, int mods) {
    TInputMouseKeyEventComponent key_event = {
        .key        = TranslateGLFWKey(key),
        .key_action = TranslateGLFWKeyAction(action),
    };
    register_event(key_event);
}

void TInputEngine::TImpl::cursor_position_callback(double xpos, double ypos) {
    TInputMouseMovedEventComponent key_event = {
        .prev_xpos = prev_cursor_xpos_,
        .prev_ypos = prev_cursor_ypos_,
        .curr_xpos = xpos,
        .curr_ypos = ypos,
    };
    register_event(key_event);
}

namespace NPrivate {

static TInputEngine::TImpl* input_engine;

static void key_callback(void*, int key, int scancode, int action, int mods) {
    assert(input_engine);

    input_engine->key_callback(key, scancode, action, mods);
}

static void mouse_key_callback(void*, int key, int action, int mods) {
    assert(input_engine);

    input_engine->mouse_key_callback(key, action, mods);
}

static void cursor_position_callback(void*, double xpos, double ypos) {
    assert(input_engine);

    input_engine->cursor_position_callback(xpos, ypos);
}

}  // namespace NPrivate

TInputEngine::TInputEngine()
    : impl_(nullptr) {
}
TInputEngine::~TInputEngine() {
}

void TInputEngine::init(TWindow* window, TECSEngine* ecs_engine) {
    assert(!impl_);

    impl_ = std::make_unique<TInputEngine::TImpl>(window, ecs_engine);
    impl_->init();
    NPrivate::input_engine = impl_.get();
}

void TInputEngine::deinit() {
    assert(!!impl_);

    NPrivate::input_engine = nullptr;
    impl_->deinit();
    impl_.reset();
}

}  // namespace NArtEngine
