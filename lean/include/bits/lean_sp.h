// bits/lean_sp.h — the single-instance reference-count engine behind the lean
// <memory> shared_ptr / weak_ptr family.
//
// A lean shared_ptr / weak_ptr is ONE pointer: the handle points straight at a
// control block, and null = empty.  The block is the classic two-count scheme:
//
//   struct sp_cb { void* elem; uint32_t strong; uint32_t weak; void(*dispose)(sp_cb*); };
//
// ELEM-FIRST (hot-path) layout: get()/operator*/operator-> perform a double
// indirection (load cb, then load cb->elem); with elem at offset 0 the second
// load needs no displacement — smaller x86 encoding, free addressing on RISC.
// The counts are only touched on copy/destroy and dispose only at death, so
// they sit after the pointer.  24 bytes on LP64, 16 on ILP32.
//
// The strong holders collectively own ONE weak reference: dispose(cb) destroys
// the managed object when strong reaches 0; the block itself is free()d when
// weak reaches 0.  Counts are 32-bit and the retain kernels TRAP on overflow
// (2^32 live handles is impossible on the target class — trap, never wrap).
//
// These kernels are NON-TEMPLATE and carry NO per-type code: the only per-type
// code in the whole design is the `dispose` thunk, which lives in <memory>.
#pragma once
// <memory> is routinely included from inside a `#pragma GCC visibility
// push(hidden)` region; a hidden reference cannot bind to a definition that
// ships in liblean.a / liblean.so.  Force default visibility across the whole
// lean include, exactly as bits/lean_string.h does, so the sp_* references
// stay bindable across the library boundary.
#pragma GCC visibility push(default)
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#pragma GCC visibility pop

namespace std {
namespace detail {

// The control block.  elem first — see the hot-path note in the file banner.
struct sp_cb {
  void* elem;                 // the managed object (get() casts this)
  uint32_t strong;            // strong owners; 0 => object destroyed
  uint32_t weak;              // weak owners + 1 held collectively by the strongs
  void (*dispose)(sp_cb*);    // destroy the managed object (NOT free the block)
};

// ---------------------------------------------------------------------------
// The refcount kernels — declarations only; the bodies live out of line in
// lean/src/kernels.cpp (one copy per binary, or one per system as liblean.*).
// Default visibility for the same reason as the include block above.
// ---------------------------------------------------------------------------
#pragma GCC visibility push(default)

// Relaxed increment of the strong count; traps on 32-bit overflow.
void sp_retain(sp_cb* cb) noexcept;

// Decrement the strong count (release, with an acquire fence on the zero
// transition).  On reaching 0: dispose the managed object, then drop the one
// weak reference the strong holders shared (which may free the block).
void sp_release(sp_cb* cb) noexcept;

// Relaxed increment of the weak count; traps on 32-bit overflow.
void sp_weak_retain(sp_cb* cb) noexcept;

// Decrement the weak count (release, acquire fence on zero).  On reaching 0 the
// block is free()d — for make_shared this frees the inline object storage too.
void sp_weak_release(sp_cb* cb) noexcept;

// weak_ptr::lock — CAS-increment the strong count only while it is nonzero.
// Returns cb (strong already incremented) on success, nullptr if the object is
// already dead.
sp_cb* sp_lock(sp_cb* cb) noexcept;

// Relaxed load of the strong count (weak_ptr::use_count / expired,
// shared_ptr::use_count).
uint32_t sp_use_count(sp_cb* cb) noexcept;

#pragma GCC visibility pop

// Byte offset at which an inline payload of the given alignment sits after the
// control block within one allocation.  Used by make_shared (T placed inline)
// and the custom-deleter constructor (D placed inline); dispose recomputes the
// same offset, so both agree without storing it.
inline size_t sp_payload_offset(size_t align) noexcept {
  size_t base = sizeof(sp_cb);
  size_t rem = base % align;
  return rem ? base + (align - rem) : base;
}

} // namespace detail
} // namespace std
