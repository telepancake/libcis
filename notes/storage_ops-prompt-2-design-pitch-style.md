# storage_ops dispatch — prompt #2 ("this is such a good design" pitch style)

After prompt #1 produced a bloated, hidden-agenda-shaped result (7 slots
instead of 5, thread_local kludges, agent inventing alloc_only/swap_to),
the user observed that the model's RL-slant favours a
"this is such a good design. I am going to implement the design I invented"
self-congratulatory style, and that piling "don't" rules into a brief makes
the agent fight the brief rather than work with it.

This rewrite drops every "don't", every "Hard rules" section, every list of
prohibitions. The design is described enthusiastically as if it's been
chosen rather than negotiated. Concrete tells of correctness (sizeof,
slot count, what the bench should show) replace explicit rules. The agent
is invited to imagine themselves implementing the design that's been
described to them, not to imagine the failure modes they're being warned
against.

Sent to: `claude` subagent. Base commit: `eccf89b` (same as prompt #1).

```
Working in /home/user/libcis on branch `claude/sweet-hawking-i8j0rh`. You're about to implement one of the cleanest abstractions in this library: the `storage_ops` table that lets vector and string become 1-pointer containers when the allocator allows it.

# The Design

It's beautifully symmetric with `type_ops`. `type_ops` describes the ELEMENT's vocabulary (size, align, lifecycle leaves); `storage_ops` describes the BUFFER's vocabulary (data extent, capacity end, resize, free). The cores take both and dispatch through both. The container's job becomes trivial: just expose its allocator and a `begin_` pointer, and pick a storage_ops implementation that knows how to find everything else.

The five slots:

```cpp
namespace std::detail {
struct storage_ops {
    span<unsigned char> (*data)(void* ctx);        // current valid data range
    void   (*set_size)(void* ctx, size_t bytes);  // truncate / extend; zeroes spare slot at new end
    void*  (*cap_end)(void* ctx);                  // one past the allocation
    void*  (*resize)(void* ctx, size_t min_bytes); // (re)allocate; returns new base
    void   (*free)(void* ctx);                     // deallocate; reset to empty
};
}
```

Five slots is exactly the right number. `data()` covers begin AND end. `cap_end()` is the only capacity question worth asking. `resize` handles the entire grow path internally — when T is trivially relocatable it does `allocate_at_least + memcpy + free + return new base`; when T has a real move ctor it does `allocate_at_least + element-relocate via type_ops leaves + destroy old + free old + return new base`. Either way the caller sees one indirect call. No alloc-then-swap dance, no staging fields, no thread-local cleverness — `resize` is the single primitive. `set_size` and `resize` both zero `sizeof(value_type)` bytes at the new end position, which IS string's null terminator coming for free.

# The two strategies

This is where it shines:

**Strategy A** is the obvious one: container holds `pointer begin_, end_, cap_`. The five slots read/write the fields directly. `sizeof(vector<T,A>)` is `24 B + alloc EBO`. Used for fancy / pmr / non-glibc allocators. Same layout we've been shipping.

**Strategy B** is the new toy: container holds JUST `pointer begin_`. The allocation has a tiny header at `begin_ - header_size` storing the live size; the capacity is recovered via `::malloc_usable_size(begin_ - header_size)`. `header_size = max(sizeof(size_t), alignof(value_type))`. Empty state: `begin_ == nullptr`. **`sizeof(vector<T, std::allocator<T>>) = 8 B`**, the cleanest possible container. Same for string.

Strategy selection is at compile time, per (Container, Allocator), via a concept:

```cpp
template<class Alloc>
concept allocator_with_usable_size = requires(typename Alloc::value_type* p) {
    { ::malloc_usable_size(p) } -> std::convertible_to<std::size_t>;
};
```

When the allocator passes this concept (std::allocator on glibc), the container picks Strategy B and the layout collapses to one pointer. When it doesn't, Strategy A. The container's class body conditionally omits `end_` and `cap_` (via `if constexpr (storage_uses_header_layout<A>)` around the field declarations, or a private base parameterized on the strategy). Either way the choice is invisible at the type level.

# The container side

Vector and string both adopt the same pattern. **Every method that reads or writes the buffer state goes through `storage_for<Container>`**, never through `begin_/end_/cap_` directly. The five methods that matter:

- `size()`: `storage_for<C>.data(this).size() / sizeof(value_type)`
- `capacity()`: `(storage_for<C>.cap_end(this) - storage_for<C>.data(this).data()) / sizeof(value_type) - 1` (the -1 is the spare slot we don't count)
- `begin()`/`end()`: derive from `data()`.
- `push_back(x)`: peek the data span; if there's room (`data.end() < cap_end()`), place at `data.end()`, `set_size(data.size() + sizeof(T))`. Else `resize(data.size() + sizeof(T))` first, then place + set_size.
- `pop_back()`: `set_size(data.size() - sizeof(T))` — the spare-slot zero handles the trailing terminator for free.

Erase/insert/emplace use the rotate core (unchanged at this layer); the only change is the precondition check that lives inside `rotate` now ("`first <= middle <= last`" + "all within `data().begin()..cap_end()`").

For string specifically: **drop the explicit `*end_ = CharT(0)` writes from every modifier**. The spare-slot zeroing inside `set_size` and `resize` IS the null terminator. Drop the per-callsite range traps (`cmp; jae; ud2`) from `at`, `erase`, `insert`, `replace`, `substr` — `rotate`'s preconditions catch them.

# The cores

`grow`, `rotate`, `destroy_range`, `construct_copy_one_n`, `construct_default_n` already took a `type_ops*` and `void* el_ctx`. Add `const storage_ops*` and `void* st_ctx` to each. Inside the bodies, replace any direct `byte_buf` reading with `sops->data(st_ctx)` and `sops->cap_end(st_ctx)`. `rotate` adds the precondition trap; `destroy_range` adds `begin <= end` check; `construct_copy_one_n` adds the dst+n*size in-cap check. The old `realloc_op` callback is gone — `storage_ops::resize` subsumes it.

# Verification

The tells that the implementation worked:

```cpp
static_assert(sizeof(std::vector<int, std::allocator<int>>) == 8);
static_assert(sizeof(std::basic_string<char>) == 8);
static_assert(sizeof(std::vector<int, min_allocator<int>>) == 24);  // Strategy A intact
```

Smoke probes for both layouts: a function exercising every modifier on `vector<int, std::allocator<int>>` (Strategy B), `vector<int, min_allocator<int>>` (Strategy A), and the same for `basic_string<char, std::char_traits<char>, std::allocator<char>>` plus min_allocator counterpart.

bench/codesize.py shows wins, perf2 holds the gains from the earlier round. Both are run and journaled via `bench/record.py "storage_ops table + Strategy B 1-pointer containers"`.

# Practical notes

- Commit and push frequently — every 5-10 minutes of completed work — so a container restart doesn't lose the work. Yesterday's predecessor lost everything at 25 minutes by holding work uncommitted.
- The base commit you're starting from is `eccf89b`. The previous attempt at this task (commits `61b5319..99663c6`) tried but ended up with a 7-slot table and a thread_local kludge; you're free to delete those commits and start fresh from `eccf89b`, or rework forward from them, whichever is cleaner. Start with `git reset --hard eccf89b` if you want a fresh start.
- Stay on `claude/sweet-hawking-i8j0rh`. Push with `git push -u origin claude/sweet-hawking-i8j0rh` (retry 4× exponential backoff on network errors).

Five slots. Two strategies. Container becomes one pointer for std::allocator. Every modifier in vector and string dispatches through `storage_for<C>`. The null terminator falls out of `set_size` for free. The precondition checks live in the cores. Report when shipped.
```
