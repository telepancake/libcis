# Callsite analysis methodology

Honest record of what the right measurement methodology is for "how does
this change affect code size on the architectures we care about?" — and
the recipe for actually doing it. Written after several rounds of me
producing decision-grade-looking numbers that turned out to be wrong-arch,
wrong-scope, or undecomposed.

## Decision-grade code-size analysis requires THREE measurements

Project `.text` is a single scalar. By itself it tells you the bottom
line but doesn't tell you which knob to turn. To act on it you need to
decompose:

```
project .text = SHARED CORES (paid once per binary)
              + PER-(C,A) INSTANTIATION (paid per distinct vector<T,A>)
              + PER-CALLSITE arg-setup × callsite count
```

Each of these scales differently and is fixed by different design
choices. Conflating them — which I have been doing — produces "+4 KB
regression!" reports that lead to wrong fixes.

### 1. SHARED CORES: `nm --size-sort src/support.cpp's .o`

These are the type-erased free functions in `support.cpp` —
`destroy_range`, `rotate`, `grow`, `relocate_block`, etc. ONE copy of
each lives in the final binary regardless of how many container shapes
the program uses. Cost is amortized over the whole program.

```sh
g++ $FLAGS -Iinclude -c src/support.cpp -o /tmp/support.o
nm --size-sort --print-size /tmp/support.o | c++filt \
    | awk '$3=="T"' | sort -k2 -nr | head
```

Use the same `$FLAGS` the bench uses (see `bench/codesize.py:_opt_flags`)
— `-std=gnu++20 -Os -fno-exceptions -fno-rtti -flto` etc.

### 2. PER-(C, A) INSTANTIATION: `nm` on a TU with one container shape

Each `(Container, T, A)` triple generates per-instantiation symbols: the
`storage_*_data<C>`, `storage_*_set_size<C>`, `storage_*_resize<C>`,
`storage_*_free<C>`, the `storage_ops_a<C>` / `storage_ops_b<C>`
constant tables, and any weak-symbol container methods that get
out-of-lined (`size()`, `capacity()`, `end_ptr_()`, `pop_back()`...).

```sh
# Build a TU that uses just one container shape.
cat > /tmp/one_inst.cpp <<'EOF'
#include <vector>
[[gnu::noinline]] void f(std::vector<int>& v, int x) { v.push_back(x); }
EOF
g++ $FLAGS -Iinclude -c /tmp/one_inst.cpp -o /tmp/one_inst.o
nm --size-sort --print-size /tmp/one_inst.o | c++filt
```

The per-instantiation cost is the sum of those symbols MINUS the
function `f` (which is per-callsite). Compare across two states of
the headers to see the per-instantiation delta.

### 3. PER-CALLSITE: `objdump -d` of a real callsite, NOT a probe TU

This is the trap I kept falling into. "Per-callsite" doesn't mean
"size of a one-call wrapper function" — that's inflated by function
prologue/epilogue, register-save decisions for *that wrapper*, etc.

Real per-callsite cost is the bytes the compiler emits at the
**actual call site** inside a larger function — from where args
start being set up to immediately after the `call` instruction. It
varies because surrounding code already has some args in registers,
so the same callee call can be 8 bytes here, 25 bytes there.

The right way to measure: take a representative actual TU (one of the
`bench/projects/*` consumers, or a manually written multi-op function),
disassemble the function containing the call you care about, and count
bytes from the first arg-setup instruction to the byte after `call`.

```sh
g++ $FLAGS -Iinclude -c real_consumer.cpp -o /tmp/real.o
objdump -dr -M intel --no-show-raw-insn /tmp/real.o \
    | awk '/<.*MY_FUNCTION.*>:/,/^$/'
```

Compare BEFORE and AFTER versions of the library on the SAME consumer
file. The per-callsite delta is meaningful; the absolute number is
context-dependent.

### Why the probe-TU approach was misleading

A probe TU full of one-call wrappers measures `function size`, which is
`prologue + arg-shuffle + call + epilogue`. The prologue/epilogue varies
when the compiler chooses different callee-save registers based on the
core's signature. That can swing the wrapper size by 10+ bytes even
when the actual callsite cost in real code is unchanged.

So probe-TU numbers are useful **only as deltas between two states of
the same probe** — and even then only roughly, because compiler
register-allocation heuristics may pick different strategies on the
two sides for reasons unrelated to the change being studied.

## Architectures we actually care about

In priority order:

1. **i386 SysV** (`g++ -m32 -Os`) — primary target for size-constrained
   32-bit Linux embedded systems. 0 register args; all args spill to
   stack via `push`. Arg-order changes don't help here (push order is
   compiler's choice anyway).

2. **ARM-32 AAPCS** (`arm-linux-gnueabihf-g++ -mthumb -Os`) — the other
   primary 32-bit target. 4 register args (r0-r3), then stack. Arg-order
   changes DO matter here.

3. **x86_64 SysV** (`g++ -Os`) — desktop/server reference; 6 register
   args (rdi, rsi, rdx, rcx, r8, r9). Arg-order may help here for cores
   with 7+ args.

4. **AArch64 AAPCS** (`aarch64-linux-gnu-g++ -Os`) — 8 register args
   (x0-x7); virtually no spill for any of our cores. Codesize basically
   doesn't move on this arch.

When reporting a code-size change, ALWAYS measure at least i386 and
x86_64. ARM-32 if a cross-compiler is installed. Do NOT report only
x86_64 numbers and pretend they represent the size-constrained 32-bit
target the library actually exists for.

### Cross-compiler setup

```sh
# Ubuntu 24.04
apt-get -y install gcc-multilib g++-multilib   # for -m32 (i386)
apt-get -y install g++-arm-linux-gnueabihf     # for arm-linux-gnueabihf-g++
apt-get -y install g++-aarch64-linux-gnu       # for aarch64-linux-gnu-g++
```

## Verifying what's actually called (counter-example: I once cited
`construct_default_n` as a +bytes culprit; it has zero callers)

```sh
# All call sites of detail::X in headers + support:
grep -rn 'detail::X\|::X\b' include/ src/ \
    | grep -v 'bits/.*\.h:\|src/support\.cpp:.*::X(.*)' \
    | head
```

If grep returns nothing outside the declaration/definition sites, the
function is **dead code**. Delete it before reporting any analysis
about "where the bytes come from."

Transitive callers: helpers like `vector::vdeallocate`,
`vector::destruct_at_end`, `vector::construct_at_end` aggregate
behavior across many public methods. To trace, build the map:

```
core             →  helper            →  public methods
─────────────────   ──────────────────   ────────────────────────────────
destroy_range    →  vdeallocate       →  ~vector, operator=, assign,
                                          shrink_to_fit(empty case)
                 →  destruct_at_end   →  clear, resize, assign (shrink),
                                          assign_with_size (shrink),
                                          erase(via rotate-delta<0)
                 →  rotate(delta<0)   →  (called from inside support.cpp)
```

Building this map by hand is error-prone (I've gotten it wrong twice).
The recipe is: grep for the helper, identify each enclosing method
function definition, list them. A subagent can do this reliably if
asked.

## When changes need full bench measurement

`bench/codesize.py` measures actual project `.text` for the eight
projects in `bench/projects/`. Use it when:

- The change affects symbols that the simple probes can't see (e.g.
  storage_*_resize templates, which only instantiate when a real
  consumer pulls them in).
- The change involves per-instantiation infrastructure (extra slots in
  `storage_ops`, extra weak symbols per container shape).
- You want decision-grade numbers for "should we ship this."

Don't use it for incremental "is the per-callsite shape right" checks
— too slow, and the answer is buried in per-project sums.

```sh
CXX="g++"        python3 bench/record.py "what changed" projects...
CXX="g++ -m32"   python3 bench/record.py "what changed (32-bit)" projects...
python3 bench/record.py --diff <prev-commit>
```

Always record BOTH 64-bit and 32-bit numbers when reporting; the
library exists for the 32-bit target.

## Trial summary (what worked, what didn't)

Documented to save the next round of analysis from re-discovering these.

| change                                  | x86_64 callsite Δ | i386 callsite Δ | verdict |
|-----------------------------------------|-------------------|-----------------|---------|
| reorder rotate args (st_ctx-first)      | +13 erase, +13 insert | 0 everywhere | **REGRESSION** — compiler picked stack-save over rbx for the pass-through `this` because of `end_ptr_()` call between |
| ensure_free_ returns pointer            | -5 push, -12 insert | TBD | **WIN** |
| rotate(...,delta) folded into core      | +12 erase callsite saved vs separate set_size/pop_back call chain | same direction (smaller magnitude) | **WIN** — see real-consumer measurement below |
| alloc_ctx slot lookup vs threaded el_ctx | +20 destroy_range callsite (5 args vs 4) | TBD | **REGRESSION at callsite, abstraction OK** — should split: cores that already take sops gain the slot, others keep el_ctx as compile-time arg |

The pattern: simple-sounding "principles" (pass-through args first, lazy
fetch always wins) don't survive contact with the compiler's
heuristics. Measure both architectures, compare across real consumer
code, then decide.

## rotate(...,delta) verdict on three architectures

Built `bench/drivers/vec_mixed.cpp` (5 instantiations: int, long,
double, string, Big — exercises push_back, insert, erase, resize) and
`src/support.cpp` with and without the rotate-`delta` argument. With
delta, erase/insert/emplace each become one rotate call (delta spills
as `mov [rsp], imm32` = 8 B on x86_64); without delta they become
rotate + explicit `pop_back`/`destruct_at_end`/`set_size_elems_`
follow-up calls.

```
Arch    Object       with-delta    no-delta    Δ
x86_64  vec_mixed.o   10371        10698       +327   (per-callsite tax)
x86_64  support.o     81563        81410       -153   (rotate body shrank)
x86_64  NET           ---          ---         +174   per binary using vec_mixed

i386    vec_mixed.o   11446        11758       +312
i386    support.o     86124        85887       -237
i386    NET           ---          ---         +75

ARM-32  vec_mixed.o    8223         8395       +172
ARM-32  support.o     62174        62070       -104
ARM-32  NET           ---          ---         +68
```

All three architectures agree: with-delta is smaller per real-world
binary. Magnitude is largest on x86_64 (set_size-via-storage-slot is
multiple call instructions); smallest on ARM-32 (Thumb-2 makes small
follow-up calls compact). On every architecture, the `support.cpp`
saving from a simpler rotate body does NOT offset the per-callsite
cost of replacing one rotate call with rotate+follow-up.

**Decision:** keep rotate(...,delta) as designed. The 8-byte delta
spill on x86_64 (`mov [rsp], 0xfffffffffffffffc`) is the right
trade for collapsing the erase/insert/emplace post-amble into one core.

This was the first analysis in this branch that compared real-consumer
TUs on real target architectures (not synthetic probes on x86_64 only).
Numbers were directional, not single-arch noise.
