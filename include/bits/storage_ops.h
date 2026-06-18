// bits/storage_ops.h — type-erased buffer-state vocabulary for the shared
// container cores.
//
// `type_ops` describes the ELEMENT's vocabulary (size/align/lifecycle leaves);
// `storage_ops` describes the BUFFER's vocabulary (data extent, capacity end,
// resize, free). The cores in src/support.cpp take both and dispatch through
// both. The container's job becomes trivial: just expose its allocator and a
// `begin_` pointer (and, for Strategy A, end_/cap_) and pick a `storage_ops`
// implementation that knows how to find everything else.
//
// Five slots:
//   data     -> the current live byte range [begin, end_used)
//   set_size -> truncate/extend the live byte count; zeroes one element-sized
//               spare slot at new end IF capacity allows. This is what
//               string's null terminator AND vector's spare-slot invariant
//               are: both fall out of set_size for free.
//   cap_end  -> one past the allocation (byte pointer)
//   resize   -> (re)allocate to at least min_bytes; relocates live bytes via
//               type_ops; returns new base. The container's begin_ has been
//               updated; caller uses the returned base if it caches one.
//   free     -> deallocate; reset container to empty (begin_ = nullptr).
//
// Two strategies (compile-time selected per (Container, Allocator)):
//
//   Strategy A: container stores begin_, end_, cap_. Used for fancy /
//               stateful / non-glibc allocators. sizeof = 24 B + alloc EBO.
//
//   Strategy B: container stores ONLY begin_. The allocation has a tiny
//               header at begin_ - header_size storing the live byte count;
//               capacity is recovered via ::malloc_usable_size. Empty:
//               begin_ == nullptr. sizeof = 8 B. Restricted to the system
//               malloc-equivalent allocator (std::allocator on glibc) — the
//               header offset breaks generic allocator deallocate contracts,
//               so the strategy bypasses allocator_traits and goes straight
//               to ::malloc/::free.
//
// Strategy selection: `allocator_with_usable_size<A>` plus stateless + raw-
// pointer constraints. std::allocator on glibc matches; min_allocator / pmr /
// scoped do not.
#pragma once
#include <cstddef>
#include <cstdint>
#include <bits/type_ops.h>

// Declare malloc_usable_size as the glibc symbol. Pulling <malloc.h> into
// libcis headers is awkward; we only need this one extern. Force default
// visibility so that #pragma GCC visibility push(hidden) in scope (e.g.
// inside <istream> includes) doesn't render the symbol hidden at link.
#pragma GCC visibility push(default)
extern "C" {
size_t malloc_usable_size(void* ptr) noexcept;
void*  malloc(size_t size) noexcept;
void   free(void* ptr) noexcept;
}
#pragma GCC visibility pop

namespace std {
namespace detail {

// A (begin, length-in-bytes) view returned from storage_ops::data.
struct byte_span {
    unsigned char* data;
    size_t size;
};

// The vocabulary table: six function pointers, container-agnostic.
//
// Every slot except alloc_ctx takes a `const type_ops*`. Strategy B's
// implementations are non-template and read the element size from tops->size
// and the header offset from tops->align (header_size = max(sizeof(size_t),
// tops->align)), so a single set of five symbols covers every (Container, T)
// pair gated to Strategy B. Strategy A's templated implementations still get
// size/align statically from C::value_type and ignore the tops argument; the
// uniform ABI is what lets the Strategy-B table itself be non-template.
struct storage_ops {
    byte_span (*data)     (const type_ops* tops, void* st_ctx);
    void      (*set_size) (const type_ops* tops, void* st_ctx, size_t bytes);
    void*     (*cap_end)  (const type_ops* tops, void* st_ctx);
    // resize: (re)allocate to hold at least `min_bytes` and relocate live
    // bytes into the new buffer. Internally fetches the element ctx via
    // alloc_ctx when the type_ops flags say the allocator has state.
    void*     (*resize)   (const type_ops* tops, void* st_ctx, size_t min_bytes);
    void      (*free)     (const type_ops* tops, void* st_ctx);
    // alloc_ctx: returns the container's allocator instance pointer (&alloc_)
    // when the allocator has per-instance state, else nullptr. Cores call this
    // lazily, gated by `tops->flags & f_alloc_ctx`. The name mirrors the gate
    // flag so callsites read self-documenting.
    void*     (*alloc_ctx)(void* st_ctx);
};

// ============================================================================
// Strategy A glue — container exposes its three byte pointers + allocator API
// through a `storage_a_glue<C>` specialization.
// ============================================================================
template<class C>
struct storage_a_glue;
// Specializations provide static functions:
//   unsigned char* begin(C*);
//   unsigned char* end  (C*);
//   unsigned char* cap  (C*);
//   void set_begin(C*, void*);
//   void set_end  (C*, void*);
//   void set_cap  (C*, void*);
//   void* allocate(C*, size_t bytes, size_t* got_bytes);  // sets *got_bytes
//   void  deallocate(C*, void* p, size_t bytes);

// Strategy A's templated slots take `tops` for uniform ABI but ignore it —
// element size / alignment come statically from C::value_type via the glue.
template<class C>
static byte_span storage_a_data(const type_ops*, void* st_ctx) {
    using G = storage_a_glue<C>;
    auto* c = static_cast<C*>(st_ctx);
    unsigned char* b = G::begin(c);
    unsigned char* e = G::end(c);
    return { b, static_cast<size_t>(e - b) };
}

template<class C>
static void* storage_a_cap_end(const type_ops*, void* st_ctx) {
    return storage_a_glue<C>::cap(static_cast<C*>(st_ctx));
}

template<class C>
static void storage_a_set_size(const type_ops*, void* st_ctx, size_t bytes) {
    using G = storage_a_glue<C>;
    auto* c = static_cast<C*>(st_ctx);
    unsigned char* b = G::begin(c);
    unsigned char* new_end = b + bytes;
    G::set_end(c, new_end);
    if (b != nullptr) {
        unsigned char* cap = G::cap(c);
        constexpr size_t es = sizeof(typename C::value_type);
        if (new_end + es <= cap)
            __builtin_memset(new_end, 0, es);
    }
}

template<class C>
static void storage_a_free(const type_ops*, void* st_ctx) {
    using G = storage_a_glue<C>;
    auto* c = static_cast<C*>(st_ctx);
    unsigned char* b = G::begin(c);
    if (b) {
        const size_t total = static_cast<size_t>(G::cap(c) - b);
        G::deallocate(c, b, total);
    }
    G::set_begin(c, nullptr);
    G::set_end  (c, nullptr);
    G::set_cap  (c, nullptr);
}

// Forward declarations of the byte-level relocation primitive used by
// resize. Defined in src/support.cpp.
void relocate_live(const type_ops* tops, void* el_ctx,
                   unsigned char* dst, unsigned char* src, size_t live_bytes);

// alloc_ctx slot: returns &c->alloc_ if the allocator has per-instance state,
// else nullptr. The container exposes &alloc_ via the glue (alloc_ptr()).
template<class C>
static void* storage_a_alloc_ctx(void* st_ctx) {
    using A = typename C::allocator_type;
    if constexpr (alloc_stateless<A>) {
        (void)st_ctx;
        return nullptr;
    } else {
        return storage_a_glue<C>::alloc_ptr(static_cast<C*>(st_ctx));
    }
}

template<class C>
static void* storage_a_resize(const type_ops* tops, void* st_ctx,
                              size_t min_bytes) {
    using G = storage_a_glue<C>;
    auto* c = static_cast<C*>(st_ctx);
    unsigned char* old_begin = G::begin(c);
    unsigned char* old_end   = G::end(c);
    const size_t live = static_cast<size_t>(old_end - old_begin);
    const size_t old_total = old_begin ? static_cast<size_t>(G::cap(c) - old_begin) : 0;

    size_t got = 0;
    unsigned char* nb = static_cast<unsigned char*>(
        G::allocate(c, min_bytes, &got));

    if (live) {
        // Only fetch the allocator instance when the leaves actually need it.
        void* ec = (tops->flags & f_alloc_ctx)
                 ? storage_a_alloc_ctx<C>(st_ctx)
                 : nullptr;
        relocate_live(tops, ec, nb, old_begin, live);
    }
    if (old_begin)
        G::deallocate(c, old_begin, old_total);

    G::set_begin(c, nb);
    G::set_end  (c, nb + live);
    G::set_cap  (c, nb + got);

    // Spare slot at the new end (one element zeroed), if capacity allows.
    constexpr size_t es = sizeof(typename C::value_type);
    if (nb + live + es <= nb + got)
        __builtin_memset(nb + live, 0, es);

    return nb;
}

template<class C>
inline constexpr storage_ops storage_ops_a{
    &storage_a_data     <C>,
    &storage_a_set_size <C>,
    &storage_a_cap_end  <C>,
    &storage_a_resize   <C>,
    &storage_a_free     <C>,
    &storage_a_alloc_ctx<C>,
};

// ============================================================================
// Strategy B — 1-pointer layout
// ============================================================================
// Container stores only begin_. The allocation has a small header storing the
// live byte count; capacity comes from malloc_usable_size. Empty: begin_ == nullptr.
//
// Restricted to allocators equivalent to system malloc — the strategy bypasses
// the allocator API on the free path because the recorded pointer is offset
// from the allocator's allocate() return.
//
// Header layout:
//     [header: size_t live_bytes][value_type buffer ...]
//   header_size = max(sizeof(size_t), alignof(value_type))
//
// Concept: must be the system `std::allocator<T>` exactly. Generic stateless
// raw-pointer allocators that *happen* to expose value_type* (e.g.
// libcxx-test's min_allocator) are NOT allowed: their allocate/deallocate
// must round-trip the same pointer, but Strategy B stores `begin_` offset
// from the allocation base (after the header). std::allocator on glibc is
// the only allocator we know we can bypass via raw ::malloc/::free.
template<class T> class allocator;

template<class Alloc>
concept allocator_with_usable_size =
    requires(typename Alloc::value_type* p) {
        { ::malloc_usable_size(p) } -> std::convertible_to<std::size_t>;
    }
    && is_same_v<Alloc, ::std::allocator<typename Alloc::value_type>>;

// Strategy B is gated to std::allocator on glibc, which is stateless and
// always rebinds to ::malloc / ::free. Combined with `begin_` being at offset
// 0 of every Strategy-B container (`vector_layout<P, true>` and
// `string_layout<P, true>` each have `begin_` as their only field; the layout
// base is the container's first / only base), the byte work below is fully
// container-agnostic: element size comes from `tops->size`, header_size from
// `tops->align`, and `*(void**)st_ctx` is `c->begin_`. So one set of five
// non-template symbols (declared here, defined in src/support.cpp) replaces
// the per-`C` `storage_b_*<C>` instantiations of the old design, and the
// `storage_ops` table for Strategy B becomes a single shared constant rather
// than a per-`C` template.
//
// header_size = max(sizeof(size_t), tops->align). Header layout:
//     [header: size_t live_bytes][value_type buffer ...]
byte_span storage_b_data    (const type_ops* tops, void* st_ctx);
void      storage_b_set_size(const type_ops* tops, void* st_ctx, size_t bytes);
void*     storage_b_cap_end (const type_ops* tops, void* st_ctx);
void*     storage_b_resize  (const type_ops* tops, void* st_ctx, size_t min_bytes);
void      storage_b_free    (const type_ops* tops, void* st_ctx);
// Strategy B is gated to std::allocator (stateless), so the slot always
// returns nullptr. Kept for ABI uniformity with Strategy A.
void*     storage_b_alloc_ctx(void* st_ctx);

inline constexpr storage_ops storage_ops_b{
    &storage_b_data,
    &storage_b_set_size,
    &storage_b_cap_end,
    &storage_b_resize,
    &storage_b_free,
    &storage_b_alloc_ctx,
};

// ============================================================================
// Strategy selection and container-facing handle.
// ============================================================================
template<class C, class A>
inline constexpr bool storage_uses_header_layout = allocator_with_usable_size<A>;

template<class C>
constexpr const storage_ops& pick_storage_for() noexcept {
    if constexpr (storage_uses_header_layout<C, typename C::allocator_type>) {
        return storage_ops_b;
    } else {
        return storage_ops_a<C>;
    }
}

template<class C>
inline constexpr const storage_ops& storage_for_v = pick_storage_for<C>();

// ============================================================================
// Layout bases — the container inherits these privately. Strategy A holds
// three pointers; Strategy B holds only `begin_` (size + cap recovered from
// a header + ::malloc_usable_size). Two specializations so that sizeof for
// the Strategy-B inheritor is exactly one pointer.
// ============================================================================
template<class Pointer, bool UsesHeaderLayout>
struct vector_layout;

template<class Pointer>
struct vector_layout<Pointer, false> {
    Pointer begin_ = Pointer{};
    Pointer end_   = Pointer{};
    Pointer cap_   = Pointer{};
};

template<class Pointer>
struct vector_layout<Pointer, true> {
    Pointer begin_ = Pointer{};
};

template<class Pointer, bool UsesHeaderLayout>
struct string_layout;

template<class Pointer>
struct string_layout<Pointer, false> {
    Pointer begin_ = Pointer{};
    Pointer end_   = Pointer{};
    Pointer cap_   = Pointer{};
};

template<class Pointer>
struct string_layout<Pointer, true> {
    Pointer begin_ = Pointer{};
};

} // namespace detail
} // namespace std
