// bits/lean_variant.h — the type-erased special-member engine behind the lean
// <variant>.
//
// Base <variant> gives each variant instantiation its own N-way switch for
// copy/move-construct, copy/move-assign and destroy: the visitation apparatus
// (an fmatrix/fdiagonal of dispatch function pointers plus a distinct lambda
// per special member) is re-emitted per variant type. Measured at -Os that is
// the ~853 B/type "per-type" slice.
//
// The lean profile replaces those switches with ONE static constexpr rodata
// table per (non-trivial) variant type — a single dispatch function pointer per
// alternative — consumed by a handful of NON-TEMPLATE walker kernels defined
// out of line in lean/src/kernels.cpp (one copy per binary, or one per system
// as liblean.*). The only per-type code that remains is one tiny per-ALTERNATIVE
// dispatch thunk, shared across every variant that lists that alternative, and
// ELIDED (null pointer) when the alternative is trivial for EVERY operation —
// the same null-means-trivial convention bits/lean.h uses for lean_ops.
//
// One pointer per alternative (not five, one per special member) is deliberate:
// the rodata table is itself per-type cost, so a 5-pointer row (40 B) times N
// alternatives swamped the code the walkers save. A single dispatch pointer with
// an action selector shrinks the table 5x, which measured as the difference
// between a net loss and a net win vs base.
//
// Storage note: base's union_storage is a recursive union whose every leaf
// alt<I,T> — and thus every alt's `value` — sits at offset 0 of the storage.
// So a walker can treat the storage pointer as a T* for the active alternative;
// the thunk placement-constructs / destroys / assigns the value in place. A null
// table entry means the alternative is trivially copyable (hence trivially
// destructible too), so the walker moves the raw storage bytes (memcpy) or, for
// destroy, skips the pass — exactly the lean_ops discipline.
#pragma once
// <variant> may be included from inside a `#pragma GCC visibility push(hidden)`
// region; a hidden reference cannot bind to a definition shipped in
// liblean.a / liblean.so. Force default visibility across this include, exactly
// as bits/lean_sp.h / bits/lean_fn.h do, so the variant_* kernel references
// stay bindable across the library boundary.
#pragma GCC visibility push(default)
#include <cstddef>
#include <cstring>
#include <new>
#include <type_traits>
#include <utility>
#pragma GCC visibility pop

namespace std {
namespace detail {

// The five special-member actions the dispatch thunk performs. Kept contiguous
// from 0 so the thunk's switch lowers to a dense jump.
enum vt_action {
  VT_COPY_CTOR = 0,   // placement copy-construct dst from src
  VT_MOVE_CTOR,       // placement move-construct dst from src
  VT_DESTROY,         // destroy dst in place (src ignored)
  VT_COPY_ASSIGN,     // dst = src            (same-index copy-assign)
  VT_MOVE_ASSIGN      // dst = std::move(src) (same-index move-assign)
};

// One dispatch thunk per alternative type. A null table slot means the
// alternative is trivially copyable — the walker takes the raw-bytes / skip
// path for every action, calling nothing.
using vt_op_fn = void (*)(int action, void* dst, void* src);

// ---------------------------------------------------------------------------
// The walker kernels — declarations only; bodies out of line in kernels.cpp.
// `index` is the active alternative (variant_npos == size_t(-1) => valueless,
// which is unreachable after construction under -fno-exceptions but handled).
// `storage_size` is sizeof the whole union storage: the byte count the trivial
// (null-slot) path moves.
// ---------------------------------------------------------------------------
#pragma GCC visibility push(default)

void variant_destroy(void* storage, size_t index, const vt_op_fn* table) noexcept;

void variant_copy_construct(void* dst, const void* src, size_t index,
                            const vt_op_fn* table, size_t storage_size);
void variant_move_construct(void* dst, void* src, size_t index,
                            const vt_op_fn* table, size_t storage_size);

// Assignment composes the standard's semantics: same index => the alternative's
// assignment operator (VT_COPY_ASSIGN / VT_MOVE_ASSIGN, or a byte move when
// trivial); different index => destroy the old alternative, then construct the
// new one from the source. Under -fno-exceptions the strong-guarantee temporary
// the base library builds for a cross-index copy into a non-nothrow-copyable /
// nothrow-movable alternative is unobservable, so it is dropped: cross-index
// assign is destroy + direct construct. (Live/value results are identical; only
// the count of moves of a throw-away temporary differs, and that count exists
// solely to unwind an exception this profile never throws.)
void variant_copy_assign(void* dst, size_t dst_index,
                         const void* src, size_t src_index,
                         const vt_op_fn* table, size_t storage_size);
void variant_move_assign(void* dst, size_t dst_index,
                         void* src, size_t src_index,
                         const vt_op_fn* table, size_t storage_size);

#pragma GCC visibility pop

// ---------------------------------------------------------------------------
// The per-alternative dispatch thunk (header-only template: the linker dedups
// one copy per alternative type, shared across every variant that lists it).
// The if constexpr guards keep an action uninstantiated for a type that cannot
// support it (a move-only alternative never instantiates the copy branches —
// its copy ops are deleted at the variant level and that action is never
// reached), so the thunk compiles for every alternative a variant can hold.
// ---------------------------------------------------------------------------
template<class T>
void vt_op(int action, void* dst, void* src) {
  switch (action) {
  case VT_COPY_CTOR:
    if constexpr (is_copy_constructible_v<T>)
      ::new (dst) T(*static_cast<const T*>(src));
    break;
  case VT_MOVE_CTOR:
    if constexpr (is_move_constructible_v<T>)
      ::new (dst) T(static_cast<T&&>(*static_cast<T*>(src)));
    break;
  case VT_DESTROY:
    static_cast<T*>(dst)->~T();
    break;
  case VT_COPY_ASSIGN:
    if constexpr (is_copy_assignable_v<T>)
      *static_cast<T*>(dst) = *static_cast<const T*>(src);
    break;
  case VT_MOVE_ASSIGN:
    if constexpr (is_move_assignable_v<T>)
      *static_cast<T*>(dst) = static_cast<T&&>(*static_cast<T*>(src));
    break;
  }
}

// Null when the alternative is trivially copyable (which implies a trivial
// destructor): the walker then moves raw bytes / skips, calling nothing.
template<class T>
constexpr vt_op_fn make_vt_op() {
  if constexpr (is_trivially_copyable_v<T>) return nullptr;
  else return &vt_op<T>;
}

} // namespace detail
} // namespace std
