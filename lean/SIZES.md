# lean vs base: measured size (-Os, marginal over empty main)

Compiler: `g++`. text+data+bss from `size(1)`, minus the same include-order's empty-`main` baseline. Every binary is linked with `-ffunction-sections -fdata-sections -Wl,--gc-sections`, so a program carries only the kernels it reaches (the lean profile links the out-of-line kernels TU `lean/src/kernels.cpp`; the base profile has no such file). A FAIL row means that program did not build/run — a missing number is a defect, not a zero.

| program | base text+data+bss | lean text+data+bss | delta |
|---|---|---|---|
| string | 2008 (1992+16+0) | 1060 (1044+16+0) | -47% |
| vector_int | 1198 (1198+0+0) | 1846 (1846+0+0) | +54% |
| vector_string | 1668 (1668+0+0) | 2033 (1993+40+0) | +22% |
| sort_int | 1486 (1486+0+0) | 3204 (3204+0+0) | +116% |
| sort_string | 5520 (5512+8+0) | 4932 (4884+48+0) | -11% |
| map_int | 1810 (1810+0+0) | 2299 (2299+0+0) | +27% |
| map_string | 2541 (2533+8+0) | 2812 (2804+8+0) | +11% |
| unordered_int | 4372 (4348+24+0) | 1758 (1734+24+0) | -60% |
| list_int | 1060 (1060+0+0) | 965 (965+0+0) | -9% |
| deque_int | 1944 (1944+0+0) | 2116 (2116+0+0) | +9% |

## liblean kernels object (one copy per SYSTEM)

The out-of-line structural/algorithm kernels compile to a single translation unit, shipped once as `liblean.a` / `liblean.so`. With `--gc-sections` each program above pulls in only the slice it uses; this is the whole-TU `size(1)` **text** cost when the library is linked in full:
- `lean/src/kernels.cpp` .o text: `13020 bytes`

## sizeof (bytes)

- base: `string=24 vector=24 map=32 set=32 unordered_map=40 list=32 deque=56`
- lean: `string=8 vector=8 map=40 set=32 unordered_map=8 list=24 deque=24`
