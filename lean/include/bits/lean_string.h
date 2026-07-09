// bits/lean_string.h — the single-instance byte engine behind lean <string>.
//
// A lean string is ONE pointer to the char payload of a lean block
// (bits/lean.h): detail::lean_used(payload) is the length in ELEMENTS and the
// storage is always NUL-terminated, so c_str()/data() is just that pointer.
// The empty string points at a shared immutable static rep — a size_t 0 header
// immediately followed by a zero terminator — never freed and never written.
//
// Characters are trivially copyable and trivially relocatable, so every growth
// / insert / erase / replace reduces to memcpy/memmove/realloc on raw bytes.
// These kernels are NON-TEMPLATE: basic_string<char>/wchar_t/char16_t/char32_t
// all share one copy, passing sizeof(CharT) as `elem`.  Because alignof == sizeof
// for the standard character types, `elem` is used as the block alignment too;
// lean_offset(elem) is sizeof(size_t) for every such type, so the `used` header
// always sits exactly sizeof(size_t) bytes below the payload — matching the
// static empty rep.
#pragma once
// bits/lean.h includes <malloc.h> for malloc_usable_size, a dynamic libc
// symbol.  <string> is routinely included from inside a
// `#pragma GCC visibility push(hidden)` region (e.g. <codecvt>), which would
// stamp that declaration hidden — and a hidden reference to a dynamic symbol
// fails to link ("hidden symbol isn't defined").  Force default visibility for
// the whole lean include so the libc reference stays bindable.
#pragma GCC visibility push(default)
#include <cstddef>
#include <cstring>
#include <bits/lean.h>
#pragma GCC visibility pop

namespace std {
namespace detail {

// The shared empty representation.  Zero-initialised: the first size_t (the
// `used` header) is 0 and the following bytes (the terminator, up to 8 bytes)
// are 0.  The payload pointer is `storage + sizeof(size_t)`, so reading
// lean_used(payload) yields 0 exactly like a real block.  Never written.
alignas(size_t) inline unsigned char lean_str_empty_storage[2 * sizeof(size_t)] = {};

inline void* lean_str_empty_payload() noexcept {
  return lean_str_empty_storage + sizeof(size_t);
}

inline bool lean_str_is_static(const void* p) noexcept {
  return p == lean_str_empty_payload();
}

// Usable capacity in ELEMENTS (excluding the terminator).  Never called on the
// static rep (the owner branches on lean_str_is_static first).
inline size_t lean_str_cap(void* p, size_t elem) noexcept {
  return lean_capacity_bytes(p, elem) / elem - 1;
}

inline bool lean_str_in_range(const void* s, const void* base, size_t bytes) noexcept {
  const char* c = static_cast<const char*>(s);
  const char* b = static_cast<const char*>(base);
  return c >= b && c < b + bytes;
}

// Write the elem-wide zero terminator at element index `used`.
inline void lean_str_write_term(void* p, size_t used, size_t elem) noexcept {
  __builtin_memset(static_cast<char*>(p) + used * elem, 0, elem);
}

// Replicate one `elem`-byte element `n` times starting at dst.
inline void lean_str_fill(void* dst, size_t elem, size_t n, const void* one) noexcept {
  char* d = static_cast<char*>(dst);
  if (elem == 1) {
    __builtin_memset(d, *static_cast<const unsigned char*>(one), n);
  } else {
    for (size_t i = 0; i < n; ++i)
      __builtin_memcpy(d + i * elem, one, elem);
  }
}

// Ensure the block can hold at least `need` elements (plus the terminator),
// preserving the content, length and terminator.  Returns the (possibly new)
// payload.  Grows from the static rep by allocating; grows a heap block by
// realloc (chars are trivially relocatable).  Allocates exactly `need` — the
// amortised geometric growth lives in the splice kernels' append path.
inline void* lean_str_reserve(void* p, size_t elem, size_t need) noexcept {
  bool st = lean_str_is_static(p);
  if (!st && need <= lean_str_cap(p, elem))
    return p;
  size_t used = lean_used(p);
  if (st) {
    void* np = lean_alloc(elem, (need + 1) * elem);
    __builtin_memcpy(np, p, (used + 1) * elem);  // used == 0: copies terminator
    lean_used(np) = used;
    return np;
  }
  return lean_realloc(p, elem, (need + 1) * elem);
}

// Shrink a heap block to fit its current length; an emptied heap block is freed
// and the string reverts to the static rep.  realloc-down may keep the same
// allocation when glibc's bin already fits — capacity() then stays as measured.
inline void* lean_str_shrink(void* p, size_t elem) noexcept {
  if (lean_str_is_static(p))
    return p;
  size_t used = lean_used(p);
  if (used == 0) {
    lean_free(p, elem);
    return lean_str_empty_payload();
  }
  if (used >= lean_str_cap(p, elem))
    return p;
  return lean_realloc(p, elem, (used + 1) * elem);
}

// Geometric target: double, but never below what is needed.
inline size_t lean_str_geo(size_t cap, size_t need) noexcept {
  size_t twice = cap * 2;
  return twice > need ? twice : need;
}

// Core splice: at element index `pos`, delete `n_del` elements and insert
// `n_add` elements copied from `src` (n_add*elem bytes).  Returns the new
// payload.  Alias-safe: when `src` points inside the current buffer the kernel
// reallocates and copies from the old buffer before freeing it, so s += s and
// self-referential insert/replace are correct.  The caller guarantees
// pos + n_del <= used.
inline void* lean_str_splice(void* p, size_t elem, size_t pos, size_t n_del,
                             const void* src, size_t n_add) noexcept {
  size_t used = lean_used(p);
  size_t new_used = used - n_del + n_add;
  bool st = lean_str_is_static(p);
  if (st && new_used == 0)
    return p;  // still empty: stay on the static rep

  size_t cap = st ? 0 : lean_str_cap(p, elem);
  bool alias = !st && src &&
               lean_str_in_range(src, p, (cap + 1) * elem);

  if (st || new_used > cap || alias) {
    size_t geo = new_used > cap ? lean_str_geo(cap, new_used) : new_used;
    void* np = lean_alloc(elem, (geo + 1) * elem);
    char* d = static_cast<char*>(np);
    const char* o = static_cast<const char*>(p);
    if (pos)
      __builtin_memcpy(d, o, pos * elem);
    if (n_add)
      __builtin_memcpy(d + pos * elem, src, n_add * elem);  // src in old: live
    size_t suffix = used - pos - n_del;
    if (suffix)
      __builtin_memcpy(d + (pos + n_add) * elem,
                       o + (pos + n_del) * elem, suffix * elem);
    lean_str_write_term(np, new_used, elem);
    lean_used(np) = new_used;
    if (!st)
      lean_free(p, elem);
    return np;
  }

  // In place, src does not alias the buffer.
  char* d = static_cast<char*>(p);
  size_t suffix = used - pos - n_del;
  if (suffix)
    __builtin_memmove(d + (pos + n_add) * elem,
                      d + (pos + n_del) * elem, suffix * elem);
  if (n_add)
    __builtin_memcpy(d + pos * elem, src, n_add * elem);
  lean_str_write_term(p, new_used, elem);
  lean_used(p) = new_used;
  return p;
}

// Fill variant of the splice: insert `n_add` copies of the element at `one`.
// No aliasing is possible — the fill value is an external single element.
inline void* lean_str_splice_fill(void* p, size_t elem, size_t pos, size_t n_del,
                                  size_t n_add, const void* one) noexcept {
  size_t used = lean_used(p);
  size_t new_used = used - n_del + n_add;
  bool st = lean_str_is_static(p);
  if (st && new_used == 0)
    return p;

  size_t cap = st ? 0 : lean_str_cap(p, elem);
  if (st || new_used > cap) {
    size_t geo = new_used > cap ? lean_str_geo(cap, new_used) : new_used;
    void* np = lean_alloc(elem, (geo + 1) * elem);
    char* d = static_cast<char*>(np);
    const char* o = static_cast<const char*>(p);
    if (pos)
      __builtin_memcpy(d, o, pos * elem);
    if (n_add)
      lean_str_fill(d + pos * elem, elem, n_add, one);
    size_t suffix = used - pos - n_del;
    if (suffix)
      __builtin_memcpy(d + (pos + n_add) * elem,
                       o + (pos + n_del) * elem, suffix * elem);
    lean_str_write_term(np, new_used, elem);
    lean_used(np) = new_used;
    if (!st)
      lean_free(p, elem);
    return np;
  }

  char* d = static_cast<char*>(p);
  size_t suffix = used - pos - n_del;
  if (suffix)
    __builtin_memmove(d + (pos + n_add) * elem,
                      d + (pos + n_del) * elem, suffix * elem);
  if (n_add)
    lean_str_fill(d + pos * elem, elem, n_add, one);
  lean_str_write_term(p, new_used, elem);
  lean_used(p) = new_used;
  return p;
}

} // namespace detail
} // namespace std
