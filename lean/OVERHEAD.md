# Overhead decomposition + deterministic perf delta (base vs lean)

Compiler: `g++` at -Os, gc-sections, standard -nodefaultlibs recipe. Sizes are text+data+bss minus the empty-main baseline of the same include order. Model S = FIXED + PER_TYPE*T + PER_CALLSITE*T*C fitted by finite differences on grid [(1, 1), (9, 1), (9, 9)]; held-out point (5, 5) reports fit error. liblean.so (once per SYSTEM, lean-so rows only): 133484 bytes.

## Containers + sort: size decomposition (bytes)

Workload: T structurally-identical types x C noinline callsites, each doing vector fill/insert/erase + sort (distinct lambda per callsite) + map insert/iterate/erase.

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 1,217 | 2,114 | 1,484 | -3.3% |
| lean-static | 4,927 | 703 | 721 | -6.8% |
| lean-so | 1,267 | 723 | 721 | -7.5% |

- delta lean-static vs base: fixed 3,710, per type -1,410, per callsite -764
- delta lean-so vs base:     fixed 50, per type -1,390, per callsite -764

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 1,217 + 2,114*T_p + 1,484*T_p*C_p ]
lean-static: sum_p [ 4,927 + 703*T_p + 721*T_p*C_p ]
lean-so:     133484  +  sum_p [ 1,267 + 723*T_p + 721*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 4815 | 6351 | 2711 |
| (9, 1) | 33599 | 17746 | 14261 |
| (9, 9) | 140483 | 69656 | 66153 |
| (5, 5) | 50541 | 28389 | 24765 |

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
| lean-so | 719 | 584 | 729 | -5.1% |

- delta lean-static vs base: fixed -174, per type -282, per callsite -5.0
- delta lean-so vs base:     fixed 80, per type -269, per callsite -6.3

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 639 + 853*T_p + 735*T_p*C_p ]
lean-static: sum_p [ 465 + 571*T_p + 730*T_p*C_p ]
lean-so:     133484  +  sum_p [ 719 + 584*T_p + 729*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 2227 | 1766 | 2032 |
| (9, 1) | 14935 | 12178 | 12538 |
| (9, 9) | 67877 | 64760 | 65024 |
| (5, 5) | 24341 | 22764 | 23028 |

## std::format: size decomposition (bytes)

Workload: F user types each with its own std::formatter x G noinline callsites, each formatting a mix (int, hex int, float, string, the user type) via std::format + format_to_n + formatted_size. The FIXED slice is the runtime engine (the vformat_to dispatch loop + fp_to_chars' snprintf float path + the integer/grouping/padding/fill write helpers) that base emits once per binary. The lean overlay relocates it out-of-line (fmt::vformat_engine<CharT>, extern-template'd in <format>, defined + explicitly instantiated in kernels.cpp): STILL per-binary for lean-static, but ONCE PER SYSTEM (liblean.so) for lean-so. lean-so's tiny FIXED vs base's ~41 KB FIXED is the move this axis exists to show. The per-type slice is the user formatter; the per-callsite slice is make_format_args + the thin out-of-line engine call.

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 42,564 | 406 | 510 | -1.3% |
| lean-static | 42,247 | 408 | 508 | -1.2% |
| lean-so | 3,794 | 406 | 508 | -3.9% |

- delta lean-static vs base: fixed -317, per type 1.9, per callsite -1.9
- delta lean-so vs base:     fixed -38,770, per type 0.0, per callsite -2.0

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 42,564 + 406*T_p + 510*T_p*C_p ]
lean-static: sum_p [ 42,247 + 408*T_p + 508*T_p*C_p ]
lean-so:     133484  +  sum_p [ 3,794 + 406*T_p + 508*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 43480 | 43163 | 4708 |
| (9, 1) | 50808 | 50491 | 12020 |
| (9, 9) | 87512 | 87059 | 48580 |
| (5, 5) | 58092 | 57703 | 19272 |


(callgrind skipped: --quick)
