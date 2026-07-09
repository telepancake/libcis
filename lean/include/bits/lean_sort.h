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

// Gate: a raw pointer to a trivially-relocatable, non-const, <=256-byte element.
template<class Iter>
inline constexpr bool lean_sort_ok = false;
template<class T>
inline constexpr bool lean_sort_ok<T*> =
    is_trivially_relocatable_v<T> && !is_const_v<T> && sizeof(T) <= 256;

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

// --- heap primitives (max-heap over [0, n)) -------------------------------

// Sift the element at `start` down into a max-heap of size n. Hole-based: the
// sifted element is held in tmp and only written once at its final slot.
inline void lean_sift_down(char* base, size_t elem, size_t n, size_t start,
                           lean_less_fn less, void* ctx, char* tmp) {
    lean_move_bytes(tmp, lean_elt(base, start, elem), elem);
    size_t root = start;
    for (;;) {
        size_t child = 2 * root + 1;
        if (child >= n) break;
        if (child + 1 < n &&
            less(lean_elt(base, child, elem), lean_elt(base, child + 1, elem), ctx))
            ++child;
        if (!less(tmp, lean_elt(base, child, elem), ctx))  // held >= child: settled
            break;
        lean_move_bytes(lean_elt(base, root, elem), lean_elt(base, child, elem), elem);
        root = child;
    }
    lean_move_bytes(lean_elt(base, root, elem), tmp, elem);
}

// Sift the last element (index n-1) up into the max-heap [0, n-1).
inline void lean_sift_up(char* base, size_t elem, size_t n,
                         lean_less_fn less, void* ctx, char* tmp) {
    if (n < 2) return;
    size_t child = n - 1;
    lean_move_bytes(tmp, lean_elt(base, child, elem), elem);
    while (child > 0) {
        size_t parent = (child - 1) / 2;
        if (!less(lean_elt(base, parent, elem), tmp, ctx))  // parent >= held: settled
            break;
        lean_move_bytes(lean_elt(base, child, elem), lean_elt(base, parent, elem), elem);
        child = parent;
    }
    lean_move_bytes(lean_elt(base, child, elem), tmp, elem);
}

inline void lean_make_heap(char* base, size_t n, size_t elem,
                           lean_less_fn less, void* ctx) {
    if (n < 2) return;
    char tmp[lean_sort_max_elem];
    for (size_t i = n / 2; i-- > 0;)
        lean_sift_down(base, elem, n, i, less, ctx, tmp);
}

inline void lean_push_heap(char* base, size_t n, size_t elem,
                           lean_less_fn less, void* ctx) {
    char tmp[lean_sort_max_elem];
    lean_sift_up(base, elem, n, less, ctx, tmp);
}

inline void lean_pop_heap(char* base, size_t n, size_t elem,
                          lean_less_fn less, void* ctx) {
    if (n < 2) return;
    char tmp[lean_sort_max_elem];
    lean_swap_bytes(base, lean_elt(base, n - 1, elem), elem, tmp);
    lean_sift_down(base, elem, n - 1, 0, less, ctx, tmp);
}

// Pop the whole heap into sorted order. Shared tmp is fine: the swap completes
// before the sift reuses the buffer.
inline void lean_sort_heap_range(char* base, size_t n, size_t elem,
                                 lean_less_fn less, void* ctx, char* tmp) {
    for (size_t i = n; i > 1; --i) {
        lean_swap_bytes(base, lean_elt(base, i - 1, elem), elem, tmp);
        lean_sift_down(base, elem, i - 1, 0, less, ctx, tmp);
    }
}

inline void lean_sort_heap(char* base, size_t n, size_t elem,
                           lean_less_fn less, void* ctx) {
    char tmp[lean_sort_max_elem];
    lean_sort_heap_range(base, n, elem, less, ctx, tmp);
}

// Build a heap then pop it — the introsort depth-limit fallback.
inline void lean_heap_sort_range(char* base, size_t n, size_t elem,
                                 lean_less_fn less, void* ctx, char* tmp) {
    for (size_t i = n / 2; i-- > 0;)
        lean_sift_down(base, elem, n, i, less, ctx, tmp);
    lean_sort_heap_range(base, n, elem, less, ctx, tmp);
}

// --- insertion sort (stable) ----------------------------------------------

inline void lean_insertion_sort(char* base, size_t n, size_t elem,
                                lean_less_fn less, void* ctx, char* tmp) {
    for (size_t i = 1; i < n; ++i) {
        if (less(lean_elt(base, i, elem), lean_elt(base, i - 1, elem), ctx)) {
            lean_move_bytes(tmp, lean_elt(base, i, elem), elem);
            size_t j = i;
            do {
                lean_move_bytes(lean_elt(base, j, elem), lean_elt(base, j - 1, elem), elem);
                --j;
            } while (j > 0 && less(tmp, lean_elt(base, j - 1, elem), ctx));
            lean_move_bytes(lean_elt(base, j, elem), tmp, elem);
        }
    }
}

// --- introsort ------------------------------------------------------------

// Median-of-three pivot into index 0, Hoare partition, final pivot placement.
// Returns the pivot's final index j: [0, j) <= base[j] <= (j, n). Both partitions
// exclude j, so recursion strictly shrinks. base[n-1] >= pivot and base[0] == pivot
// act as the scan sentinels.
inline size_t lean_partition(char* base, size_t n, size_t elem,
                             lean_less_fn less, void* ctx, char* tmp, char* pivot) {
    char* a   = base;
    char* mid = lean_elt(base, n / 2, elem);
    char* last = lean_elt(base, n - 1, elem);
    if (less(mid, a, ctx)) lean_swap_bytes(a, mid, elem, tmp);
    if (less(last, mid, ctx)) {
        lean_swap_bytes(mid, last, elem, tmp);
        if (less(mid, a, ctx)) lean_swap_bytes(a, mid, elem, tmp);
    }
    // a <= mid <= last by value; median is at mid. Move it to index 0.
    lean_swap_bytes(a, mid, elem, tmp);
    lean_move_bytes(pivot, a, elem);

    size_t i = 0, j = n;
    for (;;) {
        do { ++i; } while (i < n && less(lean_elt(base, i, elem), pivot, ctx));
        do { --j; } while (less(pivot, lean_elt(base, j, elem), ctx));
        if (i >= j) break;
        lean_swap_bytes(lean_elt(base, i, elem), lean_elt(base, j, elem), elem, tmp);
    }
    lean_swap_bytes(base, lean_elt(base, j, elem), elem, tmp);
    return j;
}

inline void lean_introsort(char* base, size_t n, size_t elem,
                           lean_less_fn less, void* ctx, size_t depth,
                           char* tmp, char* pivot) {
    const size_t cutoff = 16;
    while (n > cutoff) {
        if (depth == 0) {
            lean_heap_sort_range(base, n, elem, less, ctx, tmp);
            return;
        }
        --depth;
        size_t j = lean_partition(base, n, elem, less, ctx, tmp, pivot);
        size_t leftn  = j;
        size_t rightn = n - j - 1;
        // Recurse into the smaller side, loop on the larger (bounded stack depth).
        if (leftn < rightn) {
            lean_introsort(base, leftn, elem, less, ctx, depth, tmp, pivot);
            base = lean_elt(base, j + 1, elem);
            n = rightn;
        } else {
            lean_introsort(lean_elt(base, j + 1, elem), rightn, elem, less, ctx, depth, tmp, pivot);
            n = leftn;
        }
    }
    lean_insertion_sort(base, n, elem, less, ctx, tmp);
}

inline void lean_sort(char* base, size_t n, size_t elem,
                      lean_less_fn less, void* ctx) {
    if (n < 2) return;
    char tmp[lean_sort_max_elem];
    char pivot[lean_sort_max_elem];
    lean_introsort(base, n, elem, less, ctx, 2 * lean_log2(n), tmp, pivot);
}

// --- partial_sort ---------------------------------------------------------

// Sort the smallest m elements into [0, m); leave (m, n) unspecified. Heap of the
// first m, then push any smaller tail element through it, then pop the heap.
inline void lean_partial_sort(char* base, size_t m, size_t n, size_t elem,
                              lean_less_fn less, void* ctx) {
    if (m == 0) return;
    char tmp[lean_sort_max_elem];
    for (size_t i = m / 2; i-- > 0;)
        lean_sift_down(base, elem, m, i, less, ctx, tmp);
    for (size_t i = m; i < n; ++i) {
        if (less(lean_elt(base, i, elem), base, ctx)) {  // tail element < heap max
            lean_swap_bytes(base, lean_elt(base, i, elem), elem, tmp);
            lean_sift_down(base, elem, m, 0, less, ctx, tmp);
        }
    }
    lean_sort_heap_range(base, m, elem, less, ctx, tmp);
}

// --- nth_element (quickselect) --------------------------------------------

inline void lean_nth_element(char* base, size_t nth, size_t n, size_t elem,
                             lean_less_fn less, void* ctx) {
    if (n < 2 || nth >= n) return;
    char tmp[lean_sort_max_elem];
    char pivot[lean_sort_max_elem];
    size_t depth = 2 * lean_log2(n);
    const size_t cutoff = 16;
    while (n > cutoff) {
        if (depth == 0) {
            lean_heap_sort_range(base, n, elem, less, ctx, tmp);
            return;
        }
        --depth;
        size_t j = lean_partition(base, n, elem, less, ctx, tmp, pivot);
        if (nth == j) return;
        if (nth < j) {
            n = j;
        } else {
            base = lean_elt(base, j + 1, elem);
            nth -= j + 1;
            n -= j + 1;
        }
    }
    lean_insertion_sort(base, n, elem, less, ctx, tmp);
}

// --- stable_sort (bottom-up merge sort) -----------------------------------

// Stable merge of src[left,mid) and src[mid,right) into dst[left,right).
inline void lean_merge_runs(const char* src, char* dst,
                            size_t left, size_t mid, size_t right, size_t elem,
                            lean_less_fn less, void* ctx) {
    size_t i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (less(src + j * elem, src + i * elem, ctx)) {  // right < left: take right
            lean_move_bytes(dst + k * elem, src + j * elem, elem);
            ++j;
        } else {                                           // equal: take left (stable)
            lean_move_bytes(dst + k * elem, src + i * elem, elem);
            ++i;
        }
        ++k;
    }
    while (i < mid)   { lean_move_bytes(dst + k * elem, src + i * elem, elem); ++i; ++k; }
    while (j < right) { lean_move_bytes(dst + k * elem, src + j * elem, elem); ++j; ++k; }
}

inline void lean_stable_sort(char* base, size_t n, size_t elem,
                             lean_less_fn less, void* ctx) {
    if (n < 2) return;
    char tmp[lean_sort_max_elem];
    const size_t run = 16;
    for (size_t i = 0; i < n; i += run) {
        size_t len = n - i < run ? n - i : run;
        lean_insertion_sort(base + i * elem, len, elem, less, ctx, tmp);
    }
    if (n <= run) return;
    char* buf = static_cast<char*>(::malloc(n * elem));
    if (!buf) __builtin_trap();
    char* src = base;
    char* dst = buf;
    for (size_t width = run; width < n; width *= 2) {
        for (size_t i = 0; i < n; i += 2 * width) {
            size_t mid   = i + width   < n ? i + width   : n;
            size_t right = i + 2 * width < n ? i + 2 * width : n;
            lean_merge_runs(src, dst, i, mid, right, elem, less, ctx);
        }
        char* t = src; src = dst; dst = t;
    }
    if (src != base)
        lean_move_bytes(base, src, n * elem);
    ::free(buf);
}

// --- inplace_merge --------------------------------------------------------

// Merge two adjacent sorted runs [0,len1) and [len1,len1+len2) in place, using a
// temporary buffer holding the smaller run. Traps on allocation failure.
inline void lean_inplace_merge(char* base, size_t len1, size_t len2, size_t elem,
                               lean_less_fn less, void* ctx) {
    if (len1 == 0 || len2 == 0) return;
    size_t total = len1 + len2;
    if (len1 <= len2) {
        char* buf = static_cast<char*>(::malloc(len1 * elem));
        if (!buf) __builtin_trap();
        lean_move_bytes(buf, base, len1 * elem);  // stash the left run
        size_t i = 0, j = len1, k = 0;
        while (i < len1 && j < total) {
            if (less(base + j * elem, buf + i * elem, ctx)) {  // right < left
                lean_move_bytes(base + k * elem, base + j * elem, elem); ++j;
            } else {                                            // equal: take left (stable)
                lean_move_bytes(base + k * elem, buf + i * elem, elem); ++i;
            }
            ++k;
        }
        while (i < len1) { lean_move_bytes(base + k * elem, buf + i * elem, elem); ++i; ++k; }
        // remaining right elements are already in place
        ::free(buf);
    } else {
        char* buf = static_cast<char*>(::malloc(len2 * elem));
        if (!buf) __builtin_trap();
        lean_move_bytes(buf, base + len1 * elem, len2 * elem);  // stash the right run
        // Merge from the top down so the in-place left run is never clobbered
        // before it is read. i/j/k are counts remaining; k == i + j is invariant.
        size_t i = len2, j = len1, k = total;
        while (i > 0 && j > 0) {
            if (less(buf + (i - 1) * elem, base + (j - 1) * elem, ctx)) {
                // left tail is the larger: it goes last
                --k; lean_move_bytes(base + k * elem, base + (j - 1) * elem, elem); --j;
            } else {
                // equal or right larger: take right (stable — right after left)
                --k; lean_move_bytes(base + k * elem, buf + (i - 1) * elem, elem); --i;
            }
        }
        while (i > 0) { --k; lean_move_bytes(base + k * elem, buf + (i - 1) * elem, elem); --i; }
        // remaining left elements are already in place
        ::free(buf);
    }
}

} // namespace detail
} // namespace std
