# storage_ops dispatch — prompt #1 (rules / don'ts / "hard rules" style)

This was the second dispatch of the storage_ops task (the first was lost when
the container restarted with no commits in 25 minutes). It was written in
the conventional brief style: list of design facts up front, then a list of
"Hard rules" at the bottom that included explicit prohibitions.

Sent to: `claude` subagent. Base commit: `eccf89b`.

```
Working in /home/user/libcis on branch `claude/sweet-hawking-i8j0rh`. Major architectural refactor: introduce a `storage_ops` table (mirroring `type_ops` in shape) that abstracts the container's buffer state. The previous dispatch of this same task was killed by a container restart — start fresh, commit incrementally (every 5–10 minutes) so a restart doesn't lose work, push frequently.

# What's on the branch (read carefully before starting)

This refactor builds on a long chain of vector/string work. Re-read:
1. `include/vector` end-to-end. Especially: `ops_table_()`, `realloc_shim_`, `to_pointer_`, `vallocate(n)`, `reallocate_grow(N)`, `ensure_free_`, `emplace_back`, `emplace`, `insert`, `erase`. The spare-slot invariant + rotate-based modifier pattern is the load-bearing design.
2. `include/string` similarly — same layout as vector, the null terminator is the spare slot.
3. `include/bits/type_ops.h` — the `type_ops` struct + `OP_*` mask bits + `ops_for<T, A, MASK>` constexpr global. This is the *shape* you're mimicking for storage_ops.
4. `include/bits/vector_cores.h` + `src/support.cpp` — the cores. They currently take `(ops, el_ctx, byte_buf*, …)`; you're replacing the `byte_buf*` arg with `(storage_ops*, st_ctx)`.

# The new design (binding)

## storage_ops table

Define in `include/bits/storage_ops.h` (new file). Lives in `std::detail`:

```cpp
namespace std::detail {

struct storage_ops {
    // The CURRENT valid data range [begin, end). For a populated container,
    // begin == buffer base, end == one past the last constructed element.
    span<unsigned char> (*data)(void* ctx);

    // Set the new size in BYTES. Updates end_ (Strategy A) or the header
    // (Strategy B). Precondition: bytes <= (cap_end - data().data()).
    // IMPORTANT: this slot ALSO writes sizeof(CharT)-many zero bytes at the
    // new end position (the "spare slot zeroing" optimization). string's
    // null-terminator falls out for free; vector pays a few bytes of zero
    // it doesn't care about. The exact width of the zero is sizeof(CharT)
    // for string-likes, sizeof(value_type) for vector-likes — make the
    // shim Generic by zeroing `min(spare_bytes, sizeof_value)` or just
    // always sizeof(value_type) since both work.
    void (*set_size)(void* ctx, size_t bytes);

    // One past the allocation: data().data() + cap_bytes. Used by precondition
    // checks ("is this pointer inside the buffer?") and by grow paths.
    void* (*cap_end)(void* ctx);

    // (Re)allocate to AT LEAST min_bytes (the storage_ops shim is responsible
    // for adding the spare-slot +1 internally to keep the invariant intact,
    // AND for zeroing the spare slot at the new end). Returns the new base
    // pointer (== data().data() after the call).
    void* (*resize)(void* ctx, size_t min_bytes);

    // Free the entire buffer and reset ctx to empty state.
    void (*free)(void* ctx);
};

template<class Container>
constexpr storage_ops make_storage_ops();

template<class Container>
inline constexpr storage_ops storage_for = make_storage_ops<Container>();

} // namespace std::detail
```

## Two layout strategies

The container picks at compile time via SFINAE on the allocator. Concept:
```cpp
template<class Alloc>
concept allocator_with_usable_size = requires(typename Alloc::value_type* p) {
    { ::malloc_usable_size(p) } -> std::convertible_to<std::size_t>;
};
```
For std::allocator on glibc, `malloc_usable_size` is available — use Strategy B. For fancy / pmr / other allocators — use Strategy A.

### Strategy A (current, 3 pointers)
- Container layout: `[[no_unique_address]] Alloc alloc_; pointer begin_, end_, cap_;` — 24 B + alloc EBO.
- Shim slots:
  - `data` → `{begin_, end_ - begin_}` (span construction).
  - `set_size` → `end_ = begin_ + bytes; *end_ = value_type{};` (the spare-slot zero).
  - `cap_end` → `cap_`.
  - `resize` → `alloc_traits::allocate_at_least(min_bytes / sizeof(T) + 1)` (the +1 is the spare slot), copies bytes, updates begin_/end_/cap_, zeros the new end_, frees old.
  - `free` → `alloc_traits::deallocate(begin_, cap_ - begin_); begin_ = end_ = cap_ = nullptr;`

### Strategy B (new, 1 pointer)
- Container layout: `[[no_unique_address]] Alloc alloc_; pointer begin_;` — 8 B + alloc EBO.
- Allocation layout: `[header: size_t size, padding][element data...]`. `begin_` points at the start of element data.
- Header size: `max(sizeof(size_t), alignof(value_type))` rounded up. Critical to get right.
- Empty state: `begin_ == nullptr`.
- Shim slots:
  - `data` → if `begin_ == nullptr`: empty span. Else: read size from `begin_[-header_size]` cast to size_t*, return `{begin_, size}`.
  - `set_size` → `*reinterpret_cast<size_t*>(begin_ - header_size) = bytes; *(begin_ + bytes / sizeof(T)) = value_type{};` (zero the spare slot).
  - `cap_end` → if `begin_ == nullptr`: nullptr. Else: `begin_ + (::malloc_usable_size(begin_ - header_size) - header_size)`.
  - `resize` → allocate `min_bytes + sizeof(T) + header_size` (header + bytes + spare slot), write header, return base = (raw_alloc + header_size); free old if present; zero the spare slot at the new end.
  - `free` → `::free(begin_ - header_size); begin_ = nullptr;`.

## Core API change

Migrate cores in `src/support.cpp` + `include/bits/vector_cores.h`:

Before: `void grow(const type_ops* tops, realloc_op realloc, void* st_ctx, void* el_ctx, byte_buf* buf, size_t min_cap_bytes)`
After: `void grow(const type_ops* tops, void* el_ctx, const storage_ops* sops, void* st_ctx, size_t min_cap_bytes)`

Inside core bodies, replace `buf->begin/end/cap` access with `sops->data(st_ctx)` / `sops->cap_end(st_ctx)`. After resize, call `sops->resize(st_ctx, …)`.

Drop `realloc_op` from `bits/type_ops.h` — `storage_ops::resize` subsumes it. The old `realloc_shim_` member function on vector goes away.

Same migration for: `rotate`, `grow_with_gap`, `destroy_range`, `construct_copy_one_n`, `construct_default_n`. (`open_gap` is unused, drop it.)

## Precondition checks land in the cores

While migrating, add the precondition checks:
- `rotate`: `if (first > middle || middle > last) __builtin_trap();`. Also `if (first < data.begin() || last > data.end()) __builtin_trap();`. (last <= data.end(), not cap_end, because rotate operates on the constructed range.)
- `grow`: assert sane sops state.
- `destroy_range`: assert `begin <= end`, both within `data()` extent.
- `construct_copy_one_n`: assert `dst + n*size <= cap_end()`.

Document each check in the commit message. Then strip the corresponding per-callsite range checks from string (the `cmp; jae; ud2` sequences in erase/insert/replace/at/substr that catch pos > size). string sees these as a code-size win.

## Container migration

Vector and string:
1. Remove `realloc_shim_`, `to_pointer_`, `ops_table_`, `el_ctx_`, `byte_buf` references, `vallocate(n)`, `reallocate_grow(N)`, `vdeallocate`, `ensure_free_`. They were the old plumbing.
2. Public methods become forwarders calling `storage_for<Container>.X(this, ...)`.
3. Layout selection inside vector class body: conditionally include `pointer end_, cap_;` based on `allocator_with_usable_size<Allocator>`. Strategy A has them; Strategy B doesn't.
4. **No more explicit null-term writes in string** — the storage_ops shim's `set_size` and `resize` zero the spare slot. Remove every `*end_ = CharT(0)` from string's body.

## Gates (all required)

1. **Smoke**: probe exercising every modifier on `vector<int, std::allocator<int>>` (Strategy B), `vector<int, min_allocator<int>>` (Strategy A), and same for `basic_string`. Run clean.
2. **sizeof check**: `static_assert(sizeof(vector<int, std::allocator<int>>) == 8)`, same for `string`. For `vector<int, min_allocator<int>>` it stays ≈ 24 (depends on `min_allocator` size with its EBO).
3. **bench/codesize.py + bench/record.py "storage_ops: 1-pointer std::allocator containers + precondition checks in cores"** — measure and `--diff eccf89b`. Report per-project deltas.
4. **Use the existing /tmp/perf2.cpp** (a perf bench from before) — rebuild + run + compare to `eccf89b`.

# Hard rules

- **Commit & push every 5–10 minutes of work** so a container restart doesn't lose everything. Even partial commits are better than zero. The previous attempt was killed by a container restart after 25 minutes with zero pushes.
- Keep the spare-slot invariant. The storage_ops shim is responsible for it.
- Type-erased dispatch via `storage_ops`. NO direct field access from cores.
- Strategy is picked at COMPILE time per (Container, Allocator).
- Stay on `claude/sweet-hawking-i8j0rh`. Push with `git push -u origin claude/sweet-hawking-i8j0rh` (retry 4× exponential backoff on network errors). No PR.

# Report

- New file `include/bits/storage_ops.h`.
- `bits/type_ops.h` / `bits/vector_cores.h` / `src/support.cpp` changes (lines).
- `include/vector` / `include/string` lines deleted/added.
- Bench diff per-project .text vs `eccf89b`.
- perf2 diff per-op timings vs `eccf89b`.
- `sizeof` verification.
- Commit SHAs pushed (multiple — incremental commits are encouraged).
- Any unexpected costs or wins.
```
