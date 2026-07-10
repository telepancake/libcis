# Overhead decomposition + deterministic perf delta (base vs lean)

Compiler: `g++` at -Os, gc-sections, standard -nodefaultlibs recipe. Sizes are text+data+bss minus the empty-main baseline of the same include order. Model S = FIXED + PER_TYPE*T + PER_CALLSITE*T*C fitted by finite differences on grid [(1, 1), (9, 1), (9, 9)]; held-out point (5, 5) reports fit error. liblean.so (once per SYSTEM, lean-so rows only): 24571 bytes.

## Containers + sort: size decomposition (bytes)

Workload: T structurally-identical types x C noinline callsites, each doing vector fill/insert/erase + sort (distinct lambda per callsite) + map insert/iterate/erase.

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 1,217 | 2,114 | 1,484 | -3.3% |
| lean-static | 4,925 | 706 | 721 | -6.8% |
| lean-so | 1,267 | 723 | 721 | -7.5% |

- delta lean-static vs base: fixed 3,708, per type -1,408, per callsite -764
- delta lean-so vs base:     fixed 50, per type -1,390, per callsite -764

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 1,217 + 2,114*T_p + 1,484*T_p*C_p ]
lean-static: sum_p [ 4,925 + 706*T_p + 721*T_p*C_p ]
lean-so:     24571  +  sum_p [ 1,267 + 723*T_p + 721*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 4815 | 6351 | 2711 |
| (9, 1) | 33599 | 17762 | 14261 |
| (9, 9) | 140483 | 69656 | 66153 |
| (5, 5) | 50541 | 28405 | 24765 |

## std::function: size decomposition (bytes)

Workload: F receiver types x G noinline callsites, each constructing a one-word-capture lambda function, calling it in a loop, plus a plain-fn-pointer function and a copy — the dominant callable shapes.

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 851 | 101 | 914 | -0.2% |
| lean-static | 183 | 124 | 359 | -1.4% |
| lean-so | 175 | 131 | 359 | -1.7% |

- delta lean-static vs base: fixed -668, per type 23, per callsite -555
- delta lean-so vs base:     fixed -675, per type 30, per callsite -555

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 851 + 101*T_p + 914*T_p*C_p ]
lean-static: sum_p [ 183 + 124*T_p + 359*T_p*C_p ]
lean-so:     24571  +  sum_p [ 175 + 131*T_p + 359*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 1865 | 665 | 665 |
| (9, 1) | 9979 | 4524 | 4582 |
| (9, 9) | 75763 | 30356 | 30414 |
| (5, 5) | 24243 | 9912 | 9970 |

## std::variant: size decomposition (bytes)

Workload: T variant types (variant<long, Alt_k, double> with a distinct string-member Alt_k per type — non-trivial, so the table-driven engine is exercised) x C noinline callsites, each doing cross- and same-index assign, copy, move and a visit. The per-type slice is the special-member machinery lean erases; the per-callsite slice is the user visitor lambda (not a lean target — the visitor cannot be erased without losing its typed result).

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 639 | 853 | 735 | -4.3% |
| lean-static | 488 | 554 | 732 | -5.3% |
| lean-so | 747 | 553 | 732 | -5.3% |

- delta lean-static vs base: fixed -150, per type -300, per callsite -3.3
- delta lean-so vs base:     fixed 108, per type -300, per callsite -3.2

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 639 + 853*T_p + 735*T_p*C_p ]
lean-static: sum_p [ 488 + 554*T_p + 732*T_p*C_p ]
lean-so:     24571  +  sum_p [ 747 + 553*T_p + 732*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 2227 | 1774 | 2032 |
| (9, 1) | 14935 | 12058 | 12316 |
| (9, 9) | 67877 | 64760 | 65024 |
| (5, 5) | 24341 | 22764 | 23028 |

## Containers + sort: deterministic performance (callgrind, cache+branch sim)

Container workload, T=4 C=2 n=1500; each order's empty-main startup counts (crt + ld.so, which the lean-so order pays extra for shared-object resolution) are measured separately and subtracted — what remains is the workload itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle estimate). Counts are synthetic and repeatable — immune to host clock jitter.

| event | base | lean-static | lean-so | lean-static vs base |
|---|---|---|---|---|
| Ir | 9,964,678 | 17,616,875 | 17,946,671 | +76.8% |
| Dr | 2,509,951 | 4,130,193 | 4,293,946 | +64.6% |
| Dw | 1,480,633 | 2,643,247 | 2,644,035 | +78.5% |
| L1m | 78,096 | 65,814 | 66,201 | -15.7% |
| LLm | 3,088 | 2,676 | 2,610 | -13.3% |
| Bc | 1,688,770 | 3,294,629 | 3,295,495 | +95.1% |
| Bm | 141,610 | 147,524 | 188,673 | +4.2% |
| CEst | 12,470,538 | 20,017,855 | 20,756,411 | +60.5% |

## std::function: deterministic performance (callgrind, cache+branch sim)

function workload, F=4 G=2 n=6000; each order's empty-main startup counts (crt + ld.so, which the lean-so order pays extra for shared-object resolution) are measured separately and subtracted — what remains is the workload itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle estimate). Counts are synthetic and repeatable — immune to host clock jitter.

| event | base | lean-static | lean-so | lean-static vs base |
|---|---|---|---|---|
| Ir | 1,104,890 | 1,059,758 | 1,058,265 | -4.1% |
| Dr | 336,641 | 192,482 | 192,654 | -42.8% |
| Dw | 144,371 | 96,276 | 96,354 | -33.3% |
| L1m | 114 | 62 | 52 | -45.6% |
| LLm | 92 | 52 | 49 | -43.5% |
| Bc | 96,199 | 96,398 | 96,279 | +0.2% |
| Bm | 50 | 676 | -434 | +1252.0% |
| CEst | 1,115,730 | 1,072,338 | 1,059,345 | -3.9% |

## std::variant: deterministic performance (callgrind, cache+branch sim)

variant workload, T=4 C=2 n=1500; each order's empty-main startup counts (crt + ld.so, which the lean-so order pays extra for shared-object resolution) are measured separately and subtracted — what remains is the workload itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle estimate). Counts are synthetic and repeatable — immune to host clock jitter.

| event | base | lean-static | lean-so | lean-static vs base |
|---|---|---|---|---|
| Ir | 12,494,967 | 15,678,872 | 15,946,233 | +25.5% |
| Dr | 3,879,494 | 4,431,656 | 4,577,302 | +14.2% |
| Dw | 3,794,132 | 3,542,048 | 3,410,506 | -6.6% |
| L1m | 289 | 312 | 231 | +8.0% |
| LLm | 276 | 280 | 181 | +1.4% |
| Bc | 2,448,391 | 2,556,752 | 2,449,151 | +4.4% |
| Bm | 111,354 | 62,414 | 86,848 | -43.9% |
| CEst | 13,638,997 | 16,334,132 | 16,835,123 | +19.8% |

