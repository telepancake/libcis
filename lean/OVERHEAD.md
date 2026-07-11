# Overhead decomposition + deterministic perf delta (base vs lean)

Compiler: `g++` at -Os, gc-sections, standard -nodefaultlibs recipe. Sizes are text+data+bss minus the empty-main baseline of the same include order. Model S = FIXED + PER_TYPE*T + PER_CALLSITE*T*C fitted by finite differences on grid [(1, 1), (9, 1), (9, 9)]; held-out point (5, 5) reports fit error. liblean.so (once per SYSTEM, lean-so rows only): 133484 bytes.

## Containers + sort: size decomposition (bytes)

Workload: T structurally-identical types x C noinline callsites, each doing vector fill/insert/erase + sort (distinct lambda per callsite) + map insert/iterate/erase.

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 1,217 | 2,114 | 1,484 | -3.3% |
| lean-static | 4,927 | 703 | 721 | -6.8% |
| lean-so | 1,291 | 723 | 721 | -7.5% |

- delta lean-static vs base: fixed 3,710, per type -1,410, per callsite -764
- delta lean-so vs base:     fixed 74, per type -1,390, per callsite -764

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 1,217 + 2,114*T_p + 1,484*T_p*C_p ]
lean-static: sum_p [ 4,927 + 703*T_p + 721*T_p*C_p ]
lean-so:     133484  +  sum_p [ 1,291 + 723*T_p + 721*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 4815 | 6351 | 2735 |
| (9, 1) | 33599 | 17746 | 14285 |
| (9, 9) | 140483 | 69656 | 66177 |
| (5, 5) | 50541 | 28389 | 24789 |

## std::function: size decomposition (bytes)

Workload: F receiver types x G noinline callsites, each constructing a one-word-capture lambda function, calling it in a loop, plus a plain-fn-pointer function and a copy — the dominant callable shapes.

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 851 | 101 | 914 | -0.2% |
| lean-static | 183 | 124 | 359 | -1.3% |
| lean-so | 175 | 131 | 359 | -1.7% |

- delta lean-static vs base: fixed -668, per type 23, per callsite -555
- delta lean-so vs base:     fixed -675, per type 30, per callsite -555

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 851 + 101*T_p + 914*T_p*C_p ]
lean-static: sum_p [ 183 + 124*T_p + 359*T_p*C_p ]
lean-so:     133484  +  sum_p [ 175 + 131*T_p + 359*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 1865 | 665 | 665 |
| (9, 1) | 9979 | 4524 | 4582 |
| (9, 9) | 75763 | 30356 | 30414 |
| (5, 5) | 24243 | 9896 | 9970 |

## std::variant: size decomposition (bytes)

Workload: T variant types (variant<long, Alt_k, double> with a distinct string-member Alt_k per type — non-trivial, so the table-driven engine is exercised) x C noinline callsites, each doing cross- and same-index assign, copy, move and a visit. The per-type slice is the special-member machinery lean erases; the per-callsite slice is the user visitor lambda (not a lean target — the visitor cannot be erased without losing its typed result).

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 639 | 853 | 735 | -4.3% |
| lean-static | 465 | 571 | 730 | -5.2% |
| lean-so | 711 | 584 | 729 | -5.1% |

- delta lean-static vs base: fixed -174, per type -282, per callsite -5.0
- delta lean-so vs base:     fixed 72, per type -269, per callsite -6.3

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 639 + 853*T_p + 735*T_p*C_p ]
lean-static: sum_p [ 465 + 571*T_p + 730*T_p*C_p ]
lean-so:     133484  +  sum_p [ 711 + 584*T_p + 729*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 2227 | 1766 | 2024 |
| (9, 1) | 14935 | 12178 | 12530 |
| (9, 9) | 67877 | 64760 | 65016 |
| (5, 5) | 24341 | 22764 | 23020 |

## std::format: size decomposition (bytes)

Workload: F user types each with its own std::formatter x G noinline callsites, each formatting a mix (int, hex int, float, string, the user type) via std::format + format_to_n + formatted_size. The FIXED slice is the runtime engine (the vformat_to dispatch loop + fp_to_chars' snprintf float path + the integer/grouping/padding/fill write helpers) that base emits once per binary. The lean overlay relocates it out-of-line (fmt::vformat_engine<CharT>, extern-template'd in <format>, defined + explicitly instantiated in kernels.cpp): STILL per-binary for lean-static, but ONCE PER SYSTEM (liblean.so) for lean-so. lean-so's tiny FIXED vs base's ~41 KB FIXED is the move this axis exists to show. The per-type slice is the user formatter; the per-callsite slice is make_format_args + the thin out-of-line engine call.

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 42,564 | 406 | 510 | -1.3% |
| lean-static | 41,925 | 408 | 508 | -1.3% |
| lean-so | 3,574 | 410 | 508 | -3.9% |

- delta lean-static vs base: fixed -639, per type 1.9, per callsite -1.9
- delta lean-so vs base:     fixed -38,990, per type 4.0, per callsite -2.0

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 42,564 + 406*T_p + 510*T_p*C_p ]
lean-static: sum_p [ 41,925 + 408*T_p + 508*T_p*C_p ]
lean-so:     133484  +  sum_p [ 3,574 + 410*T_p + 508*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 43480 | 42841 | 4492 |
| (9, 1) | 50808 | 50169 | 11836 |
| (9, 9) | 87512 | 86737 | 48396 |
| (5, 5) | 58092 | 57381 | 19056 |

## iostreams (cout/cin): size decomposition (bytes)

Workload: T user types (each with its own operator<< / operator>>) x G noinline callsites, each formatting AND extracting a mix (int, hex int, float, bool, string, the user type) through std::cout / std::cin + a round-trip stringstream (cout redirected to an in-memory stringbuf). The FIXED slice is the stream machinery + the DIRECT number-formatting engine (format_integer_impl, put_float, pad_and_output, base_10/base_16 LUTs) + the cout/cin static init — NOT locale-dependent (base already formats numbers directly, no num_put/num_get facet). The lean overlay's LOCALE SEVERANCE (classic-only streams) removes the residual <locale> coupling from the stream code path — no use_facet<ctype>, no numpunct, no stored std::locale — shaving the locale ctor/copy/assign and the use_facet<ctype<char>> widen/narrow instantiations from FIXED. The ~65 KB classic facet suite is NOT in this marginal: it is pinned into EVERY binary (incl. the empty-main baseline subtracted here) by two eager initializers outside the stream headers — stream_facet_init_instance (src/support.cpp) and locale_classic_init_instance (include/locale) — so it cancels. See lean/README.md 'Locale severance' for the quantified ceiling (~83 KB recoverable once those eager inits go lazy).

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 31,583 | 187 | 891 | -0.5% |
| lean-static | 29,968 | 182 | 885 | -0.4% |
| lean-so | 29,922 | 178 | 885 | -0.5% |

- delta lean-static vs base: fixed -1,615, per type -5.2, per callsite -6.1
- delta lean-so vs base:     fixed -1,661, per type -9.2, per callsite -6.1

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 31,583 + 187*T_p + 891*T_p*C_p ]
lean-static: sum_p [ 29,968 + 182*T_p + 885*T_p*C_p ]
lean-so:     133484  +  sum_p [ 29,922 + 178*T_p + 885*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 32661 | 31035 | 30985 |
| (9, 1) | 41287 | 39571 | 39489 |
| (9, 9) | 105443 | 103291 | 103209 |
| (5, 5) | 55055 | 53239 | 53189 |

## Containers + sort: deterministic performance (callgrind, cache+branch sim)

Container workload, T=4 C=2 n=1500; each order's empty-main startup counts (crt + ld.so, which the lean-so order pays extra for shared-object resolution) are measured separately and subtracted — what remains is the workload itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle estimate). Counts are synthetic and repeatable — immune to host clock jitter.

| event | base | lean-static | lean-so | lean-static vs base |
|---|---|---|---|---|
| Ir | 9,964,674 | 17,616,873 | 17,946,905 | +76.8% |
| Dr | 2,509,951 | 4,130,192 | 4,294,146 | +64.6% |
| Dw | 1,480,633 | 2,643,247 | 2,644,232 | +78.5% |
| L1m | 78,091 | 65,810 | 65,989 | -15.7% |
| LLm | 3,090 | 2,679 | 2,600 | -13.3% |
| Bc | 1,688,769 | 3,294,629 | 3,295,702 | +95.1% |
| Bm | 141,610 | 147,134 | 147,998 | +3.9% |
| CEst | 12,470,684 | 20,014,213 | 20,346,775 | +60.5% |

## std::function: deterministic performance (callgrind, cache+branch sim)

function workload, F=4 G=2 n=6000; each order's empty-main startup counts (crt + ld.so, which the lean-so order pays extra for shared-object resolution) are measured separately and subtracted — what remains is the workload itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle estimate). Counts are synthetic and repeatable — immune to host clock jitter.

| event | base | lean-static | lean-so | lean-static vs base |
|---|---|---|---|---|
| Ir | 1,104,876 | 1,059,742 | 1,058,295 | -4.1% |
| Dr | 336,633 | 192,473 | 192,647 | -42.8% |
| Dw | 144,371 | 96,276 | 96,354 | -33.3% |
| L1m | 119 | 60 | 41 | -49.6% |
| LLm | 94 | 51 | 47 | -45.7% |
| Bc | 96,198 | 96,397 | 96,282 | +0.2% |
| Bm | 50 | 675 | -400 | +1250.0% |
| CEst | 1,115,966 | 1,072,192 | 1,059,405 | -3.9% |

## std::variant: deterministic performance (callgrind, cache+branch sim)

variant workload, T=4 C=2 n=1500; each order's empty-main startup counts (crt + ld.so, which the lean-so order pays extra for shared-object resolution) are measured separately and subtracted — what remains is the workload itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle estimate). Counts are synthetic and repeatable — immune to host clock jitter.

| event | base | lean-static | lean-so | lean-static vs base |
|---|---|---|---|---|
| Ir | 12,494,981 | 15,630,842 | 15,944,847 | +25.1% |
| Dr | 3,879,502 | 4,419,639 | 4,577,311 | +13.9% |
| Dw | 3,794,132 | 3,542,048 | 3,410,567 | -6.6% |
| L1m | 292 | 309 | 207 | +5.8% |
| LLm | 278 | 280 | 165 | +0.7% |
| Bc | 2,448,392 | 2,544,750 | 2,449,046 | +3.9% |
| Bm | 111,354 | 60,919 | 110,872 | -45.3% |
| CEst | 13,639,241 | 16,271,122 | 17,072,137 | +19.3% |

## std::format: deterministic performance (callgrind, cache+branch sim)

format workload, F=4 G=2 n=1500; each order's empty-main startup counts (crt + ld.so, which the lean-so order pays extra for shared-object resolution) are measured separately and subtracted — what remains is the workload itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle estimate). Counts are synthetic and repeatable — immune to host clock jitter.

| event | base | lean-static | lean-so | lean-static vs base |
|---|---|---|---|---|
| Ir | 179,236,282 | 181,386,249 | 186,769,961 | +1.2% |
| Dr | 42,816,281 | 43,416,353 | 46,292,199 | +1.4% |
| Dw | 30,823,250 | 31,399,250 | 31,297,176 | +1.9% |
| L1m | 146,144 | 244,175 | 423,568 | +67.1% |
| LLm | 656 | 660 | 684 | +0.6% |
| Bc | 24,695,342 | 24,875,548 | 24,787,095 | +0.7% |
| Bm | 1,485,704 | 1,518,153 | 3,079,428 | +2.2% |
| CEst | 195,620,362 | 199,075,529 | 221,868,321 | +1.8% |

## iostreams (cout/cin): deterministic performance (callgrind, cache+branch sim)

stream workload, T=4 C=2 n=1500; each order's empty-main startup counts (crt + ld.so, which the lean-so order pays extra for shared-object resolution) are measured separately and subtracted — what remains is the workload itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle estimate). Counts are synthetic and repeatable — immune to host clock jitter.

| event | base | lean-static | lean-so | lean-static vs base |
|---|---|---|---|---|
| Ir | 228,823,392 | 218,713,295 | 219,547,358 | -4.4% |
| Dr | 57,658,915 | 54,920,967 | 55,523,082 | -4.7% |
| Dw | 43,244,241 | 39,642,756 | 39,415,400 | -8.3% |
| L1m | 863,036 | 881,982 | 844,584 | +2.2% |
| LLm | 50,131 | 34,697 | 34,540 | -30.8% |
| Bc | 32,762,093 | 30,741,930 | 30,634,524 | -6.2% |
| Bm | 1,664,577 | 1,417,278 | 1,693,000 | -14.9% |
| CEst | 259,112,622 | 245,175,595 | 248,377,198 | -5.4% |

