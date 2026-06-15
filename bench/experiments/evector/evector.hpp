// evector.hpp — a vector whose heavy logic is type-erased.
//
// `core` holds raw byte pointers; the growth/reallocation logic is in
// non-template `core_*` functions that take a `const type_ops&`. They are marked
// (noinline, noclone) on purpose: that is what makes them emit ONCE for all
// element types instead of being inlined/cloned per T (which would silently
// re-monomorphize them, since `ops_for<T>` is a compile-time constant at each
// call site — the exact thing this experiment is testing against, especially
// under LTO's interprocedural constant propagation).
//
// evector<T> is a thin typed wrapper: each method casts and calls a core
// function with ops_for<T>. The only per-T code that survives is the tiny
// wrappers + the leaf ops in type_ops.hpp + the constexpr table.
#pragma once
#include "type_ops.hpp"
#include <cstddef>
#include <cstdlib>

namespace ev {

struct core {
    unsigned char* begin = nullptr;
    unsigned char* end = nullptr;
    unsigned char* cap = nullptr;
};

#define EV_SHARED __attribute__((noinline, noclone))

inline EV_SHARED void core_reserve_exact(core& v, std::size_t new_cap_elems,
                                         const type_ops& ops) {
    std::size_t cur = static_cast<std::size_t>(v.end - v.begin) / ops.size;
    unsigned char* nb = static_cast<unsigned char*>(::malloc(new_cap_elems * ops.size));
    if (cur) ops.relocate(nb, v.begin, cur);
    if (v.begin) ::free(v.begin);
    v.begin = nb;
    v.end = nb + cur * ops.size;
    v.cap = nb + new_cap_elems * ops.size;
}

inline EV_SHARED void core_push_back(core& v, const void* elem, const type_ops& ops) {
    if (v.end == v.cap) {
        std::size_t cur = static_cast<std::size_t>(v.end - v.begin) / ops.size;
        core_reserve_exact(v, cur ? cur * 2 : 4, ops);
    }
    ops.copy_one(v.end, elem);
    v.end += ops.size;
}

inline EV_SHARED void core_destroy_all(core& v, const type_ops& ops) {
    std::size_t cur = static_cast<std::size_t>(v.end - v.begin) / ops.size;
    if (cur) ops.destroy(v.begin, cur);
    if (v.begin) ::free(v.begin);
    v.begin = v.end = v.cap = nullptr;
}

#undef EV_SHARED

template<class T>
class evector {
    core c_{};
public:
    evector() = default;
    evector(const evector&) = delete;
    evector& operator=(const evector&) = delete;
    ~evector() { core_destroy_all(c_, ops_for<T>); }

    void push_back(const T& x) { core_push_back(c_, &x, ops_for<T>); }
    void reserve(std::size_t n) {
        if (n > capacity()) core_reserve_exact(c_, n, ops_for<T>);
    }
    std::size_t size() const {
        return static_cast<std::size_t>(c_.end - c_.begin) / sizeof(T);
    }
    std::size_t capacity() const {
        return static_cast<std::size_t>(c_.cap - c_.begin) / sizeof(T);
    }
    T& operator[](std::size_t i) { return reinterpret_cast<T*>(c_.begin)[i]; }
    const T& operator[](std::size_t i) const {
        return reinterpret_cast<const T*>(c_.begin)[i];
    }
};

} // namespace ev
