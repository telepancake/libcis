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


(callgrind skipped: --quick)
