// evector.hpp — a vector whose heavy logic is type-erased.
//
// `core` holds raw byte pointers; the growth/reallocation logic is in
// non-template `core_*` functions that take a `const type_ops&`, so the
// orchestration is emitted independent of the element type.
//
// NOTE: an earlier version marked these (noinline, noclone) on the theory that
// the compiler would otherwise clone/inline the core per element type and
// re-monomorphize it. That theory was tested and is FALSE at -Os: g++-10 and
// g++-13 emit the core exactly ONCE (shared, out-of-line) and dispatch through
// the table; clang-18 inlines+devirtualizes+fuses it and comes out smaller.
// The attributes only fought those optimizations (never smaller, sometimes
// bigger), so they were removed. (See the cross-compiler measurement.)
//
// evector<T> is a thin typed wrapper: each method casts and calls a core
// function with ops_for<T>. The only per-T code that survives is the tiny
// wrappers + the leaf ops in type_ops.hpp + the constexpr table.
#pragma once
#include "type_ops.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace ev {

struct core {
    unsigned char* begin = nullptr;
    unsigned char* end = nullptr;
    unsigned char* cap = nullptr;
};

// Left as a no-op: the compiler shares/fuses the core correctly on its own.
#define EV_SHARED

inline EV_SHARED void core_reserve_exact(core& v, std::size_t new_cap_elems,
                                         const type_ops& ops) {
    std::size_t cur = static_cast<std::size_t>(v.end - v.begin) / ops.size;
    unsigned char* nb = static_cast<unsigned char*>(::malloc(new_cap_elems * ops.size));
    if (cur) {
        if (ops.relocate) ops.relocate(nb, v.begin, cur);          // non-trivial
        else std::memmove(nb, v.begin, cur * ops.size);            // trivially relocatable
    }
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
    if (ops.copy_one) ops.copy_one(v.end, elem);                   // non-trivial
    else std::memcpy(v.end, elem, ops.size);                       // trivially copyable
    v.end += ops.size;
}

inline EV_SHARED void core_destroy_all(core& v, const type_ops& ops) {
    std::size_t cur = static_cast<std::size_t>(v.end - v.begin) / ops.size;
    if (cur && ops.destroy) ops.destroy(v.begin, cur);             // null => trivially destructible
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
