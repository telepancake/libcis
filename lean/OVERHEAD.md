# Overhead decomposition + deterministic perf delta (base vs lean)

Compiler: `g++` at -Os, gc-sections, standard -nodefaultlibs recipe. Generated program: T structurally-identical types x C noinline callsites each doing vector fill/insert/erase + sort (distinct lambda per callsite) + map insert/iterate/erase. Sizes are text+data+bss minus the empty-main baseline of the same include order. Model S = FIXED + PER_TYPE*T + PER_CALLSITE*T*C fitted on grid [(1, 1), (9, 1), (9, 9)], held-out point (5, 5) reports fit error.

## Size decomposition (bytes)

| profile | fixed / process | per type | per callsite | fit err on held-out |
|---|---|---|---|---|
| base | 1,217 | 2,114 | 1,484 | -3.3% |
| lean-static | 4,925 | 706 | 721 | -6.8% |
| lean-so | 1,267 | 723 | 721 | -7.5% |

- delta lean-static vs base: fixed 3,708, per type -1,408, per callsite -764
- delta lean-so vs base:     fixed 50, per type -1,390, per callsite -764
- liblean.so (once per SYSTEM, lean-so only): 23119 bytes

System-wide model, P processes each with (T,C):
```
base:        sum_p [ 1,217 + 2,114*T_p + 1,484*T_p*C_p ]
lean-static: sum_p [ 4,925 + 706*T_p + 721*T_p*C_p ]
lean-so:     23119  +  sum_p [ 1,267 + 723*T_p + 721*T_p*C_p ]
```

Raw grid (bytes over baseline):
| (T,C) | base | lean-static | lean-so |
|---|---|---|---|
| (1, 1) | 4815 | 6351 | 2711 |
| (9, 1) | 33599 | 17762 | 14261 |
| (9, 9) | 140483 | 69656 | 66153 |
| (5, 5) | 50541 | 28405 | 24765 |

## Deterministic performance (callgrind, cache+branch sim)

Same generated workload, T=4 C=2 n=1500; each order's empty-main startup counts (crt + ld.so, which the lean-so order pays extra for shared-object resolution) are measured separately and subtracted — what remains is the workload itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle estimate). Counts are synthetic and repeatable — immune to host clock jitter.

| event | base | lean-static | lean-so | lean-static vs base |
|---|---|---|---|---|
| Ir | 9,964,667 | 17,616,855 | 17,946,675 | +76.8% |
| Dr | 2,509,950 | 4,130,184 | 4,293,944 | +64.6% |
| Dw | 1,480,633 | 2,643,247 | 2,644,035 | +78.5% |
| L1m | 78,073 | 65,797 | 66,199 | -15.7% |
| LLm | 3,084 | 2,678 | 2,607 | -13.2% |
| Bc | 1,688,768 | 3,294,627 | 3,295,498 | +95.1% |
| Bm | 141,609 | 147,523 | 187,987 | +4.2% |
| CEst | 12,469,887 | 20,017,855 | 20,749,235 | +60.5% |
