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

// The shared empty representation.  A REAL size_t object (`used`, value 0) sits
// at the block-header position, immediately followed by >= 8 zero terminator
// bytes at the payload — so reading lean_used(payload) (i.e.
// reinterpret_cast<const size_t*>(payload)[-1]) accesses an actual size_t
// object, not a size_t-typed read of a char array (which would be a no-object
// strict-aliasing read).  Immutable (const, .rodata, never written), shared
// across char widths: lean_offset(elem) == sizeof(size_t) for every standard
// character type (all have alignof <= sizeof(size_t)), so the header always
// lands exactly sizeof(size_t) bytes below the 8-aligned payload.
//
// It is ONE exported symbol DEFINED in lean/src/kernels.cpp (not an inline
// variable): every empty string of a given width, and every kernel, must agree
// on its single address, because lean_str_is_static() is a pointer-identity
// test. An inline/COMDAT definition would get a DIFFERENT address in the main
// program vs. inside liblean.so, so a kernel there would mistake the shared rep
// for a heap block and free() read-only memory. Default visibility + a single
// out-of-line definition make the address identical across the library
// boundary (this is exactly how libc++ ships its empty-string rep).
struct lean_str_empty_rep {
  size_t used;         // the block header: a genuine size_t object, value 0
  char   terminator[8]; // >= 8 zero bytes, 8-aligned, readable as any width's NUL
};
#pragma GCC visibility push(default)
extern const lean_str_empty_rep lean_str_empty;
#pragma GCC visibility pop

// constexpr-usable: the ADDRESS of an extern-const static-storage object is a
// constant expression even though its VALUE (defined in kernels.cpp) is not, so
// the empty-rep payload pointer can be formed at compile time.  This is the one
// thing a lean string CAN do in constant evaluation — hold the shared empty rep
// (see lean/README.md deviations 2 & 3: no constexpr heap, no SSO, but the empty
// string is a compile-time constant just like base's SSO-empty).  The const_cast
// forms a mutable pointer value (never written through in constant evaluation).
constexpr char* lean_str_empty_payload() noexcept {
  // terminator sits sizeof(size_t) bytes above `used`; payload[-1] therefore
  // reads the real `used` size_t object.  Returns char* (not void*) so the char
  // instantiation of basic_string can assign it to its char* member with no cast
  // at all — a void*->char* static_cast is not permitted in constant evaluation,
  // but a same-type assignment is.
  return const_cast<char*>(lean_str_empty.terminator);
}

constexpr bool lean_str_is_static(const void* p) noexcept {
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

// Geometric target: double, but never below what is needed.
inline size_t lean_str_geo(size_t cap, size_t need) noexcept {
  size_t twice = cap * 2;
  return twice > need ? twice : need;
}

// ---------------------------------------------------------------------------
// Fat byte engines — defined out of line in lean/src/kernels.cpp (one copy per
// system). Declarations only here. Forced to DEFAULT visibility for the same
// reason as the <bits/lean.h> include above: <string> is routinely included
// inside a `#pragma GCC visibility push(hidden)` region (e.g. <locale> via
// <codecvt>), which would stamp these references hidden — a hidden reference
// cannot bind to a definition in liblean.a / liblean.so ("hidden symbol isn't
// defined"). Default visibility keeps the library link working.
// ---------------------------------------------------------------------------
#pragma GCC visibility push(default)

// Ensure the block can hold at least `need` elements (plus the terminator),
// preserving the content, length and terminator.  Returns the (possibly new)
// payload.  Grows from the static rep by allocating; grows a heap block by
// realloc (chars are trivially relocatable).  Allocates exactly `need` — the
// amortised geometric growth lives in the splice kernels' append path.
void* lean_str_reserve(void* p, size_t elem, size_t need) noexcept;

// Shrink a heap block to fit its current length; an emptied heap block is freed
// and the string reverts to the static rep.  realloc-down may keep the same
// allocation when glibc's bin already fits — capacity() then stays as measured.
void* lean_str_shrink(void* p, size_t elem) noexcept;

// Core splice: at element index `pos`, delete `n_del` elements and insert
// `n_add` elements copied from `src` (n_add*elem bytes).  Returns the new
// payload.  Alias-safe: when `src` points inside the current buffer the kernel
// reallocates and copies from the old buffer before freeing it, so s += s and
// self-referential insert/replace are correct.  The caller guarantees
// pos + n_del <= used.
void* lean_str_splice(void* p, size_t elem, size_t pos, size_t n_del,
                      const void* src, size_t n_add) noexcept;

// Fill variant of the splice: insert `n_add` copies of the element at `one`.
// No aliasing is possible — the fill value is an external single element.
void* lean_str_splice_fill(void* p, size_t elem, size_t pos, size_t n_del,
                           size_t n_add, const void* one) noexcept;

#pragma GCC visibility pop

} // namespace detail
} // namespace std
