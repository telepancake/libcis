// bits/algo_cores.h — non-template byte-level cores for the equality and
// linear-search algorithms.
//
// Two pairs of cores:
//
//   equal_bytes(tops, a, b, n)              — full type_ops-shaped entry.
//   find_byte (tops, b, e, needle)          —   memcmp / memchr fast path
//                                               when tops->equal == nullptr;
//                                               element-walk via tops->equal
//                                               otherwise.
//
//   equal_bytes_triv(sz, a, b, n)           — non-template thin shim for the
//   find_byte_triv (sz, b, e, needle)         "T's == is bytewise" case.
//                                             Same body as the tops form on
//                                             the trivial fast path; spares
//                                             the call site from needing the
//                                             full `type_ops` struct in scope
//                                             (which would import an awkward
//                                             include cycle through <memory>).
//
// The `_triv` shims are what `std::equal`, `std::find`, and `vector::operator==`
// actually call: those algorithms gate on `is_byte_equal_v<T,U>` (defined
// below) before dispatching, so the bytewise fast path is always semantically
// safe. The `tops` form remains the documented Stage 5 ABI for callers that
// need the non-trivial path (e.g. a future ported `std::find` instantiation
// over a deque iterator with a user-defined T).
//
// These cores are runtime-only (constexpr is deliberately not used here). The
// algorithm headers fall back to the typed loop during constant evaluation
// and when the iterators are not raw pointers.
#pragma once
#include <cstddef>
#include <type_traits>

// Forward-decl rather than including <bits/type_ops.h>. The full definition
// of `type_ops` is needed by callers that form `&ops_for<T,A,MASK>`, but the
// declarations here only see an opaque `type_ops*`. This avoids a cycle:
// bits/type_ops.h pulls <memory> which pulls <bits/memory_ranges.h> which
// pulls <algorithm> which pulls THIS header. With pragma-once on type_ops.h
// the recursive include would skip the struct definition before it was
// parsed; downstream lookup of `ops_for` then fails.
namespace std { namespace detail { struct type_ops; } }

namespace std {
namespace detail {

// ---------------------------------------------------------------------------
// type_ops-shaped cores (the documented Stage 5 ABI).
//
// equal_bytes: compare n_bytes worth of element data at `a` and `b`. When
// `tops->equal == nullptr` (the trivial case) this is one memcmp; otherwise
// it walks `n_bytes / tops->size` element pairs via tops->equal. Traps when
// tops->size doesn't divide n_bytes.
//
// find_byte: linear search [begin, end) for an element equal to *needle.
// sizeof(T) == 1 + bytewise-eq: memchr. Larger T + bytewise-eq: memcmp per
// element. Otherwise: tops->equal per candidate. Returns end on miss.
// ---------------------------------------------------------------------------
bool equal_bytes(const type_ops* tops, const void* a, const void* b,
                 size_t n_bytes);

const void* find_byte(const type_ops* tops, const void* begin, const void* end,
                      const void* needle);

// ---------------------------------------------------------------------------
// Trivial-only shims used by the algorithm/container call sites. These are
// the bodies of the tops-form cores under `tops->equal == nullptr`, hoisted
// so the call sites don't have to construct a per-T type_ops constant. The
// element_size argument is `sizeof(T)`.
// ---------------------------------------------------------------------------
bool equal_bytes_triv(size_t element_size, const void* a, const void* b,
                      size_t n_bytes);

const void* find_byte_triv(size_t element_size, const void* begin,
                           const void* end, const void* needle);

// ----------------------------------------------------------------------------
// Trait: "two T*/U* point at values whose == is equivalent to memcmp".
//
// gcc-10 has no `__is_trivially_equality_comparable` builtin, so we follow
// libc++'s pre-builtin fallback: integral T (NOT float, NOT enum), or same-
// type-pointer. Same-width same-signedness integral cross-type also folds.
// Floating point is excluded (NaN, -0.0 compare equal but differ bytewise,
// and vice versa); enums are excluded (the user may have overloaded ==).
//
// The trait gates routing through `equal_bytes_triv`/`find_byte_triv`: when
// false the algorithm header keeps its typed loop.
// ----------------------------------------------------------------------------
template<class T, class U>
inline constexpr bool is_byte_equal_v_impl = false;

template<class T>
inline constexpr bool is_byte_equal_v_impl<T, T> =
    is_integral_v<T> || is_pointer_v<T>;

// Integral cross-type: same width and same signedness compares byte-wise.
template<class T, class U>
    requires (is_integral_v<T> && is_integral_v<U> && !is_same_v<T, U> &&
              sizeof(T) == sizeof(U) && is_signed_v<T> == is_signed_v<U>)
inline constexpr bool is_byte_equal_v_impl<T, U> = true;

template<class T, class U>
inline constexpr bool is_byte_equal_v =
    is_byte_equal_v_impl<remove_cv_t<T>, remove_cv_t<U>>;

} // namespace detail
} // namespace std
