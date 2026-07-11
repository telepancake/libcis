# lean vs base: measured size (-Os, marginal over empty main)

Compiler: `g++`. text+data+bss from `size(1)`, minus the same include-order's empty-`main` baseline. Every binary is linked with `-ffunction-sections -fdata-sections -Wl,--gc-sections`, so a program carries only the kernels it reaches. `lean` links the kernels TU statically (per-binary cost, single-image targets); `lean-so` links against `liblean.so` (per-binary cost when the kernels ship once per system). A FAIL row means that program did not build/run — a missing number is a defect, not a zero.

| program | base | lean (static) | delta | lean-so (shared kernels) | delta |
|---|---|---|---|---|---|
| string | 3785 (3657+112+16) | 2903 (2791+112+0) | -23% | 977 (961+8+8) | -74% |
| vector_int | 1949 (1853+80+16) | 2804 (2708+96+0) | +44% | 1466 (1426+32+8) | -25% |
| vector_string | 3189 (3085+88+16) | 3811 (3675+136+0) | +20% | 1777 (1681+88+8) | -44% |
| sort_int | 2087 (1999+72+16) | 3832 (3752+80+0) | +84% | 904 (880+16+8) | -57% |
| sort_string | 6920 (6808+96+16) | 6792 (6648+144+0) | -2% | 1783 (1687+88+8) | -74% |
| map_int | 2560 (2464+80+16) | 2692 (2620+72+0) | +5% | 1242 (1202+32+8) | -51% |
| map_string | 4061 (3949+96+16) | 4702 (4598+104+0) | +16% | 1846 (1806+32+8) | -55% |
| unordered_int | 5078 (4958+104+16) | 2225 (2121+104+0) | -56% | 1690 (1626+56+8) | -67% |
| list_int | 1811 (1715+80+16) | 1358 (1286+72+0) | -25% | 719 (695+16+8) | -60% |
| deque_int | 2695 (2599+80+16) | 3022 (2926+96+0) | +12% | 1761 (1729+24+8) | -35% |

## liblean kernels object (one copy per SYSTEM)

The out-of-line structural/algorithm kernels compile to a single translation unit, shipped once as `liblean.a` / `liblean.so`. With `--gc-sections` each program above pulls in only the slice it uses; this is the whole-TU `size(1)` **text** cost when the library is linked in full:
- `lean/src/kernels.cpp` .o text: `70119 bytes`

## sizeof (bytes)

- base: `string=24 vector=24 map=32 set=32 unordered_map=40 list=32 deque=56`
- lean: `string=8 vector=8 map=40 set=32 unordered_map=8 list=24 deque=24`
