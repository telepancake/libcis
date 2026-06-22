# size-salvage — measured outcome

All numbers from `make size` (`python3 bench/size_slope.py`), x86_64, g++ 13,
overhead vs the non-type-erased reference (host libstdc++, rebuilt every run).
Full results live in `.test_results/latest/` (annotated `.asm`, CSVs).

## What the metric is
- PER-CALL (headline): bytes of code at one call site, `libcis - reference`.
- PER-TYPE (demoted): marginal flash per added instantiation.
- MEMORY: representative workloads — internal / real glibc heap / peak stack.
- STATIC (omitted from digest): one-time shared-core flash; allowed to grow.

## Starting baseline (sweet-hawking cores + this metric)
Per-call (x86_64): push_int +8, push_H +33, insert_H -176, erase_H -41,
assign_H -363, sort_H -113, str_append -205, str_compare +70.
Per-type (vector-only firmware): -23 B/type.
Memory heap_ovh: vec_int +480, vec_big +47200, string +4336, map -144.

The vector-only firmware did not exercise the cross-header duplication the
salvage targets. Extending firmware.cpp to route each element type T through
`any` + `optional` + `variant` (as a real image does) moved per-type to
**+292 B/type** — the honest cross-header baseline, exposing that each vocabulary
emitted its own copy/move/destroy leaf per T.

## Changes and their measured effect

1. **Strategy-B grow base off live size()** (`a8b1287`). malloc_usable_size
   rounds capacity up; doubling it compounded the rounding every grow. Doubling
   `size()` instead. (Pre-existing memory hygiene; kept.)

2. **Unify any/variant/function copy+destroy onto one shared leaf set**
   (`17380e6`). `value_ops` (any), `sum_ops` (variant) and `callable_ops`
   (function) each declared their own per-T `destroy`/`copy_construct`/
   `move_construct` templates — identical signatures and bodies, but distinct
   symbols ICF would not fold (addresses taken into ops tables). Hoisted
   `erased_destroy_op` / `erased_copy_construct_op` / `erased_move_construct_op`
   into `bits/relocatable.h` and pointed all three vocabularies at them.
   Verified in the firmware binary: exactly ONE `erased_destroy_op<T>` and ONE
   `erased_copy_construct_op<T>` per type now, where before there were two or
   three (`alt_*` + `*_small_op` + `func_*`).
   **Measured: per-type +292 -> +265 B/type.** `any`'s small-buffer move keeps a
   private leaf (it also ends the source object's lifetime).

3. **Pack rb-tree color into the parent pointer's low bit** (`3315193`).
   Dropped the separate `bool is_black`, which cost a full 8 bytes per node after
   alignment padding (`node_base` 32 -> 24). Verified clean under ASan+UBSan and
   a 5000-insert / 2500-erase / lookup / copy stress.
   **Measured:** `map<int,int>` heap UNCHANGED (-144) — both 40- and 32-byte
   nodes round to glibc's 40-byte bin, an honest neutral. Added a `map_big`
   workload (pair=16) where the 8-byte shrink straddles a malloc bin:
   **heap -80272 (libcis 239664 vs ref 319936), ~16 B/node × 5000.** The shrink
   is real per node and pays wherever allocator granularity is finer than
   glibc's 16-byte step — the embedded target.

4. **Collapse vector's destroy-only + grow ops tables into one** (`9f25b7a`).
   Destroy and move_construct are always-safe to gate, so they share one
   `OP_DESTROY|OP_MOVE_CONSTRUCT` table; the copy-construct table stays separate
   (its body-level `requires` deep-instantiates the copy ctor and must not be
   named for non-copyable types like `chrono::time_zone`). Two tables per type,
   not three. Verified move-only / chrono / `vector<time_zone>` compile + run.
   **Measured: per-type +265 -> +257 B/type.** Per-call ops unchanged.

## Ending numbers
Per-call (x86_64): unchanged from baseline (no regression on any op).
Per-type (cross-header firmware): **+257 B/type** (down from +292 at the honest
cross-header baseline; -35 B/type from unification + table collapse).
Memory: `map_big` heap **-80272** (new, shows the rbtree node shrink);
`map` -144; vec_big +47200 and string +4336 are the deliberate no-SSO-string
trade (technique #1 — tiny strings always heap-allocate; measured, not hidden).

## What was NOT done, and why (honest)
- **Unifying vector's `type_ops` leaves with the `erased_*` leaves.** The
  `type_ops` lifecycle leaves carry an allocator-ctx first argument
  (`destroy_op(void* ctx, void* p)`) to thread stateful allocators; the erased
  vocab leaves do not (`erased_destroy_op(void* p)`). For the dominant stateless
  `std::allocator` the bodies are identical modulo the unused ctx, but the
  signature mismatch blocks ICF and the cores' calling convention assumes the
  ctx arg. Bridging it cleanly needs a cores-wide calling-convention change with
  poor risk/reward (the duplicated leaf bodies are ~10-20 B; the per-type tables
  are small constant data). Left as-is; flagged rather than forced.
- **`sp_cb_ops` (shared_ptr) / `future_cb_ops` (future).** Their lifecycle ops
  (`on_zero_shared`, `destroy_value`, `invoke`) are domain-specific control-block
  teardown, NOT plain copy/move/destroy, so they do not coincide with the shared
  leaf set. Unifying would be forced; left separate.
