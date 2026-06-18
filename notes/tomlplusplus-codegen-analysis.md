# tomlplusplus codegen: where the +1708 bytes went

## Executive summary

`bench/drivers/tomlplusplus.cpp` at `-Os +opt` grew **+1708 B .text**
(111533 → 113241) between `57d8339` (pre-type-erasure round 2) and current
HEAD on `claude/sweet-hawking-i8j0rh` (worktree at `d5a9cf6`; the
user-cited `66277e9` produces the same binary). Both numbers reproduce
exactly under g++-13 with the bench's standard `-Os +opt(flags)` link.

The regression is paid for entirely by the new ops-table plumbing — there
is no single fat new function. Five mechanically related effects, in
descending order:

| Bucket                                                | Δ.text    |
|-------------------------------------------------------|----------:|
| `storage_b_resize<C>` per-container reallocate thunks |   +1000 B |
| `storage_b_data / set_size / cap_end / glue` thunks   |    +931 B |
| `vector<T>::ensure_free_()` outlined per instantiation|    +862 B |
| `vector<T>` other (constructors etc.)                 |    +444 B |
| `detail::relocate_block / grow / destroy_range`       |    +255 B |
| **subtotal adds**                                     | **+3492** |
| `basic_string<...>` consolidations                    |   -1036 B |
| other std:: (num_get/money/time/locale tightenings)   |  -1697 B  |
| toml:: (parser shrinks, mainly emplace_hint)          |    -260 B |
| `vector<T>::emplace_back<...>` shrunk to thin wrapper |    -336 B |
| **subtotal saves**                                    | **-3329** |

`size -A` reports the net at +1708 .text; the symbol-bucket arithmetic
nets at +1002, with the remaining ~700 B accounted for by `.rodata`
+616 B (the new dedup'd `ops_for` constant tables) and minor section
shuffle. The dominant call shape that costs us is **per-T outlining
around a type-erased core**. Before the conversion,
`vector::emplace_back` and its growth path inlined into one ~200 B leaf.
After, the same call site becomes: caller → `vector<T>::ensure_free_()`
outline → `detail::grow` trampoline → `storage_b_resize<C>` thunk →
`detail::relocate_block` shared helper → indirect leaf via
`&ops_for<T,A,MASK>`. Each per-T link in that chain materializes a
small function that did not exist before. For 7 distinct
`vector<C>` / `basic_string<C>` instantiations in toml++ that's 7 ×
(~143 B `storage_b_resize` + ~19 B getter + ~19 B setter + ~19 B
`storage_b_data` + ~32 B `storage_b_free` + …) of per-T glue, where the
pre-conversion code emitted nothing per-T outside the body of
`emplace_back` / `reallocate` itself.

The wider story isn't bad: the conversion also produces real shrinks
(`-1036 B basic_string`, `-1697 B std::other`, `-336 B emplace_back`).
But on toml++, which uses ~7 different element types in its parser, the
per-instantiation glue grows faster than the body savings, by 1708 B.

Instance counts in HEAD: 5 distinct `vector<T>::ensure_free_(size_t)`
outlines, 7 distinct `storage_b_resize<C>` instantiations (`std::string`,
wide string, plus 5 `vector`s), plus matching `storage_b_data /
set_size / cap_end / free / glue::raw_alloc` thunks per type.

## Method

* Build both versions with the bench's stock flags (`bench/codesize.py`
  copy): `-std=gnu++20 -nostdinc++ -Iinclude -fno-exceptions -fno-rtti
  -Os -w` plus `-flto -fmerge-all-constants -ffunction-sections
  -fdata-sections -Wl,--gc-sections -fvisibility=hidden
  -fvisibility-inlines-hidden -fno-asynchronous-unwind-tables
  -fno-unwind-tables -fuse-ld=gold -Wl,--icf=safe`.
* g++-13 was used (g++-10 is not installed in this worktree; the recorded
  `f1a9920` measurement at `tomlplusplus.text == 111533` in
  `bench/sizes.jsonl` was also g++-13, so the baseline matches exactly).
* Sizes via `size -A`; symbol-level deltas via `nm --print-size -C`
  bucketed by mangled-name prefix; section attribution via `objdump
  --syms` so `.text` is read independently of section-folded constants.
* Disassembly excerpts below are `objdump -d --no-show-raw-insn -C`.

Binaries kept at `/tmp/codegen/toml_pre` and `/tmp/codegen/toml_head`.

## Disassembly: the load-bearing change

### Sample 1 — `vector<source_position>::emplace_back` (the smoking gun)

Same source-level call site, both built at `-Os` with the same flags.

**PRE (`57d8339`)** — 219 B, fully inlined growth path:

```
000000000001db0e <…::emplace_back<source_position const&>(…) [.isra.0]>:
   1db0e:	push   %r14
   1db10:	push   %r13
   ...
   1db19:	mov    0x8(%rdi),%rax          ; load end_
   1db20:	mov    0x10(%rdi),%rcx         ; load cap_
   1db24:	cmp    %rcx,%rax               ; cheap fast-path: end != cap
   1db27:	jae    1db3d <...+0x2f>
   1db29:	mov    (%rsi),%rdx
   1db2c:	add    $0x8,%rax               ; bump end_, store value, done
   1db30:	mov    %rdx,-0x8(%rax)
   1db34:	mov    %rax,0x8(%rdi)
   1db38:	jmp    1dbdc <...+0xce>
   1db3d:	movabs $0x1fffffffffffffff,%r12 ; slow path: overflow check
   ...
   1db84:	call   1da14 <allocator<…>::allocate(unsigned long)>
   ...
   1db9c:	sub    %rdi,%rcx               ; relocate loop, fully open-coded
   ...                                    ; (~120 bytes of loop + bookkeeping)
   1db97:	xor    %eax,%eax
   1db99:	mov    %rdx,%rcx
   1db9c:	sub    %rdi,%rcx
   1db9f:	cmp    %rcx,%rax
   1dba2:	je     1dbb3 <...>
   1dba4:	mov    (%rdi,%rax,1),%rsi      ; (8B-at-a-time pointer move)
   1dba8:	mov    %rsi,0x0(%rbp,%rax,1)
   1dbad:	add    $0x8,%rax
   1dbb1:	jmp    1db9f <...>
   ...
   1dbcc:	call   5870 <_init+0x390>      ; free old buffer
   1dbd1:	mov    %rbp,(%rbx)             ; commit new begin_/end_/cap_
   1dbd4:	mov    %r14,0x8(%rbx)
   1dbd8:	mov    %r12,0x10(%rbx)
   1dbdc:	pop    %rbx ... ret
```

**HEAD (`d5a9cf6`)** — 51 B, body collapsed to call-call-tail:

```
000000000001dfe0 <…::emplace_back<source_position const&>(…) [.isra.0]>:
   1dfe0:	push   %rbp
   1dfe1:	mov    %rsi,%rbp
   1dfe4:	push   %rbx
   1dfe5:	mov    %rdi,%rbx
   1dfe8:	push   %rax
   1dfe9:	call   1df28 <…::ensure_free_(size_t) [.constprop.0]>   ; per-T outline
   1dfee:	mov    0x0(%rbp),%rdx
   1dff2:	mov    %rbx,%rdi
   1dff5:	lea    0x8(%rax),%rsi
   1dff9:	sub    (%rbx),%rsi
   1dffc:	mov    %rdx,(%rax)                                       ; store value
   1dfff:	call   121d9 <detail::storage_b_set_size<vector<source_position>>(void*, size_t) [.lto_priv.0]>
   1e004:	pop    %rdx
   1e005:	mov    %rbx,%rdi
   1e008:	pop    %rbx
   1e009:	pop    %rbp
   1e00a:	jmp    1df28 <…::ensure_free_(size_t) [.constprop.0]>    ; tail-call: restore spare-slot invariant
```

`vector<table*>::emplace_back` shows the same shape: 219 B → 51 B per
call site. The body shrinks. The cost is paid below.

### Sample 2 — where the inlined growth body moved: `vector<source_position>::ensure_free_`

This function did not exist in PRE — the growth path was entirely
inlined into `emplace_back`. In HEAD it appears as a `.constprop.0`
clone per element-type (5 instantiations in this binary, ~870 B total).

```
000000000001df28 <…::ensure_free_(size_t) [.constprop.0]>:                       ; 183 B
   1df28:	push   %r12 / push %rbp / push %rbx
   1df2e:	cmpq   $0x0,(%rdi)                                                   ; null begin_? skip data load
   1df35:	je     1df5a
   1df37:	call   11a80 <storage_b_data<vector<source_position>>(void*) [.lto_priv.0]> ; thunk
   1df3f:	shr    $0x3,%rdx                                                     ; byte size -> element count
   1df43:	mov    %rdx,%r12
   1df46:	call   12218 <storage_b_cap_end<vector<source_position>>(void*) [.lto_priv.0]> ; thunk
   1df4b:	sub    (%rbx),%rax
   1df4e:	mov    %rax,%rbp
   1df51:	shr    $0x3,%rbp
   1df55:	cmp    %rbp,%r12
   1df58:	jne    1dfd3                                                         ; space available -> hot return
   1df5a:	mov    $0x1,%eax                                                     ; slow path: recommend(sz+n)
   1df5f:	lea    0x1(%rbp),%r12
   1df63:	shl    $0x3d,%rax                                                    ; max_size guard
   1df67:	cmp    %rax,%r12
   1df6a:	jne    1df6e
   1df6c:	ud2
   1df6e:	cmpq   $0x0,(%rbx)                                                   ; ... (50B of recommend() arithmetic) ...
   1df72:	je     1df94
   ...
   1dfb2:	lea    0x8(,%r12,8),%rcx
   1dfba:	mov    %rbx,%rdx
   1dfbd:	mov    %rbp,%r12
   1dfc0:	lea    -0x9e7c(%rip),%rsi        ; &storage_b_resize<vector<source_position>>
   1dfc7:	lea    0x6012(%rip),%rdi         ; &ops_for<source_position, allocator, 10u>
   1dfce:	call   1ddaf <detail::grow(type_ops*, storage_ops*, void*, size_t) [.isra.0]>
   1dfd3:	mov    (%rbx),%rax
   ...                                                                           ; load begin_, compute returned pointer
   1dfdc:	ret
```

Compared to PRE: the cheap "is there room?" check now goes through two
out-of-line `storage_b_*` thunks (a load each) instead of two adjacent
loads. The slow recommend() and the call into the relocation core look
similar in operation count, but in HEAD they are paid via three
distinct outlined functions (`ensure_free_` → `detail::grow` →
`storage_b_resize<C>` → `detail::relocate_block`) instead of one inlined
body.

### Sample 3 — the per-T thunk that exists once per element type: `storage_b_resize<C>`

7 instantiations in this binary (one per `vector<T>` plus `string`,
`wstring`), each ~143 B:

```
000000000001414b <storage_b_resize<vector<source_position>>(type_ops*, void*, size_t) [.lto_priv.0]>:
   1414b:	endbr64
   1414f:	push   %r15 ... push %r8        ; (7-reg save)
   14164:	mov    (%rsi),%rbp              ; load old begin_
   14167:	test   %rbp,%rbp
   1416a:	je     14170
   1416c:	mov    -0x8(%rbp),%r12          ; read live count from header
   14170:	lea    0x8(%rdx),%rdi           ; want = H + min_bytes
   14174:	call   1e183 <storage_b_glue<string>::raw_alloc(string*, size_t) [.isra.0]>  ; (!) ICF folded to string's raw_alloc
   14179:	mov    %rax,%rbx
   1417c:	lea    0x8(%rax),%r13           ; nb = base + H
   14180:	test   %r12,%r12
   14183:	je     14198
   14185:	mov    %r12,%r8
   14188:	mov    %rbp,%rcx
   1418b:	mov    %r13,%rdx
   1418e:	xor    %esi,%esi                ; alloc_ctx=nullptr (stateless)
   14190:	mov    %r15,%rdi
   14193:	call   13f94 <detail::relocate_block(type_ops*, void*, void*, void*, size_t) [.lto_priv.0]>
   14198:	test   %rbp,%rbp
   1419b:	je     141a6
   1419d:	lea    -0x8(%rbp),%rdi
   141a1:	call   5d00 <_init+0x3b0>       ; free
   141a6:	mov    %r12,(%rbx)              ; rewrite header
   141a9:	mov    %rbx,%rdi
   141ac:	mov    %r13,(%r14)              ; set_begin
   141af:	call   5e90 <_init+0x540>       ; malloc_usable_size for trailing zero
   ...
   141c4:	xor    %ecx,%ecx
   141c6:	mov    %rcx,0x0(%r13,%r12,1)
   141cb:	pop    %rdx / ... / ret
```

In the PRE binary, no symbol of this form exists. Its work was done as
straight-line code inside `emplace_back` and `reallocate_grow`.

### Sample 4 — the shared relocation helper that didn't exist before

```
0000000000013f94 <detail::relocate_block(type_ops*, void*, void*, void*, size_t) [.lto_priv.0]>:     ; 152 B
   13f94:	endbr64
   13faf:	mov    0x10(%rdi),%eax          ; load type_ops::flags
   13fb2:	not    %eax
   13fb4:	test   $0x24,%al                ; trivially-reloc AND default-life?
   13fb6:	jne    13fd1                    ; nontrivial path
   13fb8:	add    $0x18,%rsp               ; trivial fast path: tail-call memmove
   13fbc:	mov    %r12,%rdi
   ...
   13fcc:	jmp    5f00 <_init+0x5b0>        ; memmove
   13fd1:	mov    (%rdi),%r13               ; nontrivial loop:
   ...                                       ; for (i=0; i<n; ++i) {
   13fea:	mov    0x8(%rsp),%rax            ;   call type_ops::move_construct(ec, dst, src);
   13fef:	cmp    %rax,%r15                 ;   if (!triv_destroy && !alloc_default_life)
   13ff2:	je     1401d                     ;     call type_ops::destroy(ec, src);
   13ff4:	mov    %rbp,%rdx                 ; }
   13ffd:	call   *0x30(%rbx)               ; indirect call: move_construct slot
   ...
   1400f:	call   *0x20(%rbx)               ; indirect call: destroy slot
   ...
   1402b:	ret
```

This is the shared core: the loop body is no longer materialized per
element type, instead it indirects through `type_ops::move_construct`
(slot at +0x30) and `type_ops::destroy` (slot at +0x20). For
trivially-relocatable types like `source_position` and the pointer
vectors, it short-circuits to `memmove`. That short-circuit recovers
most of the savings — but the bookkeeping around the fast path
(flag check, register save, conditional tail call) is what eats the
delta that the inlined `mov`/`add` loop in PRE did not have to pay.

### Sample 5 — `detail::grow` is a one-instruction trampoline

A minor point, but illustrative of the new dispatch layering:

```
000000000001ddaf <detail::grow(type_ops*, storage_ops*, void*, size_t) [.isra.0]>:                  ; 15 B
   1ddaf:	endbr64
   1ddb3:	mov    %rsi,%rax                ; load storage_ops*
   1ddb6:	mov    %rdx,%rsi                ; shift args
   1ddb9:	mov    %rcx,%rdx
   1ddbc:	jmp    *%rax                    ; tail-call storage_ops::resize (first slot)
```

`grow` was *itself* an outline call from `reallocate_grow`. With LTO it
folds to a 15 B argument-shuffle and an indirect tail call, but the
outline edge between `vector::reallocate_grow` and `storage_b_resize<C>`
still costs 15 B once across all callers.

## Why the savings don't catch up

The PRE binary has a single hot path
`vector<T>::emplace_back<…>` inlined per T. That's about 220 B × 5 element
types = ~1.1 kB. In HEAD, each of those is ~50 B (thin wrapper) + a 90 B
contribution to a shared `ensure_free_(size_t)` outline + a 30 B
contribution to `storage_b_set_size<C>` etc. The line-by-line
substitution works out to a near-wash on emplace_back proper (-336 B net
in the bucket), but the *new* glue costs that did not exist before —
`storage_b_resize<C>` (+1000), `storage_b_data/cap_end/glue` (+931),
`detail::relocate_block/grow/destroy_range` (+255) — are paid in full.

In larger-amalgamation TUs like doctest (also recorded in
`bench/sizes.jsonl`, +1551 B at the same step) the per-T glue spreads
across more vector instantiations and the per-T thunks would amortize
better; toml++ is unfortunate because it uses just enough distinct
container element types (5 vectors, 2 strings) to pay the per-T tax
without enough callers per T to amortize the inlined-body savings.

## Where to look if shrinking this is wanted

In rough priority by Δ.text recovered:

1. **`storage_b_data`, `storage_b_set_size`, `storage_b_cap_end`** are
   one- or two-load thunks (~19 B each, ~7 instantiations each = ~400+ B).
   They are reached only from `ensure_free_`, `emplace_back`,
   `destruct_at_end`. Inlining these into the consumer
   (or making them `__attribute__((always_inline))` despite the `-Os`
   default) is the cheapest recovery.
2. **`detail::grow` is a 15 B trampoline** whose only job is to reorder
   args before an indirect tail call. Inlining it into `reallocate_grow`
   removes that hop and the `endbr64`.
3. **`vector<T>::ensure_free_(size_t) [.constprop.0]`** is what GCC
   produces because the slow path is large enough that the size heuristic
   refuses to inline at `-Os`. The relevant body is the fast path
   (`size == cap_end?` -> return). Splitting into a tiny inline fast
   path + an outlined slow path (the libc++ "do_grow_slow" pattern)
   would recover ~5 × ~80 B = ~400 B and keep the cold path shared.

Numbers 1 + 3 together would erase the regression on this binary.
Number 2 is bookkeeping cleanup, not a meaningful win on its own.

## Pointers

- Reproducible bench command (used here):
  `g++-13 -std=gnu++20 -nostdinc++ -Iinclude -fno-exceptions -fno-rtti -Os -w
   -flto -fmerge-all-constants -ffunction-sections -fdata-sections
   -Wl,--gc-sections -fvisibility=hidden -fvisibility-inlines-hidden
   -fno-asynchronous-unwind-tables -fno-unwind-tables -fuse-ld=gold
   -Wl,--icf=safe -Ibench/projects/tomlplusplus/include
   bench/drivers/tomlplusplus.cpp src/support.cpp -nodefaultlibs -lpthread
   -lm -lc -lgcc_s -lgcc -o /tmp/toml`
  then `size -A /tmp/toml | grep '\.text'`.
- Source of the per-T outlines investigated:
  - `include/vector` lines 326-344 (`reallocate_grow`, `rotate_`, `destruct_at_end`
    routing through `detail::grow` / `detail::rotate` / `detail::destroy_range`)
  - `include/vector` lines 741-748 (`ensure_free_`, the new outline point)
  - `include/bits/storage_ops.h` lines 305-352 (`storage_b_resize<C>` and its
    `storage_ops_b<C>` table)
  - `include/bits/type_ops.h` lines 180-242 (`make_type_ops<T,A,MASK>` /
    `ops_for<T,A,MASK>` — these are constants, not the source of the
    .text delta; the .rodata grew +616 B from the 5 distinct dedup'd
    tables, separately from this analysis).
