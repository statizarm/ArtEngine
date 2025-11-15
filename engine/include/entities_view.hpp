#pragma once

#include <iterator>

#include "entity.hpp"

namespace NArtEngine {

using TEntityContainer = std::vector<TEntity>;

template <typename T>
class TEntityIterator;

template <>
class TEntityIterator<TEntity> {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = TEntity;
    using pointer           = TEntity*;
    using reference         = TEntity&;

  public:
    TEntityIterator(TEntityContainer::iterator real)
        : real_(real) {
    }

    TEntity& operator*() {
        return static_cast<TEntity&>(*real_);
    }
    TEntity* operator->() {
        return static_cast<TEntity*>(&*real_);
    }

    TEntityIterator<TEntity> operator++() {
        ++real_;
        return *this;
    }
    TEntityIterator<TEntity> operator++(int) {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(const TEntityIterator<TEntity>& other) const {
        return real_ == other.real_;
    }

  private:
    TEntityContainer::iterator real_;
};

template <typename T>
class TEntitiesView;

template <>
class TEntitiesView<TEntity> {
  public:
    TEntitiesView() = default;
    TEntitiesView(TEntityContainer& container)
        : begin_(container.begin()), end_(container.end()) {
    }

  public:
    TEntityIterator<TEntity> begin() const {
        return TEntityIterator<TEntity>(begin_);
    }
    TEntityIterator<TEntity> end() const {
        return TEntityIterator<TEntity>(end_);
    }

  private:
    TEntityContainer::iterator begin_;
    TEntityContainer::iterator end_;
};

}  // namespace NArtEngine
