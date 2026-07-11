# lean vs base: measured size (-Os, marginal over empty main)

Compiler: `g++`. text+data+bss from `size(1)`, minus the same include-order's empty-`main` baseline. Every binary is linked with `-ffunction-sections -fdata-sections -Wl,--gc-sections`, so a program carries only the kernels it reaches. `lean` links the kernels TU statically (per-binary cost, single-image targets); `lean-so` links against `liblean.so` (per-binary cost when the kernels ship once per system). A FAIL row means that program did not build/run — a missing number is a defect, not a zero.

| program | base | lean (static) | delta | lean-so (shared kernels) | delta |
|---|---|---|---|---|---|
| string | 2008 (1992+16+0) | 1052 (1036+16+0) | -48% | 969 (961+8+0) | -52% |
| vector_int | 1198 (1198+0+0) | 1846 (1846+0+0) | +54% | 1474 (1442+32+0) | +23% |
| vector_string | 1668 (1668+0+0) | 2025 (1985+40+0) | +21% | 1769 (1681+88+0) | +6% |
| sort_int | 1486 (1486+0+0) | 3286 (3286+0+0) | +121% | 896 (880+16+0) | -40% |
| sort_string | 5536 (5528+8+0) | 5006 (4958+48+0) | -10% | 1791 (1703+88+0) | -68% |
| map_int | 1810 (1810+0+0) | 2299 (2299+0+0) | +27% | 1250 (1218+32+0) | -31% |
| map_string | 2541 (2533+8+0) | 2932 (2924+8+0) | +15% | 1854 (1822+32+0) | -27% |
| unordered_int | 4372 (4348+24+0) | 1758 (1734+24+0) | -60% | 1698 (1642+56+0) | -61% |
| list_int | 1060 (1060+0+0) | 965 (965+0+0) | -9% | 727 (711+16+0) | -31% |
| deque_int | 1944 (1944+0+0) | 2100 (2100+0+0) | +8% | 1769 (1745+24+0) | -9% |

## liblean kernels object (one copy per SYSTEM)

The out-of-line structural/algorithm kernels compile to a single translation unit, shipped once as `liblean.a` / `liblean.so`. With `--gc-sections` each program above pulls in only the slice it uses; this is the whole-TU `size(1)` **text** cost when the library is linked in full:
- `lean/src/kernels.cpp` .o text: `80550 bytes`

## sizeof (bytes)

- base: `string=24 vector=24 map=32 set=32 unordered_map=40 list=32 deque=56`
- lean: `string=8 vector=8 map=40 set=32 unordered_map=8 list=24 deque=24`
