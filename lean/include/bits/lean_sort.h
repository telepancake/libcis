// bits/lean_sort.h — single-instance, type-erased sorting kernels for the lean
// <algorithm> overlay.
//
// The fat <algorithm> entry points (sort, stable_sort, partial_sort,
// nth_element, the heap family, inplace_merge) route contiguous,
// trivially-relocatable, small-element ranges here. The per-type code that
// survives is a two-instruction comparator thunk (lean_less_thunk); the sorting
// logic itself is compiled exactly once per binary (these are non-template
// `inline` functions — the linker keeps one copy — parameterized by an element
// size + a comparator function pointer instead of by <T, Compare>).
//
// GATE (lean_sort_ok, correctness not tuning): the iterator must be a raw
// pointer (libcis's vector/string iterators already ARE pointers, so
// "contiguous" == is_pointer here), the element must be trivially relocatable
// (byte-swapping a non-trivially-relocatable object is UB), and sizeof(element)
// must be <= 256 (the swap/hold scratch is a 256-byte stack buffer). Anything
// else takes the base templated fallback in <algorithm>.
#pragma once
#include <cstddef>
#include <cstdlib>
#include <type_traits>
#include <bits/relocatable.h>

namespace std {
namespace detail {

// The comparator thunk type: casts the two element pointers back to T and
// invokes the user comparator held in ctx.
using lean_less_fn = bool (*)(const void*, const void*, void*);

template<class T, class Compare>
bool lean_less_thunk(const void* a, const void* b, void* ctx) {
    return (*static_cast<Compare*>(ctx))(*static_cast<const T*>(a),
                                         *static_cast<const T*>(b));
}

// Gate: a raw pointer to a trivially-relocatable, non-const, <=256-byte element
// whose alignment does not exceed max_align_t. The kernels hold an element in a
// stack scratch buffer aligned only to max_align_t (see below); an over-aligned
// element (alignof > alignof(max_align_t), e.g. a raw array of an alignas(32)
// type) would be reified from that buffer at a misaligned address — UB. Such
// types take the base templated fallback in <algorithm> instead. In-contract
// container elements always satisfy alignof(T) <= alignof(max_align_t)
// (vector's static_assert), so they still reach the kernel.
template<class Iter>
inline constexpr bool lean_sort_ok = false;
template<class T>
inline constexpr bool lean_sort_ok<T*> =
    is_trivially_relocatable_v<T> && !is_const_v<T> && sizeof(T) <= 256 &&
    alignof(T) <= alignof(max_align_t);

// Max scratch element size handled on the stack. Elements above this never
// reach the kernels (lean_sort_ok caps sizeof at 256).
inline constexpr size_t lean_sort_max_elem = 256;

// floor(log2(n)) for n >= 1.
inline size_t lean_log2(size_t n) {
    return static_cast<size_t>(63 - __builtin_clzll(static_cast<unsigned long long>(n)));
}

// Byte moves. The fixed-size cases let the compiler lower the common element
// sizes to plain register moves instead of a memcpy call.
inline void lean_move_bytes(void* d, const void* s, size_t elem) {
    switch (elem) {
        case 1:  __builtin_memcpy(d, s, 1);  return;
        case 2:  __builtin_memcpy(d, s, 2);  return;
        case 4:  __builtin_memcpy(d, s, 4);  return;
        case 8:  __builtin_memcpy(d, s, 8);  return;
        case 16: __builtin_memcpy(d, s, 16); return;
        default: __builtin_memcpy(d, s, elem); return;
    }
}

inline void lean_swap_bytes(void* a, void* b, size_t elem, char* tmp) {
    lean_move_bytes(tmp, a, elem);
    lean_move_bytes(a, b, elem);
    lean_move_bytes(b, tmp, elem);
}

inline char* lean_elt(char* base, size_t i, size_t elem) { return base + i * elem; }

// ---------------------------------------------------------------------------
// Fat sorting kernels — defined out of line in lean/src/kernels.cpp (one copy
// per system). Declarations only here; the internal leaves (sift/insertion/
// partition/heap-range/merge-runs/introsort) are file-local to that TU. The
// scratch buffers those kernels allocate on the stack are sized by
// lean_sort_max_elem and aligned to max_align_t (see the gate above). Default
// visibility so the references stay bindable to liblean.a / liblean.so even
// under a hidden-visibility include region (see bits/lean_string.h).
// ---------------------------------------------------------------------------
#pragma GCC visibility push(default)

// heap family (max-heap over [0, n)).
void lean_make_heap(char* base, size_t n, size_t elem, lean_less_fn less, void* ctx);
void lean_push_heap(char* base, size_t n, size_t elem, lean_less_fn less, void* ctx);
void lean_pop_heap(char* base, size_t n, size_t elem, lean_less_fn less, void* ctx);
void lean_sort_heap(char* base, size_t n, size_t elem, lean_less_fn less, void* ctx);

// introsort (median-of-three quicksort + heap-sort depth fallback + insertion
// sort cutoff).
void lean_sort(char* base, size_t n, size_t elem, lean_less_fn less, void* ctx);

// Sort the smallest m elements into [0, m); leave (m, n) unspecified.
void lean_partial_sort(char* base, size_t m, size_t n, size_t elem,
                       lean_less_fn less, void* ctx);

// quickselect: after the call base[nth] is the element that belongs there.
void lean_nth_element(char* base, size_t nth, size_t n, size_t elem,
                      lean_less_fn less, void* ctx);

// stable bottom-up merge sort (allocates an n-element scratch buffer; traps on
// allocation failure).
void lean_stable_sort(char* base, size_t n, size_t elem,
                      lean_less_fn less, void* ctx);

// Merge two adjacent sorted runs [0,len1) and [len1,len1+len2) in place, using a
// temporary buffer holding the smaller run. Traps on allocation failure.
void lean_inplace_merge(char* base, size_t len1, size_t len2, size_t elem,
                        lean_less_fn less, void* ctx);

#pragma GCC visibility pop

} // namespace detail
} // namespace std
