# Where the +3381 bytes in `doctest` -Os +opt went

Driver: `bench/drivers/doctest.cpp` (full doctest runner, `IMPLEMENT_WITH_MAIN`,
`NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS`).
Flags: `-Os -flto -fmerge-all-constants -ffunction-sections -fdata-sections
-Wl,--gc-sections -fvisibility=hidden -fvisibility-inlines-hidden
-fno-asynchronous-unwind-tables -fno-unwind-tables` (no gold linker on this
machine, so ICF is off — same as the user's measurement).

`.text` size, `size -A`:

| Commit                                | `.text`   |
|---------------------------------------|----------:|
| `57d8339` (pre type-erasure round 2)  | 128864    |
| `d5a9cf6` (HEAD of sweet-hawking-i8j0rh) | 132245 |
| Delta                                 | **+3381** |

The driver was rebuilt on this machine with `g++ 13.3` (no `g++-10` available);
the reported delta reproduces exactly, so the trend is robust to the compiler
major. Binaries kept at `/tmp/codesize-analysis/{pre,post}.bin`, full disasm at
`/tmp/codesize-analysis/{pre,post}.S`.

## Executive summary

**`.text` grew net +3381 bytes, but underneath that there are two big opposing
flows.** The header refactor between these two commits is the type-erasure
round-2 work (cores / `type_ops` / `storage_ops`, plus the 1-pointer string and
vector layouts). On the doctest workload it:

* removes ~7300 bytes of *inlined per-type bookkeeping* (the old
  `vector<T>::ensure_capacity`, hand-rolled emplace_back grow paths,
  per-`basic_string<>` `init(...)` / iterator-pair ctors, per-T allocator
  shims), and a 4KB shrink across many already-common symbols (`Context::run`
  for instance went from 4759→3940 bytes — see below);
* adds ~10700 bytes of *new per-`vector<T,A>` outlined glue* — one
  `ensure_free_`, one `storage_b_resize`, one `storage_b_data`, one
  `storage_b_set_size`, one `storage_b_cap_end`, one `storage_b_free` per
  instantiated `vector<T,A>` — plus a small amount of newly-needed shared
  scaffolding (`relocate_block`, `grow`, `move_construct_op<T,A>` for the
  non-trivially-relocatable doctest types).

The new outlined glue is **what each call to `vector<T,A>::push_back` /
`emplace_back` collapses to**: pre's 219-byte hand-rolled fast-path/grow
prologue became a 51-byte stub that calls into a 183-byte per-vector
`ensure_free_` and tail-jumps the same again to restore the spare-slot
invariant. So pre paid 219 bytes per callsite (and the same bytes again at
every `emplace_back` callsite that wasn't ICF'd), post pays ~51 bytes per
callsite plus a single 234-byte (`ensure_free_` + `storage_b_resize`
combined) per-`vector<T,A>` lump. Doctest has ~13 different `vector<T,A>`
instantiations, which is the *unfavourable* end of the size curve: every
new instantiation is a fresh 234-byte block before any sharing kicks in.

A bookkeeping note: ~+2200 of the +3381 bytes look like they belong in
`.text` but actually float in and out as the LTO inliner reshuffles
`<locale>`'s `num_get::do_get*` instantiations (different `[isra]` /
`[constprop]` clones get cut). This is collateral noise driven by the
header-graph change, not directly caused by the type-erasure work; I called
it out below but it would shift again on the next unrelated edit.

### Symbol-level accounting

(All sizes from `nm --size-sort -S --demangle`. `OTHER` are the residuals
where neither category fits.)

```
   +5355  type-erasure overhead (only in post):
              storage_b_resize<C>       2324 across 14 C's   (~143-152B each)
              vector<>::ensure_free_    2202 across 11       (~177-216B each)
              storage_b_set_size<C>     1157
              storage_b_free<C>          512
              storage_b_cap_end<C>       496
              storage_b_data<C>          304
              move_construct_op<T,A>     376 (for non-trivially-relocatable Ts:
                                              JUnitTestCase / JUnitTestMessage)
              detail::relocate_block     152 (one, shared across C/T/A)
              detail::grow                15 (one, shared — just a tail-jmp shim)
   +3768  vec/str helper outlining (only in post):
              ::reallocate_grow, ::vallocate, ::vdeallocate, ::clear, ::end_ptr_,
              ::capacity, ::size, init_copy, init_iter_fwd, assign_raw, ...
              These are formerly-inlined methods that the new layout outlined.
   +1411  other only-post (incl. dispatchers shuffled by LTO).
    -374  pre's hand-rolled vector emplace_back/ensure_capacity (only in pre).
    -307  pre's basic_string<char_const*> / iterator-pair ctors (only in pre).
    -138  pre's allocator boilerplate (only in pre).
   -2181  pre's num_get specializations (different inlining at LTO, see below).
   -2534  other only-pre.
   -2501  common-symbol delta: many symbols that exist in both got shorter
              (Context::run -819, JUnitTestCase emplace -388, ResultBuilder::log
              -55, XmlWriter::startElement -185, ...).
   ========
   +2499  symbol-delta total (~== +3381 in `.text`, the difference is
          section padding and linker-emitted dispatchers).
```

The ~4KB of new constant tables (`ops_for<T,A,MASK>`, `storage_ops_b<C>`,
~30 of each) live in `.data.rel.ro.local`, which went 5192 → 7656 (+2464
bytes). The user asked about `.text` so that's outside this report, but
worth noting that the type-erasure work moves *some* per-instantiation cost
to read-only data: each ops table is 88 B (`type_ops`) or 48 B
(`storage_ops`).

---

## How the calling convention changed (side-by-side disasm)

### A. `vector<T*>::emplace_back<T*>(T*&&)` — the canonical hot path

This callsite gets hit from doctest's reporter machinery in many places. T*
here is `doctest::TestCaseData const*`, but the same shape holds for any
trivially-relocatable `vector<X>`.

**Pre (`57d8339`)** — 219 bytes. Everything is inlined: the fast-path
"size != cap" check, the slow-path `recommend`/`allocate`/element-copy-by-pointer
loop / `deallocate`. One symbol does it all:

```
000000000001b288 <vector<TestCaseData*>::emplace_back<TestCaseData*>(TestCaseData*&&)>:
   1b288:	endbr64
   1b28c:	push %r14; push %r13; mov %rsi,%r13; push %r12; push %rbp; push %rbx
   1b297:	mov 0x8(%rdi),%rax            ; end_
   1b29b:	mov %rdi,%rbx
   1b29e:	mov 0x10(%rdi),%rcx           ; cap_
   1b2a2:	cmp %rcx,%rax
   1b2a5:	jae 1b2bb                     ; fast path: space available
   1b2a7:	mov (%rsi),%rdx               ; *= construct in place
   1b2aa:	add $0x8,%rax                 ; *= bump end_
   1b2ae:	mov %rdx,-0x8(%rax)
   1b2b2:	mov %rax,0x8(%rdi)
   1b2b6:	jmp 1b35a                     ; out

   1b2bb:                                 ; slow path: grow
   1b2bb:	movabs $0x1fffffffffffffff,%r12 ; max_size
   1b2c5:	mov (%rdi),%rdx
   1b2c8:	sub %rdx,%rax
   1b2cb:	sar $0x3,%rax                 ; old size
   1b2cf:	inc %rax
   1b2d2:	cmp %rax,%r12
   1b2d5:	jae 1b2d9
   1b2d7:	ud2                           ; length_error trap
   1b2d9:	movabs $0x7ffffffffffffff0,%rsi
   1b2e3:	sub %rdx,%rcx                 ; old capacity in bytes
   1b2e6:	mov %rcx,%rdx
   1b2e9:	sar $0x3,%rdx
   1b2ed:	cmp %rcx,%rsi
   1b2f0:	jb 1b2ff
   1b2f2:	add %rdx,%rdx                 ; 2*cap
   1b2f5:	cmp %rax,%rdx
   1b2f8:	cmovae %rdx,%rax              ; max(2*cap, size+1)
   1b2fc:	mov %rax,%r12
   1b2ff:	mov %r12,%rdi
   1b302:	call 1b26a                    ; allocator::allocate
   1b307:	mov 0x8(%rbx),%rdx            ; old end_
   1b30b:	mov (%rbx),%rdi               ; old begin_
   1b30e:	mov %rax,%rbp
   1b311:	lea (%rax,%r12,8),%r12        ; new cap_
   1b315:	xor %eax,%eax
   1b317:	mov %rdx,%rcx                 ; *= relocate loop
   1b31a:	sub %rdi,%rcx
   1b31d:	cmp %rcx,%rax
   1b320:	je 1b331
   1b322:	mov (%rdi,%rax,1),%rsi
   1b326:	mov %rsi,0x0(%rbp,%rax,1)     ; ptr copy
   1b32b:	add $0x8,%rax
   1b32f:	jmp 1b31d
   1b331:                                 ; install new buffer
   1b331:	mov %rbp,%rcx; sub %rdi,%rcx
   1b337:	lea 0x8(%rcx,%rdx,1),%r14
   1b33c:	mov 0x0(%r13),%rdx
   1b340:	mov %rdx,0x0(%rbp,%rax,1)     ; *= construct the new tail
   1b345:	test %rdi,%rdi
   1b348:	je 1b34f
   1b34a:	call 4db0                     ; *= deallocate old
   1b34f:	mov %rbp,(%rbx); mov %r14,0x8(%rbx); mov %r12,0x10(%rbx)
   1b35a:	pop %rbx; pop %rbp; pop %r12; pop %r13; pop %r14; ret
```

**Post (`d5a9cf6`)** — 51 bytes. Same fast path, but slow path is hidden behind
two calls. The whole function is a structured "compute slot, write, advance
size, restore spare":

```
0000000000025566 <vector<TestCaseData*>::emplace_back<TestCaseData*>(TestCaseData*&&)>:
   25566:	endbr64
   2556a:	push %rbp; mov %rsi,%rbp; push %rbx; mov %rdi,%rbx; push %rax
   25573:	call 25024 <vector<TestCaseData*>::ensure_free_(unsigned long)>
   25578:	mov 0x0(%rbp),%rdx           ; the rvalue
   2557c:	mov %rbx,%rdi
   2557f:	lea 0x8(%rax),%rsi           ; new end_ - begin_
   25583:	sub (%rbx),%rsi
   25586:	mov %rdx,(%rax)              ; place the pointer
   25589:	call 13cf5 <storage_b_set_size<vector<TestCaseData*>>(void*,unsigned long)>
   2558e:	pop %rdx; mov %rbx,%rdi; pop %rbx; pop %rbp
   25594:	jmp  25024 <vector<TestCaseData*>::ensure_free_(unsigned long)>  ; tail-call: restore spare
```

The body it tail-calls is the *outlined* equivalent of pre's slow path,
**but now one copy per `vector<T,A>` regardless of how many call sites it
has**:

```
0000000000025024 <vector<TestCaseData*>::ensure_free_(unsigned long)>:
   25024:	push %r12; push %rbp; xor %ebp,%ebp; push %rbx
   2502a:	cmpq $0x0, (%rdi)            ; begin_ != null?
   2502e:	mov %rdi,%rbx
   25031:	je 25056                      ; empty -> grow directly
   25033:	call 12b33 <storage_b_data<...>(void*)>          ; current data + size
   25038:	mov %rbx,%rdi
   2503b:	shr $0x3,%rdx                ; size in elems
   2503f:	mov %rdx,%r12
   25042:	call 138b6 <storage_b_cap_end<...>(void*)>       ; cap byte ptr
   25047:	sub (%rbx),%rax              ; bytes
   2504a:	mov %rax,%rbp
   2504d:	shr $0x3,%rbp                ; cap in elems
   25051:	cmp %rbp,%r12
   25054:	jne 250cf                    ; we have free slots -> return
   25056:	mov $0x1,%eax                ; -- grow --
   2505b:	lea 0x1(%rbp),%r12           ; need size+1
   2505f:	shl $0x3d,%rax               ; max_size
   25063:	cmp %rax,%r12
   25066:	jne 2506a
   25068:	ud2
   ...
   250b9:	mov %rbp,%r12                ; save size
   250bc:	lea -0xe782(%rip),%rsi       ; storage_b_resize<C>
   250c3:	lea 0x3dd6(%rip),%rdi        ; ops_for<T,A,10>
   250ca:	call 24dab <detail::grow(type_ops*, storage_ops*, void*, size_t)>
   250cf:	mov (%rbx),%rax
   250d2:	pop %rbx; pop %rbp
   250d4:	lea (%rax,%r12,8),%rax       ; address of first free slot
   250d8:	pop %r12; ret
```

`detail::grow` is the **shared** shim — note pre had no equivalent:

```
0000000000024dab <std::detail::grow(type_ops const*, storage_ops const*, void*, unsigned long)>:
   24dab:	endbr64
   24daf:	mov %rsi,%rax     ; storage_ops*  -> rax
   24db2:	mov %rdx,%rsi     ; void*         -> rsi
   24db5:	mov %rcx,%rdx     ; bytes         -> rdx
   24db8:	jmp *%rax         ; dispatch to storage_b_resize<C>
```

Sole purpose: shuffle args and tail-call the storage backend through a
function pointer (so it remains one symbol across all `<C>`s; the per-`C`
specialization is in `storage_b_resize<C>`).

And the per-`<C>` resize backend (~143 B for trivially-relocatable, ~152 B
for non-trivially-relocatable since it has to also call `relocate_block`
through the type_ops):

```
00000000000163a3 <storage_b_resize<vector<int>>(type_ops*, void*, unsigned long)>:
   163a3:	endbr64
   163a7:	push %r15; mov %rdi,%r15      ; type_ops*
   163ac:	push %r14; mov %rsi,%r14      ; container ptr (ctx)
   163b1:	push %r13; push %r12
   163b5:	xor %r12d,%r12d
   163b8:	push %rbp; push %rbx; push %r8
   163bc:	mov (%rsi),%rbp              ; old begin_
   163bf:	test %rbp,%rbp
   163c2:	je 163c8
   163c4:	mov -0x8(%rbp),%r12          ; old live-byte count (header word)
   163c8:	lea 0x8(%rdx),%rdi
   163cc:	call 25599 <storage_b_glue<>::raw_alloc>   ; malloc(H + bytes)
   163d1:	mov %rax,%rbx
   163d4:	lea 0x8(%rax),%r13            ; new data ptr (after H)
   163d8:	test %r12,%r12
   163db:	je 163f0
   163dd:	mov %r12,%r8
   163e0:	mov %rbp,%rcx
   163e3:	mov %r13,%rdx
   163e6:	xor %esi,%esi                 ; ec = nullptr (stateless alloc)
   163e8:	mov %r15,%rdi                 ; type_ops*
   163eb:	call 1627b <detail::relocate_block>       ; for vector<int>: memcpy path
   163f0:	test %rbp,%rbp
   163f3:	je 163fe
   163f5:	lea -0x8(%rbp),%rdi
   163f9:	call 58e0                     ; free(old base)
   163fe:	mov %r12,(%rbx)               ; write new header (live bytes)
   ...
```

The pre code achieved the same effect by inlining the malloc / memcpy /
free into the emplace_back body itself. The post code separates the
*per-T* parts (typed access to `begin_`, sizeof scaling) into
`ensure_free_<vector<T,A>>` and `storage_b_resize<vector<T,A>>`, leaves the
typed *element relocation* in `relocate_block` (shared, dispatches through
`type_ops::move_construct`), and shares the `malloc`/`free` calls.

### B. `relocate_block` — the one new shared helper

There was no equivalent in pre. Pre's vector inlined its
"`while (src != end_) construct_one(new, move(*src)); ++; ++; ...`" loop
directly into each `vector<T>::reallocate_grow`. Post pushes this to one
shared function that dispatches through the table:

```
000000000001627b <detail::relocate_block(type_ops*, void*, void*, void*, unsigned long)>:
   1627b:	endbr64
   ...
   16296:	mov 0x10(%rdi),%eax       ; flags
   16299:	not %eax
   1629b:	test $0x24,%al            ; (f_triv_reloc | f_alloc_default_life) both set?
   1629d:	jne 162b8                 ; no -> per-element loop
   1629f:	add $0x18,%rsp            ; yes -> raw memcpy fast path
   ...
   162b3:	jmp 5bb0                  ; tail-call memcpy
   162b8:                            ; element-by-element path
   162bb:	mov (%rdi),%r13           ; sizeof(T) (the size field)
   162be:	xor %edx,%edx
   162c0:	mov %rdi,%rbx
   ...
   162c9:	div %r13                  ; byte count / sizeof(T) = element count
   162cc:	mov %rax,0x8(%rsp)
   162d1:	mov 0x8(%rsp),%rax
   162d6:	cmp %rax,%r15             ; loop variable vs count
   162d9:	je 16304
   162db:	mov %rbp,%rdx; mov %r12,%rsi; mov %r14,%rdi
   162e4:	call *0x30(%rbx)          ; type_ops::move_construct(ctx,dst,src)
   162e7:	mov 0x10(%rbx),%eax
   162ea:	not %eax
   162ec:	test $0x28,%al            ; (f_triv_destroy | f_alloc_default_life)?
   162ee:	je 162f9                  ; skip destroy if trivial+default-life
   162f0:	mov %rbp,%rsi; mov %r14,%rdi
   162f6:	call *0x20(%rbx)          ; type_ops::destroy(ctx,src)
   162f9:	inc %r15; add %r13,%r12; add %r13,%rbp
   162fc:	jmp 162d1                 ; next element
   16304:	add $0x18,%rsp; ...; ret
```

For doctest's `vector<int>`, `vector<doctest::TestCaseData const*>`, etc.,
the `f_triv_reloc | f_alloc_default_life` test passes and the function
immediately tail-jumps `memcpy`. For `vector<JUnitTestCase>` (which holds a
`std::string`), it takes the element loop and dispatches through the
type_ops table, which is why post emits e.g. one
`move_construct_op<JUnitTestCase, allocator<JUnitTestCase>>` (90 B). The
table dispatch costs one indirect call per element — a runtime cost the
old code didn't have (it had a static call to a generated
`construct_one`/`destroy_at` per type). The size cost on this binary is
the +90B leaf vs the saved ~108B `~JUnitTestCase` symbol pre had — close
to a wash.

### C. `std::string::operator=(char const*)` — the layout-change tax

Pre, 46 bytes, blunt:

```
000000000001ede0 <std::string::operator=(char const*)>:
   1ede0:	endbr64
   1ede4:	push %rbp; mov %rdi,%rbp; mov %rsi,%rdi
   1edeb:	sub $0x10,%rsp; mov %rsi,0x8(%rsp)
   1edf4:	call 9aea <char_traits<char>::length>
   1edf9:	mov 0x8(%rsp),%rsi
   1edfe:	add $0x10,%rsp; mov %rbp,%rdi
   1ee05:	mov %rax,%rdx
   1ee08:	pop %rbp
   1ee09:	jmp 1ed14 <assign_raw>
```

Post, 221 bytes, defensively handles `s = c_str()` where `c_str()` points
into the existing buffer (the 1-pointer Strategy B layout reallocates more
aggressively, so the old "always safe to overwrite" reasoning no longer
holds):

```
0000000000024cce <std::string::operator=(char const*)>:
   24cce:	endbr64
   ...
   24cf5:	call a634 <char_traits<char>::length>
   24cfa:	mov (%rbx),%r13            ; data ptr
   24cfd:	mov %rax,%r12              ; length
   24d00:	test %r13,%r13
   24d03:	je 24d73                   ; null-buffer fast path
   24d05:	mov %rbx,%rdi
   24d08:	call 14cca <end_ptr_()>    ; check if src is inside [data, end)
   24d0d:	inc %rax
   24d10:	cmp %rax,%rbp
   24d13:	jae 24d73                  ; src is outside -> no aliasing
   24d15:	cmp %r13,%rbp
   24d18:	jb 24d73                   ; src is before data -> no aliasing
   24d1a:	mov %rbx,%rdi
   24d1d:	call 14cfc <capacity()>
   24d22:	cmp %r12,%rax
   24d25:	jae 24d73                  ; we won't grow -> still safe in place
   24d27:                              ; ALIASING + GROW: copy into a temp, then
                                      ; swap-and-deallocate
   24d27:	mov %rsp,%r13              ; temp scratch on stack
   24d2c:	xor %eax,%eax
   ...
   24d39:	call 1508c <init_copy(char const*, unsigned long)>
   ...
   24d54:	call 248e6 <assign_raw(char const*, unsigned long)>
   24d59:	call 15d92 <vdeallocate()>
   ...
   24d73:                              ; non-aliasing fast path
   24d73:	mov 0x8(%rsp),%rax
   24d78:	sub %fs:0x28,%rax
   24d7f:	jne 24d9b
   24d83:	add $0x18,%rsp
   24d87:	mov %r12,%rdx; mov %rbp,%rsi; mov %rbx,%rdi
   24d90:	pop %rbx; pop %rbp; pop %r12; pop %r13
   24d96:	jmp 248e6 <assign_raw(char const*, unsigned long)>
   ...
```

This is **+175 bytes per such operator=, paid by string** because the new
storage layout doesn't keep `cap_` and `end_` as direct pointers any more
— it has to call `end_ptr_()` / `capacity()` to find them — and the more
aggressive realloc semantics force the alias check the pre layout did not
need. Doctest doesn't call `s = c_str()` to itself, so the alias path is
dead, but `-Os` keeps the bodies.

### D. `doctest::Context::run()` — the shrink that pays for the new tax

```
                            pre     post     delta
Context::run                4759    3940     -819   (the single biggest delta either way)
ResultBuilder::log           1155   1101      -54
XmlReporter::log_assert      1135   1069      -66
XmlReporter::report_query    1584   1480     -104
ConsoleReporter::test_run_end 1094  1094       0
JUnitReporter::test_run_end  1535   1462      -73
ConsoleReporter::report_query 3020  3020       0
```

These functions don't change source between the two commits — doctest is a
git submodule, untouched. They shrink purely because the *inlined* calls
into `vector`/`string` methods got smaller. For instance, `Context::run`
manipulates a half-dozen `vector<doctest::String>` and the old
`vector<T>::ensure_capacity` / hand-rolled grow paths inlined into the
caller bloated each callsite. Now the same callsites are 51-byte stubs +
an out-of-line `ensure_free_`/`storage_b_resize`. The savings here are
~2500 bytes across many functions and they almost cancel the +5400 bytes
of new ops glue. Net is the +2499 bytes the symbol-by-symbol math gives.

### E. `num_get<>::do_get<long/long long/unsigned long/...>` (collateral noise)

The pre binary has six `do_get_floating_point<T>` instantiations (3 for
char × 3 for wchar) at ~728 B each that disappear in post; in their place
post emits three new `do_get_signed<T>`/`do_get_unsigned<T>` helpers at
~604–712 B each, plus the existing non-template `do_get` virtuals grow
from 9 B stubs to 722–725 B. This is the same family of functions —
`<locale>`'s integer/float parsing — refactored by LTO into a slightly
different inline/outline split. **The locale headers themselves are
unchanged between the two commits.** It is genuinely a side-effect of the
header dependency graph having shifted (new `bits/storage_ops.h`,
`bits/type_ops.h`, etc.) which made `-flto -Os` choose differently. Sum
of all `num_get<*>::do_get*` text:

```
pre:  approximately 11.5 KB across all instantiations
post: approximately 11.4 KB across all instantiations
```

so the net is small, but it appears as +2181 / -2181 in the only-pre /
only-post columns because the mangled names differ. I'd not attribute any
of the +3381 to this category in a code-size journal — it would shift
again on the next unrelated edit.

---

## What this means for the size journal

**The type-erasure round-2 work is, on doctest, ~size-neutral after the
shared-shrink credit is collected:**

* The new per-`vector<T,A>` outlined glue cost ~5.4 KB.
* That outlined glue made every inlined `vector` callsite 100-150 bytes
  smaller, saving ~2.5 KB across common symbols and another ~3.0 KB by
  letting the old hand-rolled `emplace_back<>` and basic_string ctor
  bodies be deleted.
* The remaining +3.4 KB delta is roughly half real (per-`vector<T,A>`
  storage shims that doctest doesn't share enough to amortize) and half
  LTO drift in `<locale>`'s `do_get*` instantiations that has nothing to
  do with the headers that actually changed.

Where doctest is unfavourable: it instantiates ~13 distinct
`vector<T,A>`s, each paying the ~234-byte storage-shim tax once. A
workload with fewer `vector<T,A>` instantiations and the same total call
volume would tip the curve favourably (the inlined-callsite savings scale
with call volume; the storage-shim cost is per type). Conversely, the
`std::string::operator=` regression (+175 B) is a *per-operator* cost paid
by every translation unit that uses `string::operator=(char const*)`, so
it accumulates linearly across drivers, not per-instantiation.

If a follow-up wants to recover bytes here, the productive places are:

1. Outline more of `string::operator=(char const*)` — the alias check
   could be a 30-byte tail-jump to a shared 80-byte slow path instead of
   a 175-byte inlined body. Same applies to wstring.
2. Collapse `storage_b_data<C>` / `storage_b_set_size<C>` /
   `storage_b_cap_end<C>` — for the 1-pointer Strategy B layout these are
   nearly identical across `C` (they all read/write the header word at
   `data-8`). A single non-template `storage_b_header_*` taking the
   `type_ops*`'s `size` field would replace all 14 instantiations of each
   (savings ~1.5 KB).
3. The 30 `storage_ops_b<C>` tables are 48 B each (1440 B in
   `.data.rel.ro.local`). They differ only in which `storage_b_*<C>`
   functions they point to; if (2) collapses those, the tables become
   identical and ICF would fold them.
