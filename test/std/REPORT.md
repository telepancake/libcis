# libcis test-transfer: Phase-1 vendor measurement

AST-transferred libc++ test suite (`tools/transfer.py`) measured against the two reference vendor toolchains. **No libcis numbers here** (Phase 2). Every count below is a real compile/run of a transferred file -- nothing sampled or extrapolated.

- Transfer: `tools/transfer.py` (libclang AST rewrite)
- Measure : `tools/measure_std.py` -> `results.json`
- libcxx  : `clang++-20 -std=gnu++2c -stdlib=libc++`
- libstdcxx: `g++-14 -std=gnu++26`
- Both    : `-fno-exceptions -fno-rtti -I test/std/support`

## Totals (transfer stage)

- inputs found   : **1865**
- transferred    : **1652** (89% of inputs)
- skipped        : 91 (UNSUPPORTED intersects our active no-exceptions/no-rtti, or REQUIRES unmet)
- parse/xform err: 122

## Measured vendor totals (over measured subtrees)

Subtrees measured cover **327** transferred tests.

| vendor | compile-pass | run-pass |
|---|---|---|
| libcxx | 305/327 (93%) | 215/216 (100%) |
| libstdcxx | 279/327 (85%) | 211/213 (99%) |

## Per-subtree breakdown

`xfer` = transferred / inputs. compile & run are pass/total. Run total counts only run-kind tests; compile-only (`*.compile.pass.cpp`) tests have no run column entry.

| subtree | inputs | xfer | skip | err | cxx compile | cxx run | stdcxx compile | stdcxx run |
|---|--:|--:|--:|--:|--:|--:|--:|--:|
| concepts | 35 | 34 | 0 | 1 | 33/34 | 4/4 | 32/34 | 4/4 |
| diagnostics | 72 | 72 | 0 | 0 | 72/72 | 72/72 | 72/72 | 72/72 |
| language.support | 272 | 221 | 24 | 27 | 200/221 | 139/140 | 175/221 | 135/137 |

