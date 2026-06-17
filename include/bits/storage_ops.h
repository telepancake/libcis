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

// The vocabulary table: five function pointers, container-agnostic.
struct storage_ops {
    byte_span (*data)    (void* st_ctx);
    void      (*set_size)(void* st_ctx, size_t bytes);
    void*     (*cap_end) (void* st_ctx);
    // resize takes the element type_ops (for the live-bytes relocation) and
    // the element ctx (for allocator-mediated move_construct/destroy leaves).
    void*     (*resize)  (const type_ops* tops, void* st_ctx, void* el_ctx,
                          size_t min_bytes);
    void      (*free)    (void* st_ctx);
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

template<class C>
static byte_span storage_a_data(void* st_ctx) {
    using G = storage_a_glue<C>;
    auto* c = static_cast<C*>(st_ctx);
    unsigned char* b = G::begin(c);
    unsigned char* e = G::end(c);
    return { b, static_cast<size_t>(e - b) };
}

template<class C>
static void* storage_a_cap_end(void* st_ctx) {
    return storage_a_glue<C>::cap(static_cast<C*>(st_ctx));
}

template<class C>
static void storage_a_set_size(void* st_ctx, size_t bytes) {
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
static void storage_a_free(void* st_ctx) {
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

template<class C>
static void* storage_a_resize(const type_ops* tops, void* st_ctx, void* el_ctx,
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

    if (live)
        relocate_live(tops, el_ctx, nb, old_begin, live);
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
    &storage_a_data    <C>,
    &storage_a_set_size<C>,
    &storage_a_cap_end <C>,
    &storage_a_resize  <C>,
    &storage_a_free    <C>,
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

template<class C>
struct storage_b_glue;
// Specializations provide:
//   unsigned char* begin(C*);
//   void           set_begin(C*, void*);
//   void*          raw_alloc(C*, size_t bytes);
//   void           raw_free (C*, void* base);

template<class C>
struct storage_b_traits {
    using value_type = typename C::value_type;
    static constexpr size_t header_size =
        sizeof(size_t) > alignof(value_type) ? sizeof(size_t) : alignof(value_type);
};

template<class C>
static byte_span storage_b_data(void* st_ctx) {
    auto* c = static_cast<C*>(st_ctx);
    unsigned char* b = storage_b_glue<C>::begin(c);
    if (b == nullptr) return { nullptr, 0 };
    size_t live;
    __builtin_memcpy(&live, b - storage_b_traits<C>::header_size, sizeof(size_t));
    return { b, live };
}

template<class C>
static void* storage_b_cap_end(void* st_ctx) {
    auto* c = static_cast<C*>(st_ctx);
    unsigned char* b = storage_b_glue<C>::begin(c);
    if (b == nullptr) return nullptr;
    constexpr size_t H = storage_b_traits<C>::header_size;
    unsigned char* base = b - H;
    size_t total = ::malloc_usable_size(base);
    return base + total;
}

template<class C>
static void storage_b_set_size(void* st_ctx, size_t bytes) {
    auto* c = static_cast<C*>(st_ctx);
    unsigned char* b = storage_b_glue<C>::begin(c);
    if (b == nullptr) return;
    constexpr size_t H = storage_b_traits<C>::header_size;
    __builtin_memcpy(b - H, &bytes, sizeof(size_t));
    unsigned char* base = b - H;
    size_t total = ::malloc_usable_size(base);
    unsigned char* cap = base + total;
    unsigned char* new_end = b + bytes;
    constexpr size_t es = sizeof(typename C::value_type);
    if (new_end + es <= cap)
        __builtin_memset(new_end, 0, es);
}

template<class C>
static void storage_b_free(void* st_ctx) {
    auto* c = static_cast<C*>(st_ctx);
    unsigned char* b = storage_b_glue<C>::begin(c);
    if (b) {
        constexpr size_t H = storage_b_traits<C>::header_size;
        storage_b_glue<C>::raw_free(c, b - H);
    }
    storage_b_glue<C>::set_begin(c, nullptr);
}

template<class C>
static void* storage_b_resize(const type_ops* tops, void* st_ctx, void* el_ctx,
                              size_t min_bytes) {
    auto* c = static_cast<C*>(st_ctx);
    constexpr size_t H = storage_b_traits<C>::header_size;
    unsigned char* old_begin = storage_b_glue<C>::begin(c);
    size_t live = 0;
    if (old_begin) {
        __builtin_memcpy(&live, old_begin - H, sizeof(size_t));
    }

    size_t want = H + min_bytes;
    unsigned char* base = static_cast<unsigned char*>(
        storage_b_glue<C>::raw_alloc(c, want));
    unsigned char* nb = base + H;

    if (live)
        relocate_live(tops, el_ctx, nb, old_begin, live);
    if (old_begin)
        storage_b_glue<C>::raw_free(c, old_begin - H);

    __builtin_memcpy(base, &live, sizeof(size_t));
    storage_b_glue<C>::set_begin(c, nb);

    size_t total = ::malloc_usable_size(base);
    unsigned char* cap = base + total;
    constexpr size_t es = sizeof(typename C::value_type);
    if (nb + live + es <= cap)
        __builtin_memset(nb + live, 0, es);

    return nb;
}

template<class C>
inline constexpr storage_ops storage_ops_b{
    &storage_b_data    <C>,
    &storage_b_set_size<C>,
    &storage_b_cap_end <C>,
    &storage_b_resize  <C>,
    &storage_b_free    <C>,
};

// ============================================================================
// Strategy selection and container-facing handle.
// ============================================================================
template<class C, class A>
inline constexpr bool storage_uses_header_layout = allocator_with_usable_size<A>;

template<class C>
constexpr const storage_ops& pick_storage_for() noexcept {
    if constexpr (storage_uses_header_layout<C, typename C::allocator_type>) {
        return storage_ops_b<C>;
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
