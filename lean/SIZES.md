# lean vs base: measured size (-Os, marginal over empty main)

Compiler: `g++`. text+data+bss from `size(1)`, minus the same include-order's empty-`main` baseline. A FAIL row means that program did not build/run — a missing number is a defect, not a zero.

| program | base text+data+bss | lean text+data+bss | delta |
|---|---|---|---|
| string | 2595 (2579+8+8) | 1601 (1585+8+8) | -38% |
| vector_int | 1914 (1906+0+8) | 1982 (1958+0+24) | +4% |
| vector_string | 2612 (2604+0+8) | 1870 (1862+0+8) | -28% |
| sort_int | 2200 (2192+0+8) | 3326 (3302+0+24) | +51% |
| sort_string | 6280 (6272+0+8) | 4786 (4778+0+8) | -24% |
| map_int | 2524 (2516+0+8) | 3006 (2982+0+24) | +19% |
| map_string | 3420 (3412+0+8) | 3286 (3278+0+8) | -4% |
| unordered_int | 5056 (5024+24+8) | 2178 (2130+24+24) | -57% |
| list_int | 1774 (1766+0+8) | 1656 (1632+0+24) | -7% |
| deque_int | 2658 (2650+0+8) | 2442 (2418+0+24) | -8% |

## sizeof (bytes)

- base: `string=24 vector=24 map=32 set=32 unordered_map=40 list=32 deque=56`
- lean: `string=8 vector=8 map=40 set=32 unordered_map=8 list=24 deque=24`
