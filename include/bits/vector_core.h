// bits/vector_core.h — shared, size-parameterized byte ops for vector's shift.
//
// These are non-template and take the element size as a RUNTIME argument, so a
// single body serves every (trivially-relocatable) element type — instead of the
// compiler emitting a per-type move/shift loop. Defined once in src/support.cpp.
// Valid ONLY for trivially-relocatable elements (they move by copying bytes).
#pragma once
#include <cstddef>

namespace std {
namespace detail {

// Open a gap of `gap` elements at index `off`: memmove the [off, n) tail up by
// `gap` slots. The `gap` slots at [off, off+gap) become raw storage for the
// caller to construct into. `n` is the current element count.
void vec_open_gap(void* base, size_t n, size_t off, size_t gap, size_t elem);

// Close a gap of `gap` elements at index `off`: memmove the [off+gap, n) tail
// down onto [off, ...). The caller must have already destroyed [off, off+gap)
// and shrinks the size by `gap` afterwards.
void vec_close_gap(void* base, size_t n, size_t off, size_t gap, size_t elem);

} // namespace detail
} // namespace std
