// evector_dual.hpp — dual-path vector: KEEPS C++20 constexpr AND uses the
// type-erased runtime core. The whole point is to measure whether keeping
// constexpr costs runtime .text.
//
//   constant evaluation  -> typed path: std::allocator<T>::allocate +
//                           construct_at/destroy_at  (constexpr-legal)
//   runtime              -> erased path: the shared ev::core_* over bytes
//
// is_constant_evaluated() is always false at runtime, so the typed branch is
// dead there; the question is whether the compiler actually drops it.
#pragma once
#include "evector.hpp"          // ev::core, core_* , ops_for<T>
#include <memory>               // allocator, construct_at, destroy_at
#include <type_traits>          // is_constant_evaluated
#include <utility>

namespace ev {

template<class T>
class dvector {
    T* begin_ = nullptr;
    T* end_ = nullptr;
    T* cap_ = nullptr;

    constexpr void grow_typed() {
        std::size_t cur = static_cast<std::size_t>(end_ - begin_);
        std::size_t nc = cur ? cur * 2 : 4;
        std::allocator<T> a;
        T* nb = a.allocate(nc);
        for (std::size_t i = 0; i < cur; ++i) {
            std::construct_at(nb + i, std::move(begin_[i]));
            std::destroy_at(begin_ + i);
        }
        if (begin_) a.deallocate(begin_, static_cast<std::size_t>(cap_ - begin_));
        begin_ = nb; end_ = nb + cur; cap_ = nb + nc;
    }

public:
    constexpr dvector() = default;
    dvector(const dvector&) = delete;
    dvector& operator=(const dvector&) = delete;

    constexpr ~dvector() {
        if (std::is_constant_evaluated()) {
            std::allocator<T> a;
            for (T* p = begin_; p != end_; ++p) std::destroy_at(p);
            if (begin_) a.deallocate(begin_, static_cast<std::size_t>(cap_ - begin_));
        } else {
            core c{reinterpret_cast<unsigned char*>(begin_),
                   reinterpret_cast<unsigned char*>(end_),
                   reinterpret_cast<unsigned char*>(cap_)};
            core_destroy_all(c, ops_for<T>);
        }
    }

    constexpr void push_back(const T& x) {
        if (std::is_constant_evaluated()) {
            if (end_ == cap_) grow_typed();
            std::construct_at(end_, x);
            ++end_;
        } else {
            core c{reinterpret_cast<unsigned char*>(begin_),
                   reinterpret_cast<unsigned char*>(end_),
                   reinterpret_cast<unsigned char*>(cap_)};
            core_push_back(c, &x, ops_for<T>);
            begin_ = reinterpret_cast<T*>(c.begin);
            end_ = reinterpret_cast<T*>(c.end);
            cap_ = reinterpret_cast<T*>(c.cap);
        }
    }

    constexpr std::size_t size() const {
        return static_cast<std::size_t>(end_ - begin_);
    }
    constexpr const T& operator[](std::size_t i) const { return begin_[i]; }
    constexpr T& operator[](std::size_t i) { return begin_[i]; }
};

} // namespace ev
