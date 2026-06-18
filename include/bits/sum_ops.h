// bits/sum_ops.h — vocab + cores for variant-shaped sum types.
//
// std::variant<Ts...> currently routes its lifecycle paths (destroy /
// copy-construct / move-construct) through visitation_base, which builds an
// fmatrix of N function pointers per modifier — one entry per alternative,
// keyed on the exact (Visitor, Ts...) tuple. For the lifecycle visitors the
// dispatch tables and the per-alternative trampolines fold across variant
// flavors only when the body shape AND the per-Alt instantiation set both
// match; in practice every distinct variant<Ts...> emits its own machinery.
//
// sum_ops replaces that with a constant table per variant flavor:
//
//   struct sum_ops {
//       n_alternatives, size, align, flags,   // aggregate descriptor
//       const alt_ops* alts;                  // per-alternative leaves
//   };
//
// Each alt_ops carries one set of typeless leaves — destroy / copy_construct /
// move_construct — that act on the active alternative's storage. The cores
// (sum_destroy / sum_copy_construct / sum_move_construct) take a const
// sum_ops*, the byte-pointer at the variant's union storage, and the active
// index; they index into `alts[idx]` and call the matching leaf.
//
// Bodies that fold across flavors live in src/support.cpp; per-T leaves are
// instantiated once per T and COMDAT-fold across variant flavors.
//
// The std::visit / comparison / cross-index assignment paths stay
// template-instantiated; this vocab covers ONLY the three lifecycle
// modifiers, which is where the fmatrix-per-flavor cost concentrates.
//
// No allocator threading (variant has no allocator); no f_alloc_ctx slot.
// The trivial fast paths (`flags & f_triv_destroy/copy/reloc`) mean an
// entire sum_destroy/copy/move call short-circuits without dispatching.
#pragma once
#include <cstddef>
#include <new>             // placement new
#include <type_traits>
#include <utility>
#include <bits/relocatable.h>
// NOTE on includes: we do NOT include <bits/type_ops.h> here. type_ops.h pulls
// <memory>, which pulls <iterator>, which pulls <variant> — and <variant>
// includes this header, so the cycle would leave sum_ops's declarations
// invisible to the use sites in <variant>. The three flag bits sum_ops cares
// about are duplicated below (kept numerically identical to the matching
// ops_flags values in bits/type_ops.h so a stale-skew of the two definitions
// would be a load-bearing build break).

namespace std {
namespace detail {

// Local copies of the bit values from ops_flags in bits/type_ops.h. Same
// numeric values so the sum_ops flags field can be eyeballed against a
// type_ops flags field in a debugger; static_assert in src/support.cpp
// pins them to the canonical ones.
inline constexpr unsigned sum_f_triv_copy    = 1u << 1;
inline constexpr unsigned sum_f_triv_reloc   = 1u << 2;
inline constexpr unsigned sum_f_triv_destroy = 1u << 3;

// Per-alternative leaves. `offset` is the byte offset of the alternative's
// payload within the variant's union storage — always 0 in the current
// recursive-union layout (every alt overlaps at the union base), but kept as
// a slot so the cores don't assume the layout invariant. Each leaf takes a
// pointer to the alternative payload (i.e. self + offset) and performs the
// operation as if calling the matching leaf on a T at that address.
struct alt_ops {
    size_t offset;
    void (*destroy)(void* alt);
    void (*copy_construct)(void* dst_alt, const void* src_alt);
    void (*move_construct)(void* dst_alt, void* src_alt);
};

// Aggregate descriptor. n_alternatives is the index range the cores accept;
// size/align describe the variant's union storage (NOT the whole variant —
// the index field is owned by the caller). flags is the AND of per-alt
// triviality, matching variant's lifecycle rules (a variant is trivially
// destructible iff every alternative is, etc.).
struct sum_ops {
    size_t         n_alternatives;
    size_t         size;
    size_t         align;
    unsigned       flags;
    const alt_ops* alts;
};

// Trivial-fast-path predicates: AND of per-alt triviality. There is no
// allocator for variant, so the f_alloc_default_life half of the contiguous-
// cores' check collapses to "always true" here and isn't part of the check.
// Calling sum_destroy on a variant whose flags promise triv_destroy is one
// branch and a return.
//
// NOTE on triv_reloc vs triv_copy: variant's move-construct semantics are
// "construct dst from std::move(*src), src remains alive and will be
// destroyed by the caller later" — NOT the relocate-and-drop pattern that
// the contiguous-storage cores use. So sum_move_construct's fast path is
// triv_COPY (bit-copy that leaves both objects independent), not triv_reloc
// (which would memcpy and then expect src not to be destroyed). The
// triv_reloc bit is recorded for completeness and possible future cores;
// no current core consumes it.
inline bool sum_triv_destroy(const sum_ops* s) { return s->flags & sum_f_triv_destroy; }
inline bool sum_triv_copy   (const sum_ops* s) { return s->flags & sum_f_triv_copy;    }
inline bool sum_triv_reloc  (const sum_ops* s) { return s->flags & sum_f_triv_reloc;   }

// Destroy the active alternative at `self`. PRECONDITION trap: index in
// range. For trivially-destructible variants (every alt triv-destroy) this
// is one check and return.
void sum_destroy(const sum_ops* s, void* self, size_t index);

// Copy-construct the alternative-at-src_index into dst's storage. The caller
// is responsible for updating the discriminator (index field) — the core
// only touches the payload bytes. For trivially-copyable variants this is a
// memcpy of the whole union storage.
void sum_copy_construct(const sum_ops* s, void* dst, const void* src,
                        size_t src_index);

// Move-construct the alternative-at-src_index into dst's storage. Same
// caller contract on the discriminator as sum_copy_construct. Trivially-
// relocatable variants memcpy the whole union storage.
void sum_move_construct(const sum_ops* s, void* dst, void* src,
                        size_t src_index);

// ---- per-alternative leaf templates -------------------------------------
// Instantiated once per T; identical bodies for the same T COMDAT-fold
// across variant flavors. No allocator threading — variant uses ::new /
// direct dtor / direct copy/move-construct, matching libc++'s in-place
// visitation pattern.
template<class T> void alt_destroy_op(void* p) {
    static_cast<T*>(p)->~T();
}
template<class T> void alt_copy_construct_op(void* d, const void* s) {
    ::new (d) T(*static_cast<const T*>(s));
}
template<class T> void alt_move_construct_op(void* d, void* s) {
    ::new (d) T(static_cast<T&&>(*static_cast<T*>(s)));
}

// ---- table construction --------------------------------------------------
// Build the per-Alt table for Ts... at offset 0 (every alt overlaps at the
// union base in the current variant layout). A leaf is installed iff its
// expression is well-formed for T AND the op is not trivial+default-life; we
// still keep a non-null leaf for trivial types because the trivial fast path
// is owned by the sum_ops `flags` bit, not by leaf-nullness — and the leaf
// stays available for the rare path that wants to dispatch unconditionally.
// (Symmetric to type_ops's leaf install rules.)
// Note on the move_construct gate: it's keyed on is_trivially_COPYABLE, not
// is_trivially_relocatable. Variant's move-construct semantics are "make a
// new dst from std::move(*src), src stays alive and is destroyed later by the
// caller" — that's the copy-construct shape with an rvalue. For a trivially
// copyable T, a bitwise copy is semantically a move construct (dst and src
// are both independent valid objects); for a trivially-relocatable-but-not-
// trivially-copyable T (the std::string case, by the explicit specialization
// in bits/relocatable.h), a memcpy would leave src and dst sharing whatever
// owns-the-resource fields the type carries — a double-free at scope exit.
// So the leaf is needed unless trivial copy applies.
template<class T>
constexpr alt_ops make_alt_ops() {
    alt_ops a{0, nullptr, nullptr, nullptr};
    if constexpr (requires(T* p) { p->~T(); })
        if constexpr (!is_trivially_destructible_v<T>)
            a.destroy = &alt_destroy_op<T>;
    if constexpr (requires(T* d, const T& s) { ::new (d) T(s); })
        if constexpr (!is_trivially_copyable_v<T>)
            a.copy_construct = &alt_copy_construct_op<T>;
    if constexpr (requires(T* d, T&& s) { ::new (d) T(static_cast<T&&>(s)); })
        if constexpr (!is_trivially_copyable_v<T>)
            a.move_construct = &alt_move_construct_op<T>;
    return a;
}

template<class T>
inline constexpr alt_ops alt_ops_for = make_alt_ops<T>();

// Array of alt_ops for Ts... is a constexpr inline variable so sum_ops::alts
// can be taken as an address inside a constant initializer.
template<class... Ts>
inline constexpr alt_ops alts_table_for[sizeof...(Ts)] = { alt_ops_for<Ts>... };

// Build the sum_ops descriptor for variant<Ts...>. `SumSize`/`SumAlign` are
// the size/align of the variant's union storage and are passed in so this
// header doesn't have to know variant's recursive-union types. flags is the
// AND of per-alt triviality bits, so the trivial fast path is "every
// alternative is trivially X" — matching the variant lifecycle rule.
template<class... Ts>
constexpr sum_ops make_sum_ops(size_t sum_size, size_t sum_align) {
    unsigned flags = 0;
    if constexpr ((is_trivially_destructible_v<Ts> && ...)) flags |= sum_f_triv_destroy;
    if constexpr ((is_trivially_copyable_v<Ts>      && ...)) flags |= sum_f_triv_copy;
    // sum_f_triv_reloc deliberately NOT set from is_trivially_relocatable_v:
    // the cores don't have relocate semantics (no destroy-source step), so
    // the trait that matters for memcpy-as-move is is_trivially_copyable.
    // Leaving the bit definition in place for future cores that genuinely
    // want relocate semantics; computing it would be misleading here.
    return sum_ops{sizeof...(Ts), sum_size, sum_align, flags,
                   alts_table_for<Ts...>};
}

// Convenience: build sum_ops for a specific variant flavor V over Ts....
// V's union storage size/align flow into the descriptor.
template<class V, class... Ts>
inline constexpr sum_ops sum_ops_for =
    make_sum_ops<Ts...>(sizeof(V), alignof(V));

} // namespace detail
} // namespace std
