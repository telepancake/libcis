#!/usr/bin/env python3
"""Parametric overhead decomposition + callgrind performance delta, base vs lean.

Generates one test program parameterized by (T types, C callsites per type),
compiles it at -Os against base / lean-static / lean-so, and decomposes binary
size into:

    S(T, C)  =  crt-baseline  +  FIXED  +  PER_TYPE * T  +  PER_CALLSITE * T*C

by finite differences over a grid, with a held-out grid point reported as fit
error (a bad fit means the linear model is lying — that is reported, not
hidden). System-wide cost is then FIXED per process plus, for lean-so, the
liblean.so text once per system.

Every generated type is structurally identical (same layout, same work) but
differs in comparator/salt constants, so template instantiations cannot be
merged and neither library is flattered. Each callsite is a distinct
__attribute__((noinline)) function with its own lambda comparator: for base
that instantiates a full std::sort per callsite, for lean only a thunk — the
exact effect the lean profile exists to remove. Work volume comes from argv,
so nothing constant-folds away.

Performance: the same program (fixed mid-size params) runs under
    valgrind --tool=callgrind --cache-sim=yes --branch-sim=yes
whose synthetic instruction/cache/branch counts are deterministic — wall-clock
on a shared VPS is not. Reported: raw event totals and the kcachegrind cycle
estimate  CEst = Ir + 10*Bm + 10*L1m + 100*LLm.

Usage: python3 lean/tools/overhead_matrix.py [--quick]
Writes lean/OVERHEAD.md. Exit 1 on any build/run/measure failure (loud).
"""
import os
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.chdir(ROOT)

CXX = os.environ.get("CXX", "g++")
FLAGS = ("-std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -Os -w "
         "-ffunction-sections -fdata-sections").split()
GC = ["-Wl,--gc-sections"]
TAIL = "-nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc".split()
LEAN_INC = ["-nostdinc++", "-Ilean/include", "-Iinclude"]
ORDERS = {
    "base": ["-nostdinc++", "-Iinclude"],
    "lean-static": LEAN_INC,
    "lean-so": LEAN_INC,
}

# Size-model grid: three points define the plane, the fourth is held out and
# reported as fit error.
GRID = [(1, 1), (9, 1), (9, 9)]
HOLDOUT = (5, 5)
# Callgrind workload shape.
CG_T, CG_C, CG_N = 4, 2, 1500


def gen_program(types, callsites):
    L = ["#include <vector>", "#include <map>", "#include <algorithm>",
         "#include <cstdlib>",
         "template<class V> void sink(V v) { volatile V s = v; (void)s; }"]
    for k in range(types):
        L.append(f"struct Type{k} {{ long key; long a; long b; }};")
    for k in range(types):
        for j in range(callsites):
            salt = k * 1000 + j + 1
            L.append(f"""
__attribute__((noinline)) void work_t{k}_c{j}(int n) {{
  std::vector<Type{k}> v;
  for (int i = 0; i < n; ++i)
    v.push_back(Type{k}{{(long)((i * 2654435761u + {salt}u) % 8192u), i, i + {salt}}});
  v.insert(v.begin() + n / 3, Type{k}{{{salt}, 1, 2}});
  v.erase(v.begin() + n / 4);
  std::sort(v.begin(), v.end(),
            [](const Type{k}& x, const Type{k}& y) {{ return x.key + {salt} < y.key + {salt}; }});
  std::map<long, Type{k}> m;
  for (auto& e : v) m[e.key ^ {salt}] = e;
  m.erase(m.begin());
  long acc = 0;
  for (auto& kv : m) acc += kv.second.a;
  sink(acc); sink(v.size()); sink(m.size());
}}""")
    L.append("int main(int argc, char** argv) {")
    L.append("  int n = argc > 1 ? std::atoi(argv[1]) : 200;")
    for k in range(types):
        for j in range(callsites):
            L.append(f"  work_t{k}_c{j}(n);")
    L.append("  return 0;\n}")
    return "\n".join(L)


def gen_fn_program(types, callsites):
    """std::function workload: F distinct receiver/callable types x G noinline
    callsites, each exercising the dominant shapes (one-word-capture lambda,
    plain fn pointer, copy) and calling through the erased boundary."""
    L = ["#include <functional>", "#include <cstdlib>",
         "template<class V> void sink(V v) { volatile V s = v; (void)s; }"]
    for k in range(types):
        L.append(f"""
struct Recv{k} {{ long acc = 0; }};
__attribute__((noinline)) long recv_method{k}(Recv{k}* r, long x) {{
  r->acc += x + {k + 1}; return r->acc;
}}
__attribute__((noinline)) long free_fn{k}(long x) {{ return x * {2 * k + 3}; }}""")
    for k in range(types):
        for j in range(callsites):
            salt = k * 1000 + j + 1
            L.append(f"""
__attribute__((noinline)) long site_t{k}_c{j}(Recv{k}* r, int n) {{
  std::function<long(long)> fn = [r](long x) {{ return recv_method{k}(r, x + {salt}); }};
  long s = 0;
  for (int i = 0; i < n; ++i) s += fn(i);
  std::function<long(long)> fp = &free_fn{k};
  s += fp(n + {salt});
  std::function<long(long)> cp = fn;
  s += cp({salt});
  return s;
}}""")
    L.append("int main(int argc, char** argv) {")
    L.append("  int n = argc > 1 ? std::atoi(argv[1]) : 100;")
    L.append("  long total = 0;")
    for k in range(types):
        L.append(f"  Recv{k} r{k};")
        for j in range(callsites):
            L.append(f"  total += site_t{k}_c{j}(&r{k}, n);")
    L.append("  sink(total);\n  return 0;\n}")
    return "\n".join(L)


def gen_variant_program(types, callsites):
    """std::variant workload: T structurally-identical variant types x C noinline
    callsites, each exercising the special members the lean engine erases —
    copy/move-construct, cross- and same-index assign, destroy — plus a visit.
    Every type has ONE non-trivial (string-member) alternative, so the variant
    is NOT trivially copyable and the table-driven engine is exercised; long and
    double are the trivial alternatives. Modelled on the coordinator's probe."""
    L = ["#include <variant>", "#include <string>", "#include <cstdlib>",
         "template<class V> void sink(V v) { volatile V s = v; (void)s; }"]
    for k in range(types):
        # distinct non-trivial alternative per type: string member + salt
        L.append(f"struct Alt{k} {{ std::string s; long tag = {k}; }};")
    for k in range(types):
        for j in range(callsites):
            salt = k * 1000 + j + 1
            L.append(f"""
__attribute__((noinline)) long site_v{k}_c{j}(const char* a, int n) {{
  using V = std::variant<long, Alt{k}, double>;
  V v = long({salt});
  long acc = 0;
  for (int i = 0; i < n; ++i) {{
    v = Alt{k}{{std::string(a) + char('a' + i % 26), i + {salt}}};   // non-trivial assign
    V c = v;                                                        // copy switch
    V m = std::move(c);                                             // move switch
    acc += std::visit([](const auto& x) -> long {{                  // visit table
      if constexpr (std::is_same_v<std::decay_t<decltype(x)>, Alt{k}>)
        return x.tag + (long)x.s.size();
      else
        return (long)x;
    }}, m);
    v = double(i) + {salt};                                         // cross-index assign
    v = long(i);                                                    // cross-index assign
  }}
  return acc;
}}""")
    L.append("int main(int argc, char** argv) {\n  int n = argc > 1 ? std::atoi(argv[1]) : 50;\n  long t = 0;")
    for k in range(types):
        for j in range(callsites):
            L.append(f"  t += site_v{k}_c{j}(argv[0], n);")
    L.append("  sink(t);\n  return 0;\n}")
    return "\n".join(L)


def gen_format_program(types, callsites):
    """std::format workload: F distinct user types each with its own
    std::formatter specialization x G noinline callsites, each formatting a MIX
    that includes an int, a hex int, a float and the user type through
    std::format / format_to_n / formatted_size.

    The FIXED slice is the runtime formatting engine — the vformat_to dispatch
    loop, fp_to_chars' snprintf float path and the integer/grouping/padding/fill
    write helpers — which base emits once per binary. The lean overlay relocates
    that engine out-of-line (fmt::vformat_engine<CharT>, extern-template'd in
    <format>, defined + explicitly instantiated in kernels.cpp): still per-binary
    for lean-static, but ONCE PER SYSTEM for lean-so. lean-so's tiny FIXED vs
    base's ~41 KB FIXED is exactly the move this axis exists to show. The
    per-type slice is the user formatter parse/format; the per-callsite slice is
    make_format_args + the thin out-of-line engine call. Distinct salts keep the
    user formatters from merging, so neither library is flattered."""
    L = ["#include <format>", "#include <string>", "#include <iterator>",
         "#include <cstdlib>",
         "template<class V> void sink(V v) { volatile V s = v; (void)s; }"]
    for k in range(types):
        L.append(f"struct Ty{k} {{ long a; double b; }};")
    for k in range(types):
        salt = k + 1
        L.append(f"""
template<> struct std::formatter<Ty{k}, char> {{
  constexpr auto parse(std::basic_format_parse_context<char>& ctx) {{ return ctx.begin(); }}
  template<class Ctx> auto format(const Ty{k}& v, Ctx& ctx) const {{
    return std::format_to(ctx.out(), "Ty{k}({{}},{{:.2f}},{{}})", v.a + {salt}, v.b, {salt});
  }}
}};""")
    for k in range(types):
        for j in range(callsites):
            salt = k * 1000 + j + 1
            L.append(f"""
__attribute__((noinline)) long site_f{k}_c{j}(const char* a, int n) {{
  long acc = 0;
  for (int i = 0; i < n; ++i) {{
    std::string s = std::format("{{}}|{{:#x}}|{{:.3f}}|{{:>6}}|{{}}",
                                i + {salt}, (unsigned)(i * {salt}u), (i + {salt}) * 0.5,
                                a, Ty{k}{{i + {salt}, i * 0.25}});
    acc += (long)s.size();
    char buf[80];
    auto r = std::format_to_n(buf, sizeof(buf), "{{:e}}-{{:b}}", (i + {salt}) * 1.5, i + {salt});
    acc += (long)r.size;
    acc += (long)std::formatted_size("{{}}/{{}}", i, {salt});
  }}
  return acc;
}}""")
    L.append("int main(int argc, char** argv) {\n  int n = argc > 1 ? std::atoi(argv[1]) : 60;\n  long t = 0;")
    for k in range(types):
        for j in range(callsites):
            L.append(f"  t += site_f{k}_c{j}(argv[0], n);")
    L.append("  sink(t);\n  return 0;\n}")
    return "\n".join(L)


def run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def build(src_text, order, links, exe):
    src = exe + ".cpp"
    open(src, "w").write(src_text)
    p = run([CXX] + FLAGS + ORDERS[order] + [src] + links[order] + ["-o", exe])
    if p.returncode != 0:
        sys.exit(f"BUILD FAILED [{order}] {exe}:\n{p.stderr[-2000:]}")
    if run([exe, "50"]).returncode != 0:
        sys.exit(f"RUN FAILED [{order}] {exe}")


def text_size(path):
    out = run(["size", path]).stdout
    f = out.splitlines()[1].split()
    return int(f[0]) + int(f[1]) + int(f[2])   # text+data+bss


def make_links(tmp):
    so = os.path.join(tmp, "liblean.so")
    p = run([CXX] + FLAGS + LEAN_INC + ["-fPIC", "-shared",
             "lean/src/kernels.cpp", "-o", so])
    if p.returncode != 0:
        sys.exit(f"liblean.so build failed:\n{p.stderr[-2000:]}")
    return {
        "base": ["src/support.cpp"] + GC + TAIL,
        "lean-static": ["src/support.cpp", "lean/src/kernels.cpp"] + GC + TAIL,
        "lean-so": ["src/support.cpp", so, f"-Wl,-rpath,{tmp}"] + GC + TAIL,
    }, so


def measure_sizes(tmp, links, gen, tag):
    """S(T,C) minus the same order's empty-main baseline, per order."""
    S = {o: {} for o in ORDERS}
    for order in ORDERS:
        exe = os.path.join(tmp, f"empty_{order}")
        build("int main(int argc, char**){return argc > 99;}", order, links, exe)
        base = text_size(exe)
        for (t, c) in GRID + [HOLDOUT]:
            exe = os.path.join(tmp, f"{tag}_{order}_{t}_{c}")
            build(gen(t, c), order, links, exe)
            S[order][(t, c)] = text_size(exe) - base
    return S


def decompose(S):
    """Finite differences on the grid -> (fixed, per_type, per_callsite, fit_err%)."""
    (t0, c0), (t1, c1), (t2, c2) = GRID          # (1,1), (9,1), (9,9)
    per_cs = (S[(t2, c2)] - S[(t1, c1)]) / (t1 * (c2 - c1))
    per_ty = (S[(t1, c1)] - S[(t0, c0)]) / (t1 - t0) - per_cs * c0
    fixed = S[(t0, c0)] - per_ty * t0 - per_cs * t0 * c0
    th, ch = HOLDOUT
    pred = fixed + per_ty * th + per_cs * th * ch
    err = 100.0 * (pred - S[HOLDOUT]) / S[HOLDOUT] if S[HOLDOUT] else 0.0
    return fixed, per_ty, per_cs, err


def cg_events(exe, out, arg):
    p = run(["valgrind", "--tool=callgrind", "--cache-sim=yes",
             "--branch-sim=yes", f"--callgrind-out-file={out}", exe, arg])
    if p.returncode != 0:
        sys.exit(f"callgrind failed [{exe}]: {p.stderr[-1500:]}")
    events, summary = None, None
    for line in open(out):
        if line.startswith("events:"):
            events = line.split()[1:]
        elif line.startswith("summary:"):
            summary = [int(x) for x in line.split()[1:]]
    if not events or not summary:
        sys.exit(f"callgrind output unparsable [{exe}]")
    return dict(zip(events, summary))


def callgrind(tmp, links, gen, tag, n):
    """Deterministic event counts for the same workload, per order.

    Each order's empty-main binary is measured too and SUBTRACTED, so process
    startup (crt + ld.so — 2M+ instructions extra for the lean-so order's
    shared object resolution) does not masquerade as library cost. What
    remains is the workload itself."""
    res = {}
    for order in ORDERS:
        exe = os.path.join(tmp, f"cg_{tag}_{order}")
        build(gen(CG_T, CG_C), order, links, exe)
        empty = os.path.join(tmp, f"cg0_{order}")
        build("int main(int argc, char**){return argc > 99;}", order, links, empty)
        full = cg_events(exe, os.path.join(tmp, f"callgrind.{tag}.{order}"), str(n))
        startup = cg_events(empty, os.path.join(tmp, f"callgrind0.{tag}.{order}"), str(n))
        ev = {k: full[k] - startup.get(k, 0) for k in full}
        l1m = ev.get("I1mr", 0) + ev.get("D1mr", 0) + ev.get("D1mw", 0)
        llm = ev.get("ILmr", 0) + ev.get("DLmr", 0) + ev.get("DLmw", 0)
        bm = ev.get("Bim", 0) + ev.get("Bcm", 0)
        ev["L1m"], ev["LLm"], ev["Bm"] = l1m, llm, bm
        ev["CEst"] = ev["Ir"] + 10 * bm + 10 * l1m + 100 * llm
        res[order] = ev
    return res


def fmt(n):
    return f"{n:,.0f}" if abs(n) >= 10 else f"{n:.1f}"


def size_section(title, blurb, S, dec, so_text):
    L = [f"## {title}\n", blurb + "\n",
         "| profile | fixed / process | per type | per callsite | fit err on held-out |",
         "|---|---|---|---|---|"]
    for o in ORDERS:
        f_, t_, c_, e_ = dec[o]
        L.append(f"| {o} | {fmt(f_)} | {fmt(t_)} | {fmt(c_)} | {e_:+.1f}% |")
    fb, tb, cb, _ = dec["base"]
    fl, tl, cl, _ = dec["lean-static"]
    fs, ts, cs, _ = dec["lean-so"]
    L += ["",
          f"- delta lean-static vs base: fixed {fmt(fl - fb)}, per type {fmt(tl - tb)}, per callsite {fmt(cl - cb)}",
          f"- delta lean-so vs base:     fixed {fmt(fs - fb)}, per type {fmt(ts - tb)}, per callsite {fmt(cs - cb)}",
          "",
          "System-wide model, P processes each with (T,C):",
          "```",
          "base:        sum_p [ {} + {}*T_p + {}*T_p*C_p ]".format(fmt(fb), fmt(tb), fmt(cb)),
          "lean-static: sum_p [ {} + {}*T_p + {}*T_p*C_p ]".format(fmt(fl), fmt(tl), fmt(cl)),
          "lean-so:     {}  +  sum_p [ {} + {}*T_p + {}*T_p*C_p ]".format(so_text, fmt(fs), fmt(ts), fmt(cs)),
          "```",
          "",
          "Raw grid (bytes over baseline):",
          "| (T,C) | " + " | ".join(ORDERS) + " |",
          "|---|" + "---|" * len(ORDERS)]
    for pt in GRID + [HOLDOUT]:
        L.append(f"| {pt} | " + " | ".join(str(S[o][pt]) for o in ORDERS) + " |")
    L.append("")
    return L


def cg_section(title, cg, shape):
    L = [f"## {title} (callgrind, cache+branch sim)\n",
         f"{shape}; each order's empty-main startup counts (crt + ld.so, which "
         "the lean-so order pays extra for shared-object resolution) are "
         "measured separately and subtracted — what remains is the workload "
         "itself. CEst = Ir + 10*Bm + 10*L1m + 100*LLm (kcachegrind's cycle "
         "estimate). Counts are synthetic and repeatable — immune to host "
         "clock jitter.\n",
         "| event | " + " | ".join(ORDERS) + " | lean-static vs base |",
         "|---|" + "---|" * (len(ORDERS) + 1)]
    for k in ("Ir", "Dr", "Dw", "L1m", "LLm", "Bc", "Bm", "CEst"):
        row = [f"{cg[o].get(k, 0):,}" for o in ORDERS]
        b, l = cg["base"].get(k, 0), cg["lean-static"].get(k, 0)
        d = f"{100.0 * (l - b) / b:+.1f}%" if b else "-"
        L.append(f"| {k} | " + " | ".join(row) + f" | {d} |")
    L.append("")
    return L


def main():
    quick = "--quick" in sys.argv
    tmp = tempfile.mkdtemp(prefix="lean_ovh_")
    links, so = make_links(tmp)
    so_text = text_size(so)

    L = ["# Overhead decomposition + deterministic perf delta (base vs lean)\n",
         f"Compiler: `{CXX}` at -Os, gc-sections, standard -nodefaultlibs "
         "recipe. Sizes are text+data+bss minus the empty-main baseline of the "
         "same include order. Model S = FIXED + PER_TYPE*T + PER_CALLSITE*T*C "
         f"fitted by finite differences on grid {GRID}; held-out point "
         f"{HOLDOUT} reports fit error. "
         f"liblean.so (once per SYSTEM, lean-so rows only): {so_text} bytes.\n"]

    S = measure_sizes(tmp, links, gen_program, "cont")
    dec = {o: decompose(S[o]) for o in ORDERS}
    L += size_section(
        "Containers + sort: size decomposition (bytes)",
        "Workload: T structurally-identical types x C noinline callsites, each "
        "doing vector fill/insert/erase + sort (distinct lambda per callsite) "
        "+ map insert/iterate/erase.",
        S, dec, so_text)

    Sf = measure_sizes(tmp, links, gen_fn_program, "fn")
    decf = {o: decompose(Sf[o]) for o in ORDERS}
    L += size_section(
        "std::function: size decomposition (bytes)",
        "Workload: F receiver types x G noinline callsites, each constructing "
        "a one-word-capture lambda function, calling it in a loop, plus a "
        "plain-fn-pointer function and a copy — the dominant callable shapes.",
        Sf, decf, so_text)

    Sv = measure_sizes(tmp, links, gen_variant_program, "var")
    decv = {o: decompose(Sv[o]) for o in ORDERS}
    L += size_section(
        "std::variant: size decomposition (bytes)",
        "Workload: T variant types (variant<long, Alt_k, double> with a distinct "
        "string-member Alt_k per type — non-trivial, so the table-driven engine "
        "is exercised) x C noinline callsites, each doing cross- and same-index "
        "assign, copy, move and a visit. The per-type slice is the special-member "
        "machinery lean erases; the per-callsite slice is the user visitor lambda "
        "(not a lean target — the visitor cannot be erased without losing its "
        "typed result).",
        Sv, decv, so_text)

    Sfmt = measure_sizes(tmp, links, gen_format_program, "fmt")
    decfmt = {o: decompose(Sfmt[o]) for o in ORDERS}
    L += size_section(
        "std::format: size decomposition (bytes)",
        "Workload: F user types each with its own std::formatter x G noinline "
        "callsites, each formatting a mix (int, hex int, float, string, the user "
        "type) via std::format + format_to_n + formatted_size. The FIXED slice "
        "is the runtime engine (the vformat_to dispatch loop + fp_to_chars' "
        "snprintf float path + the integer/grouping/padding/fill write helpers) "
        "that base emits once per binary. The lean overlay relocates it "
        "out-of-line (fmt::vformat_engine<CharT>, extern-template'd in <format>, "
        "defined + explicitly instantiated in kernels.cpp): STILL per-binary for "
        "lean-static, but ONCE PER SYSTEM (liblean.so) for lean-so. lean-so's "
        "tiny FIXED vs base's ~41 KB FIXED is the move this axis exists to show. "
        "The per-type slice is the user formatter; the per-callsite slice is "
        "make_format_args + the thin out-of-line engine call.",
        Sfmt, decfmt, so_text)

    if quick:
        L.append("\n(callgrind skipped: --quick)")
    else:
        cg = callgrind(tmp, links, gen_program, "cont", CG_N)
        L += cg_section("Containers + sort: deterministic performance", cg,
                        f"Container workload, T={CG_T} C={CG_C} n={CG_N}")
        cgf = callgrind(tmp, links, gen_fn_program, "fn", CG_N * 4)
        L += cg_section("std::function: deterministic performance", cgf,
                        f"function workload, F={CG_T} G={CG_C} n={CG_N * 4}")
        cgv = callgrind(tmp, links, gen_variant_program, "var", CG_N)
        L += cg_section("std::variant: deterministic performance", cgv,
                        f"variant workload, T={CG_T} C={CG_C} n={CG_N}")
        cgfmt = callgrind(tmp, links, gen_format_program, "fmt", CG_N)
        L += cg_section("std::format: deterministic performance", cgfmt,
                        f"format workload, F={CG_T} G={CG_C} n={CG_N}")
    L.append("")
    open("lean/OVERHEAD.md", "w").write("\n".join(L))
    print("\n".join(L))
    print("wrote lean/OVERHEAD.md")


if __name__ == "__main__":
    main()
