#pragma once

#include "entities_view.hpp"
#include "rendering_context.hpp"

namespace NArtEngine {

enum ESystemMode {
    ENABLED,
    DISABLED,
};

template <typename TSystem>
concept TRunableSystem = requires(std::decay_t<TSystem> a) {
    a.run(TRenderingContext{}, TEntitiesView<TEntity>{});
};

template <typename TSystem>
concept CSystem = TRunableSystem<std::decay_t<TSystem>>;

template <typename T, CSystem TSystem>
struct TSystemTraits {
    using TEntityType = T;
};

class TTypeErasedSystem {
  public:
    TTypeErasedSystem(const TTypeErasedSystem&) = delete;
    TTypeErasedSystem(TTypeErasedSystem&& other);
    ~TTypeErasedSystem();

    TTypeErasedSystem& operator=(const TTypeErasedSystem&) = delete;
    TTypeErasedSystem& operator=(TTypeErasedSystem&& other);

  public:
    template <CSystem TSystem>
    static TTypeErasedSystem create(TSystem&&);

  public:
    void run(
        const TRenderingContext& context, const TEntitiesView<TEntity>& view
    );

  private:
    using TDeleter = void (*)(void*);
    using TCall =
        void (*)(void*, const TRenderingContext&, const TEntitiesView<TEntity>&);

  private:
    TTypeErasedSystem() = default;

  private:
    void* memory_     = nullptr;
    TDeleter deleter_ = nullptr;
    TCall call_       = nullptr;
};

}  // namespace NArtEngine

#include "system-inl.hpp"
