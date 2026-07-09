// lean/include/bits/lean.h — the lean profile's allocation kernel and type ops.
//
// FROZEN CONTRACT: every lean container builds on this file. Component
// headers must not edit it; put component-specific helpers in your own
// bits/lean_<component>.h.
//
// The lean block: array-like containers store ONE pointer, to the payload of
//
//   [ base ...... header ][ payload ................... spare tail ]
//              ^ size_t `used` at payload - sizeof(size_t)
//
// - `used` (element count or byte length — the owner decides) lives at a
//   fixed negative offset inside the same allocation as the data.
// - Capacity is NOT stored: lean_capacity_bytes() recomputes it from
//   malloc_usable_size(base), so glibc's bin rounding becomes free capacity.
// - The header offset is max(sizeof(size_t), alignof(T)) so the payload is
//   aligned for any T with alignof(T) <= alignof(max_align_t); containers
//   static_assert that bound.
// - Allocation failure traps (this profile is -fno-exceptions, matching the
//   base library's throw sites).
#pragma once
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>
#include <bits/relocatable.h>
#include <malloc.h>

namespace std {

template<class T> class allocator;

namespace detail {

// Port hook: on a non-glibc target, replace the body with the platform's
// "how big is this malloc block really" call (or `return 0` to fall back to
// exact-fit reallocation everywhere).
inline size_t lean_usable_size(void* base) noexcept {
  return ::malloc_usable_size(base);
}

inline constexpr size_t lean_offset(size_t align) noexcept {
  return align > sizeof(size_t) ? align : sizeof(size_t);
}

inline size_t& lean_used(void* payload) noexcept {
  return reinterpret_cast<size_t*>(payload)[-1];
}
inline size_t lean_used(const void* payload) noexcept {
  return reinterpret_cast<const size_t*>(payload)[-1];
}

inline void* lean_base(void* payload, size_t align) noexcept {
  return static_cast<char*>(payload) - lean_offset(align);
}

// Bytes available for payload (>= what was asked for at allocation).
inline size_t lean_capacity_bytes(void* payload, size_t align) noexcept {
  return lean_usable_size(lean_base(payload, align)) - lean_offset(align);
}

// Allocate a block with room for `bytes` of payload; `used` starts at 0.
inline void* lean_alloc(size_t align, size_t bytes) noexcept {
  void* base = ::malloc(lean_offset(align) + bytes);
  if (!base)
    __builtin_trap();
  void* payload = static_cast<char*>(base) + lean_offset(align);
  lean_used(payload) = 0;
  return payload;
}

// Resize in place when possible; `used` and the payload BYTES move together,
// so this is valid only while the payload content is memcpy-safe (trivially
// relocatable). Kernels handling non-trivial types allocate fresh and
// relocate through the ops instead.
inline void* lean_realloc(void* payload, size_t align, size_t bytes) noexcept {
  void* base = ::realloc(lean_base(payload, align), lean_offset(align) + bytes);
  if (!base)
    __builtin_trap();
  return static_cast<char*>(base) + lean_offset(align);
}

inline void lean_free(void* payload, size_t align) noexcept {
  ::free(lean_base(payload, align));
}

// Growth advice in bytes: geometric, then let malloc's rounding add the rest
// (lean_capacity_bytes() recovers whatever extra the allocator handed back).
inline size_t lean_grow_bytes(size_t capacity, size_t need) noexcept {
  size_t twice = capacity * 2;
  return twice > need ? twice : need;
}

// ---------------------------------------------------------------------------
// lean_ops — the type-operations table the single-instance kernels take
// instead of being templates. A null function pointer means "trivial": the
// kernel uses memcpy/memmove or skips the pass entirely, with no calls.
// ---------------------------------------------------------------------------
struct lean_ops {
  unsigned size;
  unsigned align;
  // Move n elements to fresh storage and end the source objects' lifetimes.
  // relocate: dst does not overlap src, or dst < src (forward walk is safe).
  // relocate_backward: dst > src with possible overlap (walks from the end).
  // Null => the type is trivially relocatable => kernels memmove.
  void (*relocate)(void* dst, void* src, size_t n);
  void (*relocate_backward)(void* dst, void* src, size_t n);
  // Copy-construct n elements into raw storage. Null => memcpy.
  void (*copy)(void* dst, const void* src, size_t n);
  // Destroy n elements. Null => trivial destruction, kernels skip the pass.
  void (*destroy)(void* p, size_t n);
};

template<class T>
void lean_relocate_thunk(void* dstv, void* srcv, size_t n) {
  T* dst = static_cast<T*>(dstv);
  T* src = static_cast<T*>(srcv);
  for (size_t i = 0; i < n; ++i) {
    ::new (static_cast<void*>(dst + i)) T(static_cast<T&&>(src[i]));
    src[i].~T();
  }
}

template<class T>
void lean_relocate_backward_thunk(void* dstv, void* srcv, size_t n) {
  T* dst = static_cast<T*>(dstv);
  T* src = static_cast<T*>(srcv);
  for (size_t i = n; i > 0; --i) {
    ::new (static_cast<void*>(dst + i - 1)) T(static_cast<T&&>(src[i - 1]));
    src[i - 1].~T();
  }
}

template<class T>
void lean_copy_thunk(void* dstv, const void* srcv, size_t n) {
  T* dst = static_cast<T*>(dstv);
  const T* src = static_cast<const T*>(srcv);
  for (size_t i = 0; i < n; ++i)
    ::new (static_cast<void*>(dst + i)) T(src[i]);
}

template<class T>
void lean_destroy_thunk(void* pv, size_t n) {
  T* p = static_cast<T*>(pv);
  for (size_t i = 0; i < n; ++i)
    p[i].~T();
}

// if constexpr keeps the thunks uninstantiated for types that can't support
// them (move-only elements must not instantiate the copy thunk, etc.).
template<class T>
constexpr auto lean_relocate_ptr() {
  if constexpr (is_trivially_relocatable_v<T>)
    return static_cast<void (*)(void*, void*, size_t)>(nullptr);
  else
    return &lean_relocate_thunk<T>;
}
template<class T>
constexpr auto lean_relocate_backward_ptr() {
  if constexpr (is_trivially_relocatable_v<T>)
    return static_cast<void (*)(void*, void*, size_t)>(nullptr);
  else
    return &lean_relocate_backward_thunk<T>;
}
template<class T>
constexpr auto lean_copy_ptr() {
  if constexpr (is_trivially_copyable_v<T>)
    return static_cast<void (*)(void*, const void*, size_t)>(nullptr);
  else if constexpr (is_copy_constructible_v<T>)
    return &lean_copy_thunk<T>;
  else
    return static_cast<void (*)(void*, const void*, size_t)>(nullptr);
}
template<class T>
constexpr auto lean_destroy_ptr() {
  if constexpr (is_trivially_destructible_v<T>)
    return static_cast<void (*)(void*, size_t)>(nullptr);
  else
    return &lean_destroy_thunk<T>;
}

// One table per element type per program (inline variable: the linker dedups).
template<class T>
inline constexpr lean_ops lean_ops_for = {
    sizeof(T),
    alignof(T),
    lean_relocate_ptr<T>(),
    lean_relocate_backward_ptr<T>(),
    lean_copy_ptr<T>(),
    lean_destroy_ptr<T>(),
};

// The static_assert every lean container applies to its element/allocator.
template<class T, class Allocator>
inline constexpr bool lean_allocator_ok =
    is_same_v<Allocator, allocator<T>>;

} // namespace detail
} // namespace std
