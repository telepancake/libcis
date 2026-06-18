// src/support.cpp — freestanding C++ runtime support for libcis.
//
// Provides ALL symbols normally imported from libstdc++/libsupc++ so that
// the final binary can be linked with -nodefaultlibs (only libc + compiler
// runtime needed).
//
// Compilation:
//   g++-10 -std=gnu++20 -fno-exceptions -fno-rtti \
//          -nostdinc++ -Iinclude -c src/support.cpp
//
// We pull in the C standard headers (stdlib.h, string.h, stdint.h, sched.h)
// and our own libcis headers for the types that must match (<new>, <exception>).

#include <new>         // nothrow_t, align_val_t, new_handler declarations
#include <exception>   // exception, bad_exception, terminate_handler declarations
#include <stdlib.h>    // malloc, free, abort, aligned_alloc / posix_memalign
#include <string.h>    // memcpy / memmove for the trivial-relocation cores
#include <stdint.h>    // int64_t, uint8_t
#include <sched.h>     // sched_yield — for guard spin loop
#include <bits/cores.h>
#include <bits/value_ops.h>
#include <bits/algo_cores.h>
#include <bits/sum_ops.h>

// ---------------------------------------------------------------------------
// Container cores — non-template bodies that vector/string/etc. forward into.
// Each is keyed by a const type_ops* (element leaves) and, for grow, a
// const storage_ops* (buffer state vocabulary) + void* st_ctx + void* el_ctx.
// ---------------------------------------------------------------------------
namespace std {
namespace detail {

// Trivial-fast-path predicates: BOTH the type is trivially X AND the
// allocator has default lifecycle (so memcpy/memset/skip is semantically
// equivalent to calling the leaf).
static inline bool triv_destroy(const type_ops* ops) {
    return (ops->flags & f_triv_destroy) && (ops->flags & f_alloc_default_life);
}
static inline bool triv_default(const type_ops* ops) {
    return (ops->flags & f_triv_default) && (ops->flags & f_alloc_default_life);
}
static inline bool triv_copy(const type_ops* ops) {
    return (ops->flags & f_triv_copy) && (ops->flags & f_alloc_default_life);
}
static inline bool triv_reloc(const type_ops* ops) {
    return (ops->flags & f_triv_reloc) && (ops->flags & f_alloc_default_life);
}

// Fetch the element ctx lazily: only when the type_ops flag says the
// allocator has per-instance state, ask storage_ops for &alloc_. Stateless
// allocators (std::allocator, min_allocator, ...) never trigger the slot.
static inline void* fetch_ec(const type_ops* ops, const storage_ops* sops,
                             void* st_ctx) {
    return (ops->flags & f_alloc_ctx) ? sops->alloc_ctx(st_ctx) : nullptr;
}

void destroy_range(const type_ops* ops, const storage_ops* sops,
                   void* st_ctx, void* begin, void* end) {
    if (begin > end) __builtin_trap();
    if (triv_destroy(ops)) return;
    void* el_ctx = fetch_ec(ops, sops, st_ctx);
    const size_t sz = ops->size;
    auto p = static_cast<unsigned char*>(end);
    auto q = static_cast<unsigned char*>(begin);
    while (p != q) {
        p -= sz;
        ops->destroy(el_ctx, p);
    }
}

void construct_copy_one_n(const type_ops* ops, const storage_ops* sops,
                          void* st_ctx, void* dst, const void* src, size_t n) {
    const size_t sz = ops->size;
    auto p = static_cast<unsigned char*>(dst);
    if (triv_copy(ops)) {
        for (size_t i = 0; i < n; ++i, p += sz)
            __builtin_memcpy(p, src, sz);
        return;
    }
    void* el_ctx = fetch_ec(ops, sops, st_ctx);
    for (size_t i = 0; i < n; ++i, p += sz)
        ops->copy_construct(el_ctx, p, src);
}

// Relocate every element of [src, src+size_bytes) into [dst, ...). Iterates
// forward — safe for non-overlapping ranges or for shift-DOWN (dst < src).
// Source elements are destroyed once moved.
static void relocate_block(const type_ops* ops, void* el_ctx,
                           void* dst, void* src, size_t size_bytes) {
    if (triv_reloc(ops)) {
        // triv-reloc + default lifecycle: memmove handles both overlap cases.
        __builtin_memmove(dst, src, size_bytes);
        return;
    }
    const size_t sz = ops->size;
    auto* d = static_cast<unsigned char*>(dst);
    auto* s = static_cast<unsigned char*>(src);
    const size_t n = size_bytes / sz;
    for (size_t i = 0; i < n; ++i, d += sz, s += sz) {
        ops->move_construct(el_ctx, d, s);
        if (!triv_destroy(ops)) ops->destroy(el_ctx, s);
    }
}

// Same, but iterates BACKWARD — required for the leaf-path shift-UP
// (dst > src) where the source range overlaps with the destination range
// from above.
static void relocate_block_rev(const type_ops* ops, void* el_ctx,
                               void* dst, void* src, size_t size_bytes) {
    if (triv_reloc(ops)) {
        __builtin_memmove(dst, src, size_bytes);
        return;
    }
    const size_t sz = ops->size;
    auto* d = static_cast<unsigned char*>(dst) + size_bytes;
    auto* s = static_cast<unsigned char*>(src) + size_bytes;
    const size_t n = size_bytes / sz;
    for (size_t i = 0; i < n; ++i) {
        d -= sz; s -= sz;
        ops->move_construct(el_ctx, d, s);
        if (!triv_destroy(ops)) ops->destroy(el_ctx, s);
    }
}

// Public wrapper: storage_ops::resize implementations call this to relocate
// live bytes from an old buffer to a freshly-allocated new one.
void relocate_live(const type_ops* ops, void* el_ctx,
                   unsigned char* dst, unsigned char* src, size_t live_bytes) {
    relocate_block(ops, el_ctx, dst, src, live_bytes);
}

void grow(const type_ops* tops, const storage_ops* sops,
          void* st_ctx, size_t min_bytes) {
    // resize fetches el_ctx internally via sops->alloc_ctx if needed.
    sops->resize(tops, st_ctx, min_bytes);
}

// ---------------------------------------------------------------------------
// Strategy B — the one set of buffer-vocab symbols shared by every container
// gated to std::allocator on glibc. begin_ is at offset 0 of the container
// (`vector_layout<P, true>` / `string_layout<P, true>` have it as their only
// field, and the layout base is the container's only base — the empty
// allocator collapses under [[no_unique_address]]), so `*(void**)st_ctx` is
// the live data pointer. Element size comes from `tops->size`; header_size
// from max(sizeof(size_t), tops->align). See bits/storage_ops.h for the
// design comment.
// ---------------------------------------------------------------------------
static inline size_t storage_b_header_size(const type_ops* tops) noexcept {
    const size_t a = tops->align;
    return sizeof(size_t) > a ? sizeof(size_t) : a;
}

byte_span storage_b_data(const type_ops* tops, void* st_ctx) {
    auto* b = *static_cast<unsigned char**>(st_ctx);
    if (b == nullptr) return { nullptr, 0 };
    size_t live;
    __builtin_memcpy(&live, b - storage_b_header_size(tops), sizeof(size_t));
    return { b, live };
}

void* storage_b_cap_end(const type_ops* tops, void* st_ctx) {
    auto* b = *static_cast<unsigned char**>(st_ctx);
    if (b == nullptr) return nullptr;
    unsigned char* base = b - storage_b_header_size(tops);
    return base + ::malloc_usable_size(base);
}

void storage_b_set_size(const type_ops* tops, void* st_ctx, size_t bytes) {
    auto* b = *static_cast<unsigned char**>(st_ctx);
    if (b == nullptr) return;
    const size_t H = storage_b_header_size(tops);
    __builtin_memcpy(b - H, &bytes, sizeof(size_t));
    unsigned char* base = b - H;
    unsigned char* cap  = base + ::malloc_usable_size(base);
    unsigned char* new_end = b + bytes;
    const size_t es = tops->size;
    if (new_end + es <= cap)
        __builtin_memset(new_end, 0, es);
}

void storage_b_free(const type_ops* tops, void* st_ctx) {
    auto** slot = static_cast<unsigned char**>(st_ctx);
    unsigned char* b = *slot;
    if (b) ::free(b - storage_b_header_size(tops));
    *slot = nullptr;
}

void* storage_b_alloc_ctx(void* /*st_ctx*/) {
    return nullptr;
}

void* storage_b_resize(const type_ops* tops, void* st_ctx, size_t min_bytes) {
    auto** slot = static_cast<unsigned char**>(st_ctx);
    const size_t H = storage_b_header_size(tops);
    unsigned char* old_begin = *slot;
    size_t live = 0;
    if (old_begin)
        __builtin_memcpy(&live, old_begin - H, sizeof(size_t));

    auto* base = static_cast<unsigned char*>(::malloc(H + min_bytes));
    if (!base) __builtin_trap();
    unsigned char* nb = base + H;

    if (live) {
        // Strategy B's gate guarantees std::allocator (stateless), so
        // f_alloc_ctx is never set here — the check folds away. Kept for
        // uniformity with Strategy A.
        void* ec = (tops->flags & f_alloc_ctx) ? storage_b_alloc_ctx(st_ctx)
                                               : nullptr;
        relocate_live(tops, ec, nb, old_begin, live);
    }
    if (old_begin) ::free(old_begin - H);

    __builtin_memcpy(base, &live, sizeof(size_t));
    *slot = nb;

    unsigned char* cap = base + ::malloc_usable_size(base);
    const size_t es = tops->size;
    if (nb + live + es <= cap)
        __builtin_memset(nb + live, 0, es);

    return nb;
}

// ---------------------------------------------------------------------------
// rotate: standard std::rotate semantics, type-erased.
//
// Rearranges [first, last) so that *middle becomes the new *first and
// *(middle - 1) becomes the new *(last - 1). Pre: every slot in [first, last)
// is constructed.
//
// Algorithm: block-swap via scratch. Save the smaller block to scratch,
// shift the larger block to its new position (memmove or leaf-loop), then
// restore scratch to the other end.
//
// For trivially-relocatable + default-lifecycle T: pure byte ops. Scratch
// allocation uses ::malloc for blocks > 256 bytes (the common insert-by-one
// case has a 1-element block so the stack alternative covers it).
// ---------------------------------------------------------------------------
void rotate(const type_ops* ops, const storage_ops* sops, void* st_ctx,
            void* first, void* middle, void* last,
            ptrdiff_t size_delta_bytes) {
    auto* f = static_cast<unsigned char*>(first);
    auto* m = static_cast<unsigned char*>(middle);
    auto* l = static_cast<unsigned char*>(last);
    if (f > m || m > l) __builtin_trap();
    if (f != m && m != l) {
        const size_t left  = static_cast<size_t>(m - f);
        const size_t right = static_cast<size_t>(l - m);
        const size_t small = left <= right ? left : right;

        constexpr size_t kStack = 256;
        alignas(::max_align_t) unsigned char stack_scratch[kStack];
        void* scratch = small <= kStack ? static_cast<void*>(stack_scratch)
                                        : ::malloc(small);

        if (triv_reloc(ops)) {
            if (left <= right) {
                __builtin_memcpy(scratch, f, left);
                __builtin_memmove(f, m, right);
                __builtin_memcpy(f + right, scratch, left);
            } else {
                __builtin_memcpy(scratch, m, right);
                __builtin_memmove(f + right, f, left);
                __builtin_memcpy(f, scratch, right);
            }
        } else {
            void* el_ctx = fetch_ec(ops, sops, st_ctx);
            if (left <= right) {
                relocate_block    (ops, el_ctx, scratch, f, left);
                relocate_block    (ops, el_ctx, f, m, right);
                relocate_block    (ops, el_ctx, f + right, scratch, left);
            } else {
                relocate_block    (ops, el_ctx, scratch, m, right);
                relocate_block_rev(ops, el_ctx, f + right, f, left);
                relocate_block    (ops, el_ctx, f, scratch, right);
            }
        }

        if (small > kStack) ::free(scratch);
    }

    if (size_delta_bytes != 0) {
        if (size_delta_bytes < 0) {
            unsigned char* db = l + size_delta_bytes;
            if (!triv_destroy(ops))
                destroy_range(ops, sops, st_ctx, db, l);
        }
        byte_span data = sops->data(ops, st_ctx);
        sops->set_size(ops, st_ctx, data.size + size_delta_bytes);
    }
}

// ---------------------------------------------------------------------------
// callable_ops lifecycle cores (stage 3 of notes/cores-design.md §3.4).
//
// These three helpers carry the small-vs-heap discrimination + the trivial
// fast path for std::function<Sig>'s value_func<Sig>, so the wrapper body
// has one call per lifecycle modifier. They are TYPELESS — keyed only by
// the signature-INDEPENDENT prefix of callable_ops<Sig> (callable_ops_base).
//
// Storage convention (matches bits/value_ops.h):
//   small (f_is_small): held object lives in place at &buf[0..size).
//   large (!f_is_small): held object lives on the heap; pointer cached in
//                        *(void**)&buf[0].
//
// The trivial fast paths read off ops->flags exactly the same way the
// container cores read type_ops->flags — except there is no allocator-
// default-life bit to AND in (none of these wrappers thread an allocator
// through their per-T leaves), so a triv_X flag alone authorises the byte
// path.
// ---------------------------------------------------------------------------

namespace {

// In bits/value_ops.h the small-buffer size is hard-coded as
// 3 * sizeof(void*). Mirror it here for the heap-vs-inline ::operator new
// calls; static_assert in value_ops.h pins the layout match.
constexpr size_t kCallableSmallBufBytes = 3 * sizeof(void*);

inline void* heap_ptr_of(void* buf) {
    return *static_cast<void**>(buf);
}
inline const void* heap_ptr_of(const void* buf) {
    return *static_cast<void* const*>(buf);
}
inline void store_heap_ptr(void* buf, void* p) {
    *static_cast<void**>(buf) = p;
}

} // anonymous

void func_copy_into(const callable_ops_base* ops, void* dst_buf, const void* src_buf) {
    if (ops->flags & vo_f_is_small) {
        // Both wrappers' inline buffers are 3*sizeof(void*) bytes,
        // void*-aligned. Construct the destination object directly inside
        // dst_buf — there is no heap allocation either way.
        if (ops->flags & vo_f_triv_copy) {
            __builtin_memcpy(dst_buf, src_buf, ops->size);
        } else {
            ops->copy_construct(dst_buf, src_buf);
        }
    } else {
        // Heap-owned: allocate a fresh object on the heap and copy-construct
        // it from *heap_ptr_of(src_buf). Aligned allocation when the
        // alignment exceeds the default new alignment.
        const size_t sz = ops->size;
        const size_t al = ops->align;
        void* p;
        if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
            p = ::operator new(sz, std::align_val_t{al});
        } else {
            p = ::operator new(sz);
        }
        const void* src_obj = heap_ptr_of(src_buf);
        if (ops->flags & vo_f_triv_copy) {
            __builtin_memcpy(p, src_obj, sz);
        } else {
            ops->copy_construct(p, src_obj);
        }
        store_heap_ptr(dst_buf, p);
    }
}

void func_move_into(const callable_ops_base* ops, void* dst_buf, void* src_buf) {
    if (ops->flags & vo_f_is_small) {
        // Inline: move the bytes (or invoke the per-F move leaf) into
        // dst_buf. The source object remains constructed at src_buf; the
        // wrapper is expected to destroy it via func_destroy_held after.
        if (ops->flags & vo_f_triv_reloc) {
            __builtin_memcpy(dst_buf, src_buf, ops->size);
        } else {
            ops->move_construct(dst_buf, src_buf);
        }
    } else {
        // Heap-owned: pointer transfer. No leaf call (the heap object is
        // not moved, only its owning slot rotates). Clear the source slot
        // so its wrapper's destructor doesn't double-free.
        void* p = heap_ptr_of(src_buf);
        store_heap_ptr(dst_buf, p);
        store_heap_ptr(src_buf, nullptr);
    }
}

void func_destroy_held(const callable_ops_base* ops, void* buf) {
    if (ops->flags & vo_f_is_small) {
        // Inline: just run the dtor leaf (skip if trivial).
        if (!(ops->flags & vo_f_triv_destroy))
            ops->destroy(buf);
    } else {
        // Heap-owned: run the dtor on the heap object then release.
        void* p = heap_ptr_of(buf);
        if (!p) return;  // post-move zeroing — nothing to release.
        if (!(ops->flags & vo_f_triv_destroy))
            ops->destroy(p);
        const size_t al = ops->align;
        if (al > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
            ::operator delete(p, std::align_val_t{al});
        } else {
            ::operator delete(p);
        }
        store_heap_ptr(buf, nullptr);
    }
}

// ===========================================================================
// Segmented (deque) cores. The chain walks via segment_map_ops::next_segment.
// At this layer, an iterator is a (segment_base, in-segment byte pointer)
// pair carried as two void*s.
// ===========================================================================

static inline void* seg_fetch_ec(const type_ops* ops,
                                 const segment_map_ops* mops, void* st_ctx) {
    return (ops->flags & f_alloc_ctx) ? mops->alloc_ctx(st_ctx) : nullptr;
}

// Number of "next" segment hops between two segments along the live chain,
// counted by walking. Used only for assertion-style monotonicity checks; the
// cores themselves walk and don't need a count.
static size_t seg_hop_count(const segment_map_ops* mops, void* st_ctx,
                            void* from_seg, void* to_seg) {
    size_t n = 0;
    void* s = from_seg;
    while (s && s != to_seg) {
        s = mops->next_segment(st_ctx, s);
        ++n;
        if (n > (size_t)-1 / 2) __builtin_trap(); // runaway: chain malformed
    }
    return n;
}

void segmented_destroy(const type_ops* ops, const segment_map_ops* mops,
                       void* st_ctx,
                       void* first_seg, void* first_ptr,
                       void* last_seg,  void* last_ptr) {
    if (first_seg == last_seg) {
        if (first_ptr > last_ptr) __builtin_trap();
        if (triv_destroy(ops)) return;
        void* ec = seg_fetch_ec(ops, mops, st_ctx);
        const size_t sz = ops->size;
        auto* p = static_cast<unsigned char*>(last_ptr);
        auto* q = static_cast<unsigned char*>(first_ptr);
        while (p != q) { p -= sz; ops->destroy(ec, p); }
        return;
    }
    if (triv_destroy(ops)) return;

    const size_t seg_bytes = mops->size_bytes(st_ctx);
    const size_t sz = ops->size;
    void* ec = seg_fetch_ec(ops, mops, st_ctx);

    // First segment: [first_ptr, first_seg + seg_bytes)
    auto* fseg = static_cast<unsigned char*>(first_seg);
    auto* fend = fseg + seg_bytes;
    auto* p = fend;
    auto* q = static_cast<unsigned char*>(first_ptr);
    while (p != q) { p -= sz; ops->destroy(ec, p); }

    // Middle segments: full segments [seg, seg + seg_bytes)
    void* s = mops->next_segment(st_ctx, first_seg);
    while (s && s != last_seg) {
        auto* ss = static_cast<unsigned char*>(s);
        auto* ee = ss + seg_bytes;
        auto* pp = ee;
        while (pp != ss) { pp -= sz; ops->destroy(ec, pp); }
        s = mops->next_segment(st_ctx, s);
    }
    if (s != last_seg) __builtin_trap(); // last_seg not reachable from first_seg

    // Last segment: [last_seg, last_ptr)
    auto* lseg = static_cast<unsigned char*>(last_seg);
    auto* lend = static_cast<unsigned char*>(last_ptr);
    auto* pp = lend;
    while (pp != lseg) { pp -= sz; ops->destroy(ec, pp); }
}

// Walk a (seg, ptr) cursor by `bytes` bytes within a chain. Returns the new
// (seg, ptr) by reference. `bytes` must be ≤ total remaining in the chain.
static void seg_advance(const segment_map_ops* mops, void* st_ctx,
                        size_t seg_bytes,
                        void*& seg, unsigned char*& ptr, size_t bytes) {
    auto* sb = static_cast<unsigned char*>(seg);
    size_t in_seg = static_cast<size_t>(ptr - sb);
    size_t avail  = seg_bytes - in_seg;
    while (bytes >= avail && seg) {
        bytes -= avail;
        seg = mops->next_segment(st_ctx, seg);
        sb  = static_cast<unsigned char*>(seg);
        ptr = sb;
        in_seg = 0;
        avail  = seg_bytes;
        if (bytes == 0) return;
    }
    ptr += bytes;
}

// Copy/move-construct from src chain to dst chain. The destination range is
// raw uninitialized memory; on the trivial fast path this is per-segment
// memcpy; else element-by-element.
void segmented_copy(const type_ops* ops,
                    const segment_map_ops* src_mops,
                    const segment_map_ops* dst_mops,
                    void* src_ctx, void* dst_ctx,
                    void* src_first_seg, void* src_first_ptr,
                    void* src_last_seg,  void* src_last_ptr,
                    void* dst_first_seg, void* dst_first_ptr,
                    bool is_copy) {
    const size_t src_seg_bytes = src_mops->size_bytes(src_ctx);
    const size_t dst_seg_bytes = dst_mops->size_bytes(dst_ctx);
    const size_t sz = ops->size;

    const bool triv = is_copy ? triv_copy(ops) : triv_reloc(ops);

    void*  src_seg = src_first_seg;
    auto*  src_ptr = static_cast<unsigned char*>(src_first_ptr);
    void*  dst_seg = dst_first_seg;
    auto*  dst_ptr = static_cast<unsigned char*>(dst_first_ptr);

    void* dst_ec = (ops->flags & f_alloc_ctx) ? dst_mops->alloc_ctx(dst_ctx) : nullptr;

    while (!(src_seg == src_last_seg
             && src_ptr == static_cast<unsigned char*>(src_last_ptr))) {
        auto* sb = static_cast<unsigned char*>(src_seg);
        auto* db = static_cast<unsigned char*>(dst_seg);

        // Bytes available on the source side this run.
        size_t src_avail;
        if (src_seg == src_last_seg)
            src_avail = static_cast<size_t>(
                static_cast<unsigned char*>(src_last_ptr) - src_ptr);
        else
            src_avail = src_seg_bytes - static_cast<size_t>(src_ptr - sb);

        // Bytes available on the destination side this run.
        size_t dst_avail = dst_seg_bytes - static_cast<size_t>(dst_ptr - db);

        size_t run = src_avail < dst_avail ? src_avail : dst_avail;

        if (triv) {
            __builtin_memcpy(dst_ptr, src_ptr, run);
        } else {
            size_t n = run / sz;
            for (size_t i = 0; i < n; ++i, dst_ptr += sz, src_ptr += sz) {
                if (is_copy) ops->copy_construct(dst_ec, dst_ptr, src_ptr);
                else         ops->move_construct(dst_ec, dst_ptr, src_ptr);
            }
            // Trivial branch advances pointers below; non-trivial already did.
            // Skip the post-increment block for the non-trivial path.
            // Step segments if we exhausted them.
            if (src_ptr == sb + src_seg_bytes && src_seg != src_last_seg) {
                src_seg = src_mops->next_segment(src_ctx, src_seg);
                src_ptr = static_cast<unsigned char*>(src_seg);
            }
            if (dst_ptr == db + dst_seg_bytes) {
                dst_seg = dst_mops->next_segment(dst_ctx, dst_seg);
                dst_ptr = static_cast<unsigned char*>(dst_seg);
            }
            continue;
        }
        // Trivial-path pointer advance.
        src_ptr += run;
        dst_ptr += run;
        if (src_ptr == sb + src_seg_bytes && src_seg != src_last_seg) {
            src_seg = src_mops->next_segment(src_ctx, src_seg);
            src_ptr = static_cast<unsigned char*>(src_seg);
        }
        if (dst_ptr == db + dst_seg_bytes) {
            dst_seg = dst_mops->next_segment(dst_ctx, dst_seg);
            dst_ptr = static_cast<unsigned char*>(dst_seg);
        }
    }
}

// Reverse [first, last) walking the segmented chain. Element swap via a
// scratch element on the stack (ops->size ≤ kSwapStack) or ::malloc above.
// The three-reverse algorithm (reverse-first, reverse-second, reverse-all)
// builds the rotate.
static void seg_reverse(const type_ops* ops, const segment_map_ops* mops,
                        void* st_ctx, size_t seg_bytes,
                        void* lo_seg, unsigned char* lo_ptr,
                        void* hi_seg, unsigned char* hi_ptr) {
    const size_t sz = ops->size;
    if (lo_seg == hi_seg && lo_ptr >= hi_ptr) return;

    constexpr size_t kSwapStack = 64;
    alignas(::max_align_t) unsigned char stack_tmp[kSwapStack];
    unsigned char* tmp = (sz <= kSwapStack)
        ? stack_tmp
        : static_cast<unsigned char*>(::malloc(sz));

    if (triv_reloc(ops)) {
        // Two-cursor swap walking forward / backward.
        while (true) {
            // hi cursor is one-past; step it back to point at the last live elt.
            // Step hi back by sz.
            if (hi_ptr == static_cast<unsigned char*>(hi_seg)) {
                // walk back: need previous segment — but we don't have a
                // prev_segment slot. Fall back to scan from first_seg.
                // For deque this shouldn't be exercised on triv_reloc T since
                // we go through the trivial path; build a fallback that scans.
                void* p_seg = lo_seg;
                while (p_seg) {
                    void* n = mops->next_segment(st_ctx, p_seg);
                    if (n == hi_seg) { hi_seg = p_seg; hi_ptr = static_cast<unsigned char*>(p_seg) + seg_bytes; break; }
                    p_seg = n;
                }
                if (hi_ptr == static_cast<unsigned char*>(hi_seg)) {
                    // hi_seg degenerate; nothing to do.
                    break;
                }
            }
            hi_ptr -= sz;
            if (lo_seg == hi_seg && lo_ptr >= hi_ptr) break;
            __builtin_memcpy(tmp,    lo_ptr, sz);
            __builtin_memcpy(lo_ptr, hi_ptr, sz);
            __builtin_memcpy(hi_ptr, tmp,    sz);
            // advance lo
            lo_ptr += sz;
            if (lo_ptr == static_cast<unsigned char*>(lo_seg) + seg_bytes) {
                lo_seg = mops->next_segment(st_ctx, lo_seg);
                lo_ptr = static_cast<unsigned char*>(lo_seg);
            }
            if (lo_seg == hi_seg && lo_ptr >= hi_ptr) break;
        }
    } else {
        void* ec = seg_fetch_ec(ops, mops, st_ctx);
        while (true) {
            if (hi_ptr == static_cast<unsigned char*>(hi_seg)) {
                void* p_seg = lo_seg;
                while (p_seg) {
                    void* n = mops->next_segment(st_ctx, p_seg);
                    if (n == hi_seg) { hi_seg = p_seg; hi_ptr = static_cast<unsigned char*>(p_seg) + seg_bytes; break; }
                    p_seg = n;
                }
                if (hi_ptr == static_cast<unsigned char*>(hi_seg)) break;
            }
            hi_ptr -= sz;
            if (lo_seg == hi_seg && lo_ptr >= hi_ptr) break;
            // swap lo and hi using ops->swap if present, else use move-thru-tmp
            if (ops->swap) {
                ops->swap(lo_ptr, hi_ptr);
            } else {
                ops->move_construct(ec, tmp,    lo_ptr); ops->destroy(ec, lo_ptr);
                ops->move_construct(ec, lo_ptr, hi_ptr); ops->destroy(ec, hi_ptr);
                ops->move_construct(ec, hi_ptr, tmp);    ops->destroy(ec, tmp);
            }
            lo_ptr += sz;
            if (lo_ptr == static_cast<unsigned char*>(lo_seg) + seg_bytes) {
                lo_seg = mops->next_segment(st_ctx, lo_seg);
                lo_ptr = static_cast<unsigned char*>(lo_seg);
            }
            if (lo_seg == hi_seg && lo_ptr >= hi_ptr) break;
        }
    }

    if (sz > kSwapStack) ::free(tmp);
}

void segmented_rotate(const type_ops* ops, const segment_map_ops* mops,
                      void* st_ctx,
                      void* first_seg,  void* first_ptr,
                      void* middle_seg, void* middle_ptr,
                      void* last_seg,   void* last_ptr,
                      ptrdiff_t size_delta_bytes) {
    const size_t seg_bytes = mops->size_bytes(st_ctx);

    auto eq = [](void* a_seg, void* a_ptr, void* b_seg, void* b_ptr) {
        return a_seg == b_seg && a_ptr == b_ptr;
    };

    if (!eq(first_seg, first_ptr, middle_seg, middle_ptr)
        && !eq(middle_seg, middle_ptr, last_seg, last_ptr)) {
        // Three-reverse rotate.
        seg_reverse(ops, mops, st_ctx, seg_bytes,
                    first_seg,  static_cast<unsigned char*>(first_ptr),
                    middle_seg, static_cast<unsigned char*>(middle_ptr));
        seg_reverse(ops, mops, st_ctx, seg_bytes,
                    middle_seg, static_cast<unsigned char*>(middle_ptr),
                    last_seg,   static_cast<unsigned char*>(last_ptr));
        seg_reverse(ops, mops, st_ctx, seg_bytes,
                    first_seg, static_cast<unsigned char*>(first_ptr),
                    last_seg,  static_cast<unsigned char*>(last_ptr));
    }

    if (size_delta_bytes < 0) {
        // Destroy the trailing |delta| bytes from the back.
        size_t drop = static_cast<size_t>(-size_delta_bytes);
        // Walk backward from last by drop bytes; since segment_map_ops has no
        // prev_segment, walk forward from first to find the drop-point.
        // But we already have `first` as the start of the rotated range; the
        // simpler computation is: total range bytes = (last - first), drop-point
        // is at total - drop from the start.
        // We can ask locate for a byte index, but locate is from the deque's
        // logical start, not from `first`. Walk instead.
        // Compute total live bytes in [first, last) by walking segments.
        size_t total = 0;
        if (first_seg == last_seg) {
            total = static_cast<size_t>(
                static_cast<unsigned char*>(last_ptr) -
                static_cast<unsigned char*>(first_ptr));
        } else {
            total = seg_bytes - static_cast<size_t>(
                static_cast<unsigned char*>(first_ptr) -
                static_cast<unsigned char*>(first_seg));
            void* s = mops->next_segment(st_ctx, first_seg);
            while (s && s != last_seg) {
                total += seg_bytes;
                s = mops->next_segment(st_ctx, s);
            }
            total += static_cast<size_t>(
                static_cast<unsigned char*>(last_ptr) -
                static_cast<unsigned char*>(last_seg));
        }
        if (drop > total) __builtin_trap();
        size_t keep = total - drop;
        void* drop_seg = first_seg;
        unsigned char* drop_ptr = static_cast<unsigned char*>(first_ptr);
        seg_advance(mops, st_ctx, seg_bytes, drop_seg, drop_ptr, keep);
        if (!triv_destroy(ops))
            segmented_destroy(ops, mops, st_ctx,
                              drop_seg, drop_ptr,
                              last_seg, last_ptr);
    }
}

// ---------------------------------------------------------------------------
// equal_bytes / find_byte — Stage 5 algorithm cores.
//
// Trivial fast paths collapse to memcmp/memchr. Non-trivial T (where the
// caller passes a type_ops whose `equal` leaf is set) walks element-wise.
// Used by `std::equal`, `std::find`, `vector::operator==`.
// ---------------------------------------------------------------------------

bool equal_bytes_triv(size_t /*element_size*/, const void* a, const void* b,
                      size_t n_bytes) {
    // The bytewise gate is up to the caller: this body assumes T's `==` is
    // equivalent to memcmp on sizeof(T) bytes. (element_size is unused on
    // the trivial path; the count is already in bytes.)
    if (n_bytes == 0) return true;
    return __builtin_memcmp(a, b, n_bytes) == 0;
}

const void* find_byte_triv(size_t element_size, const void* begin,
                           const void* end, const void* needle) {
    auto p = static_cast<const unsigned char*>(begin);
    auto e = static_cast<const unsigned char*>(end);
    if (p > e) __builtin_trap();
    const size_t n_bytes = static_cast<size_t>(e - p);
    if (n_bytes == 0) return e;
    if (element_size == 0 || (n_bytes % element_size) != 0) __builtin_trap();
    if (element_size == 1) {
        const unsigned char nv = *static_cast<const unsigned char*>(needle);
        const void* hit = __builtin_memchr(p, nv, n_bytes);
        return hit ? hit : e;
    }
    const size_t n = n_bytes / element_size;
    for (size_t i = 0; i < n; ++i, p += element_size)
        if (__builtin_memcmp(p, needle, element_size) == 0) return p;
    return e;
}

bool equal_bytes(const type_ops* ops, const void* a, const void* b,
                 size_t n_bytes) {
    if (n_bytes == 0) return true;
    // Trivial fast path: when the consumer routed through here on the strength
    // of "T's == is bytewise" (the caller's gate, not the table's), tops->equal
    // is null and we just memcmp. Body shared with equal_bytes_triv.
    if (ops->equal == nullptr)
        return equal_bytes_triv(ops->size, a, b, n_bytes);
    const size_t sz = ops->size;
    if (sz == 0 || (n_bytes % sz) != 0) __builtin_trap();
    auto p = static_cast<const unsigned char*>(a);
    auto q = static_cast<const unsigned char*>(b);
    const size_t n = n_bytes / sz;
    for (size_t i = 0; i < n; ++i, p += sz, q += sz)
        if (!ops->equal(p, q)) return false;
    return true;
}

const void* find_byte(const type_ops* ops, const void* begin, const void* end,
                      const void* needle) {
    if (ops->equal == nullptr)
        return find_byte_triv(ops->size, begin, end, needle);
    auto p = static_cast<const unsigned char*>(begin);
    auto e = static_cast<const unsigned char*>(end);
    if (p > e) __builtin_trap();
    const size_t n_bytes = static_cast<size_t>(e - p);
    if (n_bytes == 0) return e;
    const size_t sz = ops->size;
    if (sz == 0 || (n_bytes % sz) != 0) __builtin_trap();
    const size_t n = n_bytes / sz;
    for (size_t i = 0; i < n; ++i, p += sz)
        if (ops->equal(p, needle)) return p;
    return e;
}

// ---------------------------------------------------------------------------
// Pin sum_ops's locally-duplicated trivial-fast-path bits to the canonical
// ops_flags values, so any future skew is a load-bearing build break. The
// duplication exists only because bits/sum_ops.h cannot include type_ops.h
// (cycle through <memory> → <iterator> → <variant> → <bits/sum_ops.h>).
static_assert(sum_f_triv_destroy == static_cast<unsigned>(f_triv_destroy));
static_assert(sum_f_triv_copy    == static_cast<unsigned>(f_triv_copy));
static_assert(sum_f_triv_reloc   == static_cast<unsigned>(f_triv_reloc));

// ---------------------------------------------------------------------------
// sum_ops cores — variant lifecycle. The fmatrix-per-flavor that visitation
// builds for destroy / copy-construct / move-construct collapses into one
// table lookup plus a leaf call. Trivial fast paths short-circuit:
//   sum_destroy on a fully-triv-destroy variant returns immediately;
//   sum_copy_construct on a fully-triv-copy variant memcpys the union storage;
//   sum_move_construct uses the SAME triv_copy bit (NOT triv_reloc) — see
//   the note on the function for why.
// Caller owns the discriminator (index field): the cores touch payload only.
// ---------------------------------------------------------------------------
void sum_destroy(const sum_ops* s, void* self, size_t index) {
    if (index >= s->n_alternatives) __builtin_trap();
    if (sum_triv_destroy(s)) return;
    const alt_ops& a = s->alts[index];
    // A non-trivial sum may still have individual trivial alternatives whose
    // destroy slot is null — skip those without dispatching.
    if (a.destroy)
        a.destroy(static_cast<unsigned char*>(self) + a.offset);
}

void sum_copy_construct(const sum_ops* s, void* dst, const void* src,
                        size_t src_index) {
    if (src_index >= s->n_alternatives) __builtin_trap();
    if (sum_triv_copy(s)) {
        __builtin_memcpy(dst, src, s->size);
        return;
    }
    const alt_ops& a = s->alts[src_index];
    if (a.copy_construct) {
        a.copy_construct(static_cast<unsigned char*>(dst) + a.offset,
                         static_cast<const unsigned char*>(src) + a.offset);
    } else {
        // Trivial alt inside a non-trivial sum: memcpy the alt payload. The
        // sum's `size` covers the whole union, so we copy that span and let
        // the inactive bytes ride — they are unobserved (union semantics).
        __builtin_memcpy(static_cast<unsigned char*>(dst) + a.offset,
                         static_cast<const unsigned char*>(src) + a.offset,
                         s->size - a.offset);
    }
}

void sum_move_construct(const sum_ops* s, void* dst, void* src,
                        size_t src_index) {
    if (src_index >= s->n_alternatives) __builtin_trap();
    // Move-construct semantics here are "construct dst from std::move(*src),
    // leaving src in a valid moved-from state that the caller will destroy
    // later." That's variant's pattern, not the relocate-and-destroy pattern
    // the contiguous-storage cores use. The trivially-copyable bit is the
    // right fast path (bit-copy preserves both objects); triv_reloc would be
    // wrong because for e.g. std::string two memcpy'd copies would share the
    // heap buffer and double-free at scope exit.
    if (sum_triv_copy(s)) {
        __builtin_memcpy(dst, src, s->size);
        return;
    }
    const alt_ops& a = s->alts[src_index];
    if (a.move_construct) {
        a.move_construct(static_cast<unsigned char*>(dst) + a.offset,
                         static_cast<unsigned char*>(src) + a.offset);
    } else {
        // Trivially-copyable alt inside a non-trivially-copyable sum: a
        // memcpy of the alt's payload is safe (the type has a trivial copy
        // ctor by definition).
        __builtin_memcpy(static_cast<unsigned char*>(dst) + a.offset,
                         static_cast<unsigned char*>(src) + a.offset,
                         s->size - a.offset);
    }
}

// ---------------------------------------------------------------------------
// Stage 8 cold-corner cores (notes/cores-design.md §3.9).
// ---------------------------------------------------------------------------

// valarray<T> uses std::allocator<T> only — no allocator ctx, no leaf
// dispatch through storage_ops. Direct ::operator new / ::operator delete
// (which valarray reaches via std::allocator<T>().{allocate,deallocate})
// is the byte-vocab for the heap; the core mirrors it.
void* valarray_resize_exact(const type_ops* ops, void* old_begin,
                            size_t old_n, size_t new_n,
                            const void* src_value) {
    const size_t sz = ops->size;

    // 1. Destroy and deallocate the old buffer.
    if (old_begin != nullptr) {
        if (!triv_destroy(ops)) {
            auto* p = static_cast<unsigned char*>(old_begin) + old_n * sz;
            auto* q = static_cast<unsigned char*>(old_begin);
            while (p != q) {
                p -= sz;
                ops->destroy(nullptr, p);
            }
        }
        ::operator delete(old_begin);
    }

    if (new_n == 0) return nullptr;

    // 2. Allocate the new buffer.
    void* nb = ::operator new(new_n * sz);

    // 3. Broadcast src_value into all new_n slots.
    auto* p = static_cast<unsigned char*>(nb);
    if (triv_copy(ops)) {
        for (size_t i = 0; i < new_n; ++i, p += sz)
            __builtin_memcpy(p, src_value, sz);
    } else {
        for (size_t i = 0; i < new_n; ++i, p += sz)
            ops->copy_construct(nullptr, p, src_value);
    }
    return nb;
}

// path_lexical_normal_bytes — single state-machine walk over a POSIX path.
//
// The algorithm mirrors the in-class lexically_normal (filesystem path
// algorithm from libc++) but operates entirely on bytes: no path objects,
// no std::vector<std::pair<std::string, ...>>, no path concatenation. It
// scans `in` and emits canonicalized components into `out`.
//
// State per component: we track the kinds (Filename, RootSep, DotDot) of
// the components currently in `out` via byte indices stored in a small
// scratch ring; for the typical depth the stack alloca suffices and
// arbitrarily deep paths fall back to ::malloc.
//
// Component kinds we observe in the input stream:
//   - '/' (root separator at the leading position)
//   - '..'                                 (dot-dot)
//   - '.'                                  (dot — skipped)
//   - '' between two separators            (collapsed)
//   - any other non-empty run              (filename)
size_t path_lexical_normal_bytes(const char* in, size_t in_n,
                                 char* out, size_t out_cap) {
    // [fs.path.generic]p6.1: empty input -> empty output.
    if (in_n == 0) return 0;

    // Scratch: starting byte offset within `out` of each emitted component,
    // plus a kind byte (0=RootSep, 1=Filename, 2=DotDot). Per emission we
    // push; per dot-dot collapse we pop. Depth grows with the input
    // component count; 64 entries cover the typical case on the stack and
    // we ::malloc beyond that.
    constexpr size_t kStack = 64;
    struct entry { size_t off; unsigned char kind; };
    entry stack_buf[kStack];
    entry* parts = stack_buf;
    size_t parts_cap = kStack;
    size_t parts_n = 0;

    auto reserve_parts = [&](size_t need) {
        if (need <= parts_cap) return;
        size_t new_cap = parts_cap * 2;
        while (new_cap < need) new_cap *= 2;
        entry* nb = static_cast<entry*>(::malloc(new_cap * sizeof(entry)));
        for (size_t i = 0; i < parts_n; ++i) nb[i] = parts[i];
        if (parts != stack_buf) ::free(parts);
        parts = nb;
        parts_cap = new_cap;
    };

    // Walk the input; emit components into `out` (sized writes only when
    // we are within out_cap, but always advance `out_len` for the
    // measurement-pass return).
    size_t out_len = 0;
    bool maybe_need_trailing_sep = false;

    auto push_byte = [&](char c) {
        if (out_len < out_cap) out[out_len] = c;
        ++out_len;
    };
    auto push_bytes = [&](const char* s, size_t n) {
        for (size_t i = 0; i < n; ++i) push_byte(s[i]);
    };
    auto pop_to = [&](size_t off) { out_len = off; };

    enum kind : unsigned char { K_RootSep = 0, K_Filename = 1, K_DotDot = 2 };

    auto emit_root_sep = [&]() {
        reserve_parts(parts_n + 1);
        parts[parts_n++] = entry{out_len, K_RootSep};
        push_byte('/');
        maybe_need_trailing_sep = false;
    };
    auto emit_filename = [&](const char* s, size_t n) {
        reserve_parts(parts_n + 1);
        // Insert preferred separator between parts unless we're appending
        // immediately after a RootSep.
        if (parts_n > 0 && parts[parts_n - 1].kind != K_RootSep)
            push_byte('/');
        parts[parts_n++] = entry{out_len, K_Filename};
        push_bytes(s, n);
        maybe_need_trailing_sep = false;
    };
    auto emit_dotdot = [&]() {
        reserve_parts(parts_n + 1);
        if (parts_n > 0 && parts[parts_n - 1].kind != K_RootSep)
            push_byte('/');
        parts[parts_n++] = entry{out_len, K_DotDot};
        push_byte('.');
        push_byte('.');
        maybe_need_trailing_sep = false;
    };

    // Leading run of '/' is a single RootSep (POSIX-only: no root name).
    size_t i = 0;
    if (i < in_n && in[i] == '/') {
        emit_root_sep();
        while (i < in_n && in[i] == '/') ++i;
    }

    while (i < in_n) {
        // Skip duplicate separators between components.
        if (in[i] == '/') {
            // Trailing-separator hint applies if we're at the end of input.
            maybe_need_trailing_sep = true;
            ++i;
            continue;
        }
        // Consume one component [comp_start, i).
        size_t comp_start = i;
        while (i < in_n && in[i] != '/') ++i;
        size_t comp_len = i - comp_start;
        const char* comp = in + comp_start;

        if (comp_len == 1 && comp[0] == '.') {
            // Dot: skip; remember trailing-sep intent.
            maybe_need_trailing_sep = true;
        } else if (comp_len == 2 && comp[0] == '.' && comp[1] == '.') {
            // Dot-dot: collapse against a preceding Filename, drop above
            // RootSep, otherwise emit literally.
            unsigned char last = parts_n ? parts[parts_n - 1].kind : 255;
            if (last == K_Filename) {
                // Pop the preceding Filename; rewind `out` to its offset,
                // trimming the leading '/' we inserted before it (when
                // there was one — i.e. when the part before it wasn't
                // a RootSep).
                size_t off = parts[parts_n - 1].off;
                bool had_sep =
                    parts_n >= 2 && parts[parts_n - 2].kind != K_RootSep;
                pop_to(had_sep ? off - 1 : off);
                --parts_n;
                maybe_need_trailing_sep = true;
            } else if (last == K_RootSep) {
                // .. above root: drop silently (POSIX root has no parent).
                maybe_need_trailing_sep = false;
            } else {
                emit_dotdot();
            }
        } else {
            emit_filename(comp, comp_len);
        }
    }

    // [fs.path.generic]p6.8: empty -> ".".
    if (parts_n == 0) {
        push_byte('.');
        if (parts != stack_buf) ::free(parts);
        return out_len;
    }

    // [fs.path.generic]p6.7: if the last filename is dot-dot, remove any
    // trailing directory-separator.
    bool last_is_filename =
        parts[parts_n - 1].kind == K_Filename;
    if (maybe_need_trailing_sep && last_is_filename) {
        // Trailing separator (the "/" before the implicit empty filename).
        push_byte('/');
    }

    if (parts != stack_buf) ::free(parts);
    return out_len;
}

} // namespace detail
} // namespace std

// ---------------------------------------------------------------------------
// std::nothrow
// ---------------------------------------------------------------------------
namespace std {
    const nothrow_t nothrow{};
} // namespace std

// ---------------------------------------------------------------------------
// std::new_handler / set_new_handler / get_new_handler
//
// An atomic pointer to the current new-handler (initially nullptr).
// ---------------------------------------------------------------------------
namespace std {

namespace {
    // _Atomic-style storage using __atomic builtins.
    new_handler g_new_handler = nullptr;
} // anonymous

new_handler set_new_handler(new_handler h) noexcept {
    return __atomic_exchange_n(&g_new_handler, h, __ATOMIC_SEQ_CST);
}

new_handler get_new_handler() noexcept {
    return __atomic_load_n(&g_new_handler, __ATOMIC_SEQ_CST);
}

} // namespace std

// ---------------------------------------------------------------------------
// std::terminate_handler / set_terminate / get_terminate / terminate
// ---------------------------------------------------------------------------
namespace std {

namespace {
    terminate_handler g_terminate_handler = nullptr;
} // anonymous

terminate_handler set_terminate(terminate_handler h) noexcept {
    return __atomic_exchange_n(&g_terminate_handler, h, __ATOMIC_SEQ_CST);
}

terminate_handler get_terminate() noexcept {
    return __atomic_load_n(&g_terminate_handler, __ATOMIC_SEQ_CST);
}

// terminate(): call the installed handler (if any), then abort.
// Must not return — declared [[noreturn]] in <exception>.
[[noreturn]] void terminate() noexcept {
    terminate_handler h = __atomic_load_n(&g_terminate_handler, __ATOMIC_SEQ_CST);
    if (h) {
        h(); // handler must not return; if it does, fall through to abort
    }
    ::abort();
    __builtin_unreachable();
}

} // namespace std

// ---------------------------------------------------------------------------
// Internal helper: new-handler loop.
//
// Called when malloc/aligned_alloc returns nullptr.  Repeatedly invokes the
// new-handler (if any); each time we re-try the allocation.  If no handler
// is installed (or the handler keeps failing), we call std::terminate()
// because we cannot throw std::bad_alloc under -fno-exceptions.
// ---------------------------------------------------------------------------
namespace {

// Try to allocate `size` bytes (unaligned).  Returns non-null or terminates.
[[nodiscard]] void* new_loop(std::size_t size) noexcept {
    for (;;) {
        void* p = ::malloc(size == 0 ? 1 : size);
        if (p) return p;
        std::new_handler h = std::get_new_handler();
        if (!h) std::terminate();
        h(); // handler may free memory or install a different handler
    }
}

// Try to allocate `size` bytes with alignment `align`.
// Returns non-null or terminates.
[[nodiscard]] void* new_loop_aligned(std::size_t size, std::size_t align) noexcept {
    for (;;) {
        // posix_memalign requires alignment >= sizeof(void*) and power-of-two.
        // aligned_alloc requires size to be a multiple of alignment.
        // Use posix_memalign for simplicity; it works on glibc and can be
        // freed with free().
        if (size == 0) size = 1;
        // Round size up to multiple of alignment so aligned_alloc is happy.
        // posix_memalign has no such requirement, but be defensive.
        void* p = nullptr;
        int rc = ::posix_memalign(&p, align < sizeof(void*) ? sizeof(void*) : align, size);
        if (rc == 0 && p) return p;
        std::new_handler h = std::get_new_handler();
        if (!h) std::terminate();
        h();
    }
}

} // anonymous namespace
// Replaceable allocation functions are WEAK: a test TU that defines its own
// operator new/delete (libc++'s count_new.h instrumentation) overrides these
// at link time, per ELF replaceable-function practice (glibc does the same).
// Without test definitions the weak ones are used.
#define LIBCIS_REPLACEABLE __attribute__((__weak__))

// ---------------------------------------------------------------------------
// operator new / new[]  (scalar and array)
// ---------------------------------------------------------------------------

// Plain scalar new — may loop through new_handler; terminates if OOM
// (cannot throw bad_alloc under -fno-exceptions — comment documents intent).
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new(std::size_t size) {
    return new_loop(size);
}

// Aligned scalar new
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new(std::size_t size, std::align_val_t alignment) {
    return new_loop_aligned(size, static_cast<std::size_t>(alignment));
}

// Nothrow scalar new — returns nullptr on failure instead of terminating.
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
    void* p = ::malloc(size == 0 ? 1 : size);
    if (p) return p;
    // Try new-handler once per iteration, but return nullptr rather than abort.
    for (;;) {
        std::new_handler h = std::get_new_handler();
        if (!h) return nullptr;
        h();
        p = ::malloc(size == 0 ? 1 : size);
        if (p) return p;
    }
}

// Aligned nothrow scalar new
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new(std::size_t size, std::align_val_t alignment,
                                  const std::nothrow_t&) noexcept {
    std::size_t align = static_cast<std::size_t>(alignment);
    if (align < sizeof(void*)) align = sizeof(void*);
    if (size == 0) size = 1;
    void* p = nullptr;
    if (::posix_memalign(&p, align, size) == 0 && p) return p;
    for (;;) {
        std::new_handler h = std::get_new_handler();
        if (!h) return nullptr;
        h();
        p = nullptr;
        if (::posix_memalign(&p, align, size) == 0 && p) return p;
    }
}

// Array new delegates to scalar new
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new[](std::size_t size) {
    return ::operator new(size);
}

[[nodiscard]] LIBCIS_REPLACEABLE void* operator new[](std::size_t size, std::align_val_t alignment) {
    return ::operator new(size, alignment);
}

[[nodiscard]] LIBCIS_REPLACEABLE void* operator new[](std::size_t size, const std::nothrow_t& nt) noexcept {
    return ::operator new(size, nt);
}

[[nodiscard]] LIBCIS_REPLACEABLE void* operator new[](std::size_t size, std::align_val_t alignment,
                                    const std::nothrow_t& nt) noexcept {
    return ::operator new(size, alignment, nt);
}

// ---------------------------------------------------------------------------
// operator delete / delete[]
//
// On glibc, memory allocated with posix_memalign() is freed with free().
// ---------------------------------------------------------------------------

LIBCIS_REPLACEABLE void operator delete(void* ptr) noexcept {
    ::free(ptr);
}

// The default sized/nothrow forms MUST forward to the replaceable base form
// ([new.delete.single]): a program replacing only operator delete(void*) --
// e.g. the suite's count_new.h -- must still intercept these deallocations.
LIBCIS_REPLACEABLE void operator delete(void* ptr, std::size_t /*size*/) noexcept {
    ::operator delete(ptr);
}

LIBCIS_REPLACEABLE void operator delete(void* ptr, std::align_val_t /*alignment*/) noexcept {
    ::free(ptr);
}

LIBCIS_REPLACEABLE void operator delete(void* ptr, std::size_t /*size*/,
                     std::align_val_t alignment) noexcept {
    ::operator delete(ptr, alignment);
}

LIBCIS_REPLACEABLE void operator delete(void* ptr, const std::nothrow_t&) noexcept {
    ::operator delete(ptr);
}

LIBCIS_REPLACEABLE void operator delete(void* ptr, std::align_val_t alignment,
                     const std::nothrow_t&) noexcept {
    ::operator delete(ptr, alignment);
}

// Array delete
LIBCIS_REPLACEABLE void operator delete[](void* ptr) noexcept {
    ::free(ptr);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, std::size_t /*size*/) noexcept {
    ::operator delete[](ptr);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, std::align_val_t /*alignment*/) noexcept {
    ::free(ptr);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, std::size_t /*size*/,
                        std::align_val_t alignment) noexcept {
    ::operator delete[](ptr, alignment);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, const std::nothrow_t&) noexcept {
    ::operator delete[](ptr);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, std::align_val_t alignment,
                        const std::nothrow_t&) noexcept {
    ::operator delete[](ptr, alignment);
}

#undef LIBCIS_REPLACEABLE
// ---------------------------------------------------------------------------
// __cxa_guard_acquire / __cxa_guard_release / __cxa_guard_abort
//
// Itanium C++ ABI §3.3.2: the guard object is a 64-bit value (on 64-bit
// platforms; 32-bit on 32-bit).  Byte 0 (LSB, little-endian) = 1 means
// "already initialized"; we use byte 1 as a spin-lock flag.
//
// Scheme:
//   acquire:
//     1. Atomically load byte 0 with ACQUIRE ordering.
//        If 1 → already done, return 0 (skip init).
//     2. CAS byte 1 from 0 → 1 (acquire the "in-progress" lock).
//        If CAS fails, spin + yield then go to step 1.
//     3. Re-check byte 0 (another thread may have finished while we spun).
//        If 1 → release byte-1 lock, return 0.
//     4. Return 1 → caller must run the initializer.
//   release:
//     Store 1 into byte 0 with RELEASE, then store 0 into byte 1 with RELEASE.
//   abort:
//     Store 0 into byte 1 (no init happened), RELEASE.
//
// The guard object is 8 bytes; we treat it as int64_t but address individual
// bytes via uint8_t* pointers.  All accesses use __atomic_* builtins so GCC
// generates the correct barriers without needing <stdatomic.h>.
// ---------------------------------------------------------------------------
extern "C" {

// The guard object is declared as int64_t by the ABI, but we need byte
// access.  We receive it as a pointer to 64-bit value.
int __cxa_guard_acquire(int64_t* guard_object) {
    uint8_t* initialized   = reinterpret_cast<uint8_t*>(guard_object);     // byte 0
    uint8_t* in_progress   = reinterpret_cast<uint8_t*>(guard_object) + 1; // byte 1

    for (;;) {
        // Fast path: already initialized.
        if (__atomic_load_n(initialized, __ATOMIC_ACQUIRE) != 0) {
            return 0; // skip initialization
        }

        // Try to grab the in-progress lock: CAS 0 → 1.
        uint8_t expected = 0;
        uint8_t desired  = 1;
        if (__atomic_compare_exchange_n(in_progress, &expected, desired,
                                        /*weak=*/0,
                                        __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) {
            // We own the lock.  Re-check byte 0 in case another thread finished
            // while we were spinning before we grabbed the lock.
            if (__atomic_load_n(initialized, __ATOMIC_ACQUIRE) != 0) {
                // Already done by another thread — release lock and return 0.
                __atomic_store_n(in_progress, (uint8_t)0, __ATOMIC_RELEASE);
                return 0;
            }
            return 1; // caller must run the initializer
        }

        // Another thread holds the lock; yield and retry.
        ::sched_yield();
    }
}

void __cxa_guard_release(int64_t* guard_object) {
    uint8_t* initialized = reinterpret_cast<uint8_t*>(guard_object);
    uint8_t* in_progress = reinterpret_cast<uint8_t*>(guard_object) + 1;

    // Mark as initialized (RELEASE so prior writes are visible).
    __atomic_store_n(initialized, (uint8_t)1, __ATOMIC_RELEASE);
    // Release the in-progress lock.
    __atomic_store_n(in_progress, (uint8_t)0, __ATOMIC_RELEASE);
}

void __cxa_guard_abort(int64_t* guard_object) {
    uint8_t* in_progress = reinterpret_cast<uint8_t*>(guard_object) + 1;
    // Initialization failed; release the lock so other threads can retry.
    __atomic_store_n(in_progress, (uint8_t)0, __ATOMIC_RELEASE);
}

// ---------------------------------------------------------------------------
// __cxa_pure_virtual
//
// Called when a pure-virtual function is invoked (undefined behavior, but we
// try to give a useful message before trapping).
// ---------------------------------------------------------------------------
[[noreturn]] void __cxa_pure_virtual() {
    // Use the write() syscall-like approach via fputs equivalent.
    // __builtin_fprintf is not available as a built-in; use write to stderr (fd 2).
    const char msg[] = "pure virtual function called\n";
    // Avoid pulling in stdio; use __builtin_write if available, otherwise trap.
    // On Linux/glibc, fileno(stderr)==2 and write() is in libc.
    extern int write(int, const void*, unsigned long) noexcept __asm__("write");
    write(2, msg, sizeof(msg) - 1);
    __builtin_trap();
    __builtin_unreachable();
}

} // extern "C"

// ---------------------------------------------------------------------------
// std::exception and std::bad_exception out-of-line definitions.
//
// By defining the key function (virtual destructor) here, this TU becomes
// the "key TU" that emits the vtable for these classes.  All other TUs that
// use std::exception get a reference to the vtable defined here.
//
// The declarations in include/exception must NOT carry inline bodies or
// gnu::visibility("default") import annotations — they are just declarations.
// ---------------------------------------------------------------------------
namespace std {

// exception
exception::~exception() noexcept {}
const char* exception::what() const noexcept { return "std::exception"; }

// bad_exception
bad_exception::~bad_exception() noexcept {}
const char* bad_exception::what() const noexcept { return "std::bad_exception"; }

} // namespace std

// ---------------------------------------------------------------------------
// __cxa_thread_atexit: register a thread_local object's destructor.
//
// Emitted by the compiler for `static thread_local T x;` with a non-trivial
// dtor (e.g. <future>'s at-thread-exit registry).  Normally provided by
// libstdc++/libc++abi, which we do NOT link (-nodefaultlibs).  glibc 2.18+
// ships the real machinery as __cxa_thread_atexit_impl; forward to it.
// ---------------------------------------------------------------------------
extern "C" int __cxa_thread_atexit_impl(void (*func)(void*), void* obj, void* dso_handle);

extern "C" int __cxa_thread_atexit(void (*func)(void*), void* obj, void* dso_handle) {
    return __cxa_thread_atexit_impl(func, obj, dso_handle);
}

// ---------------------------------------------------------------------------
// Classic stream-dependent locale facets (num_get/num_put/money*/time*/messages).
//
// These facets are defined in <locale> but reference a COMPLETE std::ios_base,
// which <locale> itself cannot see (the <ios> -> <locale> include cycle leaves
// ios_base incomplete while <locale> is parsed).  So their CLASSIC instances are
// instantiated and registered HERE, in a TU that includes <sstream> first and
// therefore has a complete ios_base.  Registration goes into the same global
// facet registry the header uses (locale::__register_facet); the facet `id`
// objects are shared inline statics, so lookups in any TU resolve correctly.
//
// Reported per the porting contract: this is the only src/support.cpp edit for
// the localization pass.
// ---------------------------------------------------------------------------
#include <sstream>   // pulls <ios>/<streambuf>/<istream>/<ostream> fully, then <locale>
#include <locale>

namespace std {
namespace detail {

// Construct the classic stream facets in STATIC STORAGE (placement-new into
// aligned buffers) rather than with ::operator new.  The libc++ test harness
// replaces the global operator new/delete to count outstanding allocations and
// asserts the count is 0 across locale construction, so these long-lived classic
// facets must not go through operator new.  refs=1 => never destroyed.
template <class Facet, class... Args>
static Facet* make_static_facet(Args&&... args) {
    alignas(Facet) static unsigned char buf[sizeof(Facet)];
    return ::new (static_cast<void*>(buf)) Facet(static_cast<Args&&>(args)...);
}

struct stream_facet_init {
    stream_facet_init() {
        locale::__register_facet(std::num_get<char>::id.__get(),    make_static_facet<std::num_get<char>>(1));
        locale::__register_facet(std::num_get<wchar_t>::id.__get(), make_static_facet<std::num_get<wchar_t>>(1));
        locale::__register_facet(std::num_put<char>::id.__get(),    make_static_facet<std::num_put<char>>(1));
        locale::__register_facet(std::num_put<wchar_t>::id.__get(), make_static_facet<std::num_put<wchar_t>>(1));
        locale::__register_facet(std::time_get<char>::id.__get(),    make_static_facet<std::time_get<char>>(1));
        locale::__register_facet(std::time_get<wchar_t>::id.__get(), make_static_facet<std::time_get<wchar_t>>(1));
        locale::__register_facet(std::time_put<char>::id.__get(),    make_static_facet<std::time_put<char>>(1));
        locale::__register_facet(std::time_put<wchar_t>::id.__get(), make_static_facet<std::time_put<wchar_t>>(1));
        locale::__register_facet(std::money_get<char>::id.__get(),    make_static_facet<std::money_get<char>>(1));
        locale::__register_facet(std::money_get<wchar_t>::id.__get(), make_static_facet<std::money_get<wchar_t>>(1));
        locale::__register_facet(std::money_put<char>::id.__get(),    make_static_facet<std::money_put<char>>(1));
        locale::__register_facet(std::money_put<wchar_t>::id.__get(), make_static_facet<std::money_put<wchar_t>>(1));
        locale::__register_facet(std::moneypunct<char, false>::id.__get(),    make_static_facet<std::moneypunct<char, false>>(1));
        locale::__register_facet(std::moneypunct<char, true>::id.__get(),     make_static_facet<std::moneypunct<char, true>>(1));
        locale::__register_facet(std::moneypunct<wchar_t, false>::id.__get(), make_static_facet<std::moneypunct<wchar_t, false>>(1));
        locale::__register_facet(std::moneypunct<wchar_t, true>::id.__get(),  make_static_facet<std::moneypunct<wchar_t, true>>(1));
        locale::__register_facet(std::messages<char>::id.__get(),    make_static_facet<std::messages<char>>(1));
        locale::__register_facet(std::messages<wchar_t>::id.__get(), make_static_facet<std::messages<wchar_t>>(1));
    }
};
stream_facet_init stream_facet_init_instance;

} // namespace detail
} // namespace std

// ---------------------------------------------------------------------------
// locale::__install_{monetary,time,messages}: install byname facets into a
// named locale's imp.  Defined here (not in <locale>) because constructing the
// byname facets instantiates their ios_base-using virtual bodies, which need a
// complete std::ios_base.
// ---------------------------------------------------------------------------
namespace std {

void locale::__install_monetary(imp* ni, const char* name, bool isC) {
    if (isC) return;
    ni->install(std::moneypunct<char, false>::id.__get(),    new std::moneypunct_byname<char, false>(name));
    ni->install(std::moneypunct<char, true>::id.__get(),     new std::moneypunct_byname<char, true>(name));
    ni->install(std::moneypunct<wchar_t, false>::id.__get(), new std::moneypunct_byname<wchar_t, false>(name));
    ni->install(std::moneypunct<wchar_t, true>::id.__get(),  new std::moneypunct_byname<wchar_t, true>(name));
}
void locale::__install_time(imp* ni, const char* name, bool isC) {
    if (isC) return;
    ni->install(std::time_get<char>::id.__get(),    new std::time_get_byname<char>(name));
    ni->install(std::time_get<wchar_t>::id.__get(), new std::time_get_byname<wchar_t>(name));
    ni->install(std::time_put<char>::id.__get(),    new std::time_put_byname<char>(name));
    ni->install(std::time_put<wchar_t>::id.__get(), new std::time_put_byname<wchar_t>(name));
}
void locale::__install_messages(imp* ni, const char* name, bool isC) {
    if (isC) return;
    ni->install(std::messages<char>::id.__get(),    new std::messages_byname<char>(name));
    ni->install(std::messages<wchar_t>::id.__get(), new std::messages_byname<wchar_t>(name));
}

} // namespace std
