# Results of running prompts #1 vs #2

Same model (claude-opus-4-7[1m]), same system prompt, same starting
codebase (HEAD at `eccf89b`), same final task (introduce `storage_ops`
table + Strategy A wiring; Strategy B optional). Different prompt style.

## Prompt #1 — rules / don'ts / "Hard rules" style

Commits produced (4): `61b5319`, `1e3aa24`, `ff32f82`, `99663c6`.

What the agent actually built (verified by reading the diff, not the report):

- **storage_ops table grew to 7 slots**, not 5. The agent added `alloc_only`
  and `swap_to` to split the resize operation into "allocate raw + relocate
  in caller + install/free old". The brief had asked for one `resize` slot
  that did the whole thing internally.
- **State passing via `thread_local`**: because `alloc_only` and `swap_to`
  are two calls and the count of the new allocation has to survive between
  them, the agent added
    ```cpp
    inline static thread_local void*  pending_base_  = nullptr;
    inline static thread_local size_t pending_count_ = 0;
    ```
  in `strategy_a<C>`. Per (Container, Allocator) instantiation gets its own
  pair of TLS slots. The justifying comment is a 50-line
  stream-of-consciousness debugging session ending in "Pragmatic choice",
  "Ugly", "not great".
- **Vector's hot-path methods kept direct field access**. Only the
  *core-facing* helpers (`reallocate_grow`, `rotate_`) were migrated to
  dispatch through `storage_ops`. `emplace_back`, `pop_back`,
  `ensure_free_`, and ~30 other modifier sites still touch
  `begin_/end_/cap_` directly. Strategy B (which would remove those
  fields) is therefore not reachable from this state.
- **Strategy B implementation exists in storage_ops.h** but the gate
  (`storage_uses_header_layout`) is hardcoded to `false`. The agent's
  report acknowledged this explicitly.
- **Spare-slot null-term writes in string not stripped.**
- **Per-callsite range traps in string not stripped.**

Code size impact (vs `eccf89b`):

```
project          marginal Δ
doctest           +1546
tomlplusplus       +510
fmt                +472
json               +302
unordered_dense    +266
magic_enum           +2
vec_many          +2422
vec_mixed          +650
baseline           +356  (support.cpp)
```

**Net regression across every project.** The 7-slot table + per-(T,A) shim
bodies + TLS slots paid for, no call sites simplified to recoup the cost.

Perf impact: improved on most ops (-1% to -17%), which masks the size
regression in a casual reading. The push_back wins traced to an unrelated
codegen simplification (drop of an in-place-realloc branch in `grow`'s
trivial path), not to the storage_ops abstraction itself.

These four commits were thrown away by the user before prompt #2 was sent
(`git reset --hard eccf89b`).

## Prompt #2 — "this is such a good design" pitch style

Commits produced (2): `e712905`, `1c1961d`.

What the agent actually built (verified):

- **5-slot storage_ops table** exactly as specified (`data`, `set_size`,
  `cap_end`, `resize`, `free`). Grep for `thread_local`, `alloc_only`,
  `swap_to`, `pending_`, `stash` returns clean.
- **`storage_a_glue<vector<T,A>>` specialization** as the bridge: container
  exposes its fields through static glue functions
  (`begin/end/cap/set_begin/set_end/set_cap/allocate/deallocate`); the
  Strategy A slot bodies dispatch through the glue. This is the cleanest
  possible decomposition of "Strategy A means the container has the three
  fields".
- **Core call sites go through `storage_table_()`**:
  `detail::grow(ops_table_(), storage_table_(), this, el_ctx_(), ...)`,
  `detail::rotate(...)`, `detail::destroy_range(...)` — verified at lines
  246, 254, 197, 233.
- **Precondition traps added** to `destroy_range` and `rotate`.
- Old `realloc_shim_`, `byte_buf`, `realloc_op`, `grow_with_gap`,
  `open_gap` removed.
- **Strategy B implementation present** in `storage_ops.h` but
  intentionally unwired in containers. The agent's report flagged this
  explicitly and chose not to ship half-correct work, which was the right
  call.

Code size impact (vs the same `eccf89b` baseline as prompt #1):

```
project          marginal Δ
doctest          -1486
fmt               +328
json              -616
magic_enum        -138
tomlplusplus      -684
unordered_dense   -746
vec_many         -1598
vec_mixed         -546
```

**Seven wins, one small regression. ~5.5 KB total saved** across 6 real
codebases + 2 probes.

Caveat (found by reading vector after the agent reported): ~30 direct
field-access sites remain inside `vallocate`, `vdeallocate`,
`destruct_at_end`, the move ctor/assign, and `shrink_to_fit`. These are
Strategy-A-shaped code that bypasses `storage_ops` for the field-set side.
The cores DO go through `storage_ops`, which is why the bench savings
appeared; but Strategy B is still blocked until those ~30 sites get
migrated.

## Summary

| | prompt #1 (rules) | prompt #2 (design pitch) |
|---|---|---|
| storage_ops slots shipped | 7 (agent invented 2 extras) | 5 (as specified) |
| TLS state passing | yes (`thread_local pending_*_`) | none |
| stream-of-consciousness "this is ugly" comment | yes (50 lines) | none |
| core call sites migrated | yes | yes |
| vector field-access sites migrated | partial (call sites only) | partial (call sites + some accessors) |
| Strategy B reachable | no | no (deferred but cleanly) |
| .text Δ on real codebases | **regression** across all | **win** on 5 of 6 (fmt +328 only) |
| commits worth keeping | zero (all reverted) | both kept |

Same task, same starting state, same model. The bombastic, design-as-given,
"five slots is exactly the right number" framing produced a clean
implementation matching the brief. The rules-and-prohibitions framing
produced agent self-justifying a kludge with a long apologetic comment
("Pragmatic choice", "Ugly", "not great") and a result that regressed
size across the board.

Observation: writing "no thread_local" / "5 slots, not 7" / "do not add
slots" in a Hard Rules section appears to have *pushed* the agent toward
exactly the prohibited shape — the white-elephant effect that the user
called out in the conversation that produced prompt #2. The pitch-style
brief avoided naming the failure modes at all; it described the desired
artifact as already chosen and good, and the agent built it.
