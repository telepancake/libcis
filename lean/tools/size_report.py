#!/usr/bin/env python3
"""Measure base-vs-lean marginal code size and object sizes; write lean/SIZES.md.

For each feature program: compile at -Os with the standard -nodefaultlibs link
recipe against (a) the base include order and (b) the lean overlay order,
subtract the empty-main baseline of the same include order, and report the
marginal text+data+bss. Every number is measured with size(1) on a real
binary — nothing is estimated. Programs sink results through volatile and take
sizes from argc so the optimizer cannot delete the container code.
"""
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.chdir(ROOT)

CXX = os.environ.get("CXX", "g++")
# -ffunction-sections/-fdata-sections + -Wl,--gc-sections (the same trick
# tools/gen_groups.py uses): each program's binary then carries ONLY the kernels
# it actually reaches. Without it the whole liblean kernels TU would land in
# every lean binary — including the empty-main baseline — poisoning the marginal
# subtraction. The flags are applied to BOTH include orders so base vs lean stays
# apples-to-apples.
FLAGS = ("-std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -Os -w "
         "-ffunction-sections -fdata-sections").split()
GC = ["-Wl,--gc-sections"]
LINK_TAIL = "-nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc".split()
# Three deployment shapes are measured:
#   base    — the base library (no kernels TU exists).
#   lean    — kernels statically linked into the binary (gc-sections keeps only
#             the reached slice): the per-BINARY cost on a static/single-image
#             target.
#   lean-so — kernels resolved from liblean.so (built once below): the
#             per-binary cost when the kernels ship once per SYSTEM.
LEAN_INC = ["-nostdinc++", "-Ilean/include", "-Iinclude"]
LINKS = {
    "base": ["src/support.cpp"] + GC + LINK_TAIL,
    "lean": ["src/support.cpp", "lean/src/kernels.cpp"] + GC + LINK_TAIL,
    "lean-so": None,  # filled in main() once liblean.so is built
}
ORDERS = {
    "base": ["-nostdinc++", "-Iinclude"],
    "lean": LEAN_INC,
    "lean-so": LEAN_INC,
}

COMMON = r"""
template<class T> void sink(T v) { volatile T s = v; (void)s; }
"""

PROGRAMS = {
    "baseline": "int main(int argc, char**) { return argc - 1; }",
    "string": COMMON + r"""
#include <string>
int main(int argc, char** argv) {
  std::string s(argv[0]);
  s += "suffix";
  s.insert(1, "mid");
  s.erase(2, 1);
  std::string t = s.substr(1, (unsigned)argc + 5);
  sink(s.find("uf"));
  sink(s.compare(t));
  sink(s.c_str()[0]);
  s.resize((unsigned)argc * 40, 'x');
  sink(s.size());
  return t.empty();
}
""",
    "vector_int": COMMON + r"""
#include <vector>
int main(int argc, char**) {
  std::vector<int> v;
  for (int i = 0; i < argc * 100; ++i) v.push_back(i);
  v.insert(v.begin() + 1, 7);
  v.erase(v.begin());
  v.resize((unsigned)argc * 50);
  sink(v[0]); sink(v.size()); sink(v.capacity());
  return v.empty();
}
""",
    "vector_string": COMMON + r"""
#include <vector>
#include <string>
int main(int argc, char** argv) {
  std::vector<std::string> v;
  for (int i = 0; i < argc * 10; ++i) v.push_back(std::string(argv[0]) + "x");
  v.insert(v.begin(), std::string("head"));
  v.erase(v.begin());
  sink(v[0].size()); sink(v.size());
  return v.empty();
}
""",
    "sort_int": COMMON + r"""
#include <algorithm>
#include <vector>
int main(int argc, char**) {
  std::vector<int> v;
  for (int i = 0; i < argc * 100; ++i) v.push_back((i * 2654435761u) >> 7);
  std::sort(v.begin(), v.end());
  sink(v[0]);
  return !std::is_sorted(v.begin(), v.end());
}
""",
    "sort_string": COMMON + r"""
#include <algorithm>
#include <string>
#include <vector>
int main(int argc, char** argv) {
  std::vector<std::string> v;
  for (int i = 0; i < argc * 20; ++i) v.push_back(std::string(argv[0]) + char('a' + i % 26));
  std::sort(v.begin(), v.end());
  std::stable_sort(v.begin(), v.end(),
                   [](const std::string& a, const std::string& b) { return a.size() < b.size(); });
  sink(v[0].size());
  return !std::is_sorted(v.begin(), v.end(),
                         [](const std::string& a, const std::string& b) { return a.size() < b.size(); });
}
""",
    "map_int": COMMON + r"""
#include <map>
int main(int argc, char**) {
  std::map<int, int> m;
  for (int i = 0; i < argc * 100; ++i) m[(i * 2654435761u) >> 7] = i;
  m.erase(m.begin());
  sink(m.count(3)); sink(m.find(4) == m.end()); sink(m.size());
  int acc = 0;
  for (auto& kv : m) acc += kv.second;
  sink(acc);
  return m.empty();
}
""",
    "map_string": COMMON + r"""
#include <map>
#include <string>
int main(int argc, char** argv) {
  std::map<std::string, std::string> m;
  for (int i = 0; i < argc * 20; ++i)
    m[std::string(argv[0]) + char('a' + i % 26)] = "v";
  sink(m.count(argv[0])); sink(m.size());
  m.erase(m.begin());
  return m.empty();
}
""",
    "unordered_int": COMMON + r"""
#include <unordered_map>
int main(int argc, char**) {
  std::unordered_map<int, int> m;
  for (int i = 0; i < argc * 100; ++i) m[i] = i * 3;
  m.erase(1);
  sink(m.count(3)); sink(m.find(4) == m.end()); sink(m.size());
  return m.empty();
}
""",
    "list_int": COMMON + r"""
#include <list>
int main(int argc, char**) {
  std::list<int> l;
  for (int i = 0; i < argc * 50; ++i) l.push_back(i);
  l.push_front(-1);
  l.pop_back();
  l.reverse();
  l.sort();
  sink(l.front()); sink(l.size());
  return l.empty();
}
""",
    "deque_int": COMMON + r"""
#include <deque>
int main(int argc, char**) {
  std::deque<int> d;
  for (int i = 0; i < argc * 100; ++i) { d.push_back(i); d.push_front(-i); }
  d.pop_front(); d.pop_back();
  sink(d[3]); sink(d.size());
  return d.empty();
}
""",
}

SIZEOF_PROBE = r"""
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <list>
#include <deque>
int main() {
  std::printf("string=%zu vector=%zu map=%zu set=%zu unordered_map=%zu list=%zu deque=%zu\n",
              sizeof(std::string), sizeof(std::vector<int>), sizeof(std::map<int, int>),
              sizeof(std::set<int>), sizeof(std::unordered_map<int, int>),
              sizeof(std::list<int>), sizeof(std::deque<int>));
  return 0;
}
"""


def build(src_text, inc, link, exe):
    src = exe + ".cpp"
    open(src, "w").write(src_text)
    p = subprocess.run([CXX] + FLAGS + inc + [src] + link + ["-o", exe],
                       capture_output=True, text=True)
    if p.returncode != 0:
        return p.stderr
    return None


def kernels_object_text(tmp):
    """size(1) text of the full lean kernels .o — the one-copy-per-SYSTEM cost
    shipped in liblean.a / liblean.so, independent of any single program."""
    obj = os.path.join(tmp, "kernels.o")
    p = subprocess.run([CXX] + FLAGS + ORDERS["lean"] +
                       ["-c", "lean/src/kernels.cpp", "-o", obj],
                       capture_output=True, text=True)
    if p.returncode != 0:
        return None
    return tdb(obj)[0]


def tdb(exe):
    """(text, data, bss) from size(1)."""
    out = subprocess.run(["size", exe], capture_output=True, text=True).stdout
    f = out.splitlines()[1].split()
    return int(f[0]), int(f[1]), int(f[2])


def build_liblean_so(tmp):
    so = os.path.join(tmp, "liblean.so")
    p = subprocess.run([CXX] + FLAGS + LEAN_INC +
                       ["-fPIC", "-shared", "lean/src/kernels.cpp", "-o", so],
                       capture_output=True, text=True)
    if p.returncode != 0:
        sys.exit(f"liblean.so build failed:\n{p.stderr}")
    return so


def main():
    tmp = tempfile.mkdtemp(prefix="lean_size_")
    so = build_liblean_so(tmp)
    # rpath so the measured binaries also RUN (the report executes them).
    LINKS["lean-so"] = (["src/support.cpp", so, f"-Wl,-rpath,{tmp}"]
                        + GC + LINK_TAIL)
    rows = []       # (prog, {order: (marginal_text, marginal_data, marginal_bss) | error})
    baselines = {}
    failures = []
    for order, inc in ORDERS.items():
        exe = os.path.join(tmp, f"baseline_{order}")
        err = build(PROGRAMS["baseline"], inc, LINKS[order], exe)
        if err:
            sys.exit(f"baseline failed for {order}:\n{err}")
        baselines[order] = tdb(exe)
    for prog, text in PROGRAMS.items():
        if prog == "baseline":
            continue
        row = {}
        for order, inc in ORDERS.items():
            exe = os.path.join(tmp, f"{prog}_{order}")
            err = build(text, inc, LINKS[order], exe)
            if err:
                row[order] = None
                failures.append((prog, order, err.strip().splitlines()[:8]))
                continue
            rc = subprocess.run([exe, "a"], capture_output=True).returncode
            if rc != 0:
                row[order] = None
                failures.append((prog, order, [f"runtime exit={rc}"]))
                continue
            t, d, b = tdb(exe)
            bt, bd, bb = baselines[order]
            row[order] = (t - bt, d - bd, b - bb)
        rows.append((prog, row))

    sizeofs = {}
    for order in ("base", "lean"):  # lean-so has identical layouts to lean
        exe = os.path.join(tmp, f"sizeof_{order}")
        err = build(SIZEOF_PROBE, ORDERS[order], LINKS[order], exe)
        sizeofs[order] = ("BUILD FAILED" if err else
                          subprocess.run([exe], capture_output=True, text=True).stdout.strip())

    kern_text = kernels_object_text(tmp)

    L = ["# lean vs base: measured size (-Os, marginal over empty main)\n",
         f"Compiler: `{CXX}`. text+data+bss from `size(1)`, minus the same "
         "include-order's empty-`main` baseline. Every binary is linked with "
         "`-ffunction-sections -fdata-sections -Wl,--gc-sections`, so a program "
         "carries only the kernels it reaches. `lean` links the kernels TU "
         "statically (per-binary cost, single-image targets); `lean-so` links "
         "against `liblean.so` (per-binary cost when the kernels ship once per "
         "system). A FAIL row means that program did not build/run — a missing "
         "number is a defect, not a zero.\n",
         "| program | base | lean (static) | delta | lean-so (shared kernels) | delta |",
         "|---|---|---|---|---|---|"]
    for prog, row in rows:
        cells = {}
        for order in ORDERS:
            v = row[order]
            cells[order] = "FAIL" if v is None else f"{sum(v)} ({v[0]}+{v[1]}+{v[2]})"

        def delta(order):
            if not (row["base"] and row[order]):
                return "-"
            b, l = sum(row["base"]), sum(row[order])
            return f"{100.0 * (l - b) / b:+.0f}%" if b else "-"
        L.append(f"| {prog} | {cells['base']} | {cells['lean']} | {delta('lean')} "
                 f"| {cells['lean-so']} | {delta('lean-so')} |")
    L += ["\n## liblean kernels object (one copy per SYSTEM)\n",
          "The out-of-line structural/algorithm kernels compile to a single "
          "translation unit, shipped once as `liblean.a` / `liblean.so`. With "
          "`--gc-sections` each program above pulls in only the slice it uses; "
          "this is the whole-TU `size(1)` **text** cost when the library is "
          "linked in full:",
          f"- `lean/src/kernels.cpp` .o text: "
          f"`{'BUILD FAILED' if kern_text is None else kern_text} bytes`"]
    L += ["\n## sizeof (bytes)\n"]
    for order in sizeofs:
        L.append(f"- {order}: `{sizeofs[order]}`")
    if failures:
        L.append("\n## FAILURES (loud, not hidden)\n")
        for prog, order, lines in failures:
            L.append(f"- **{prog} [{order}]**:")
            L += [f"  - `{ln}`" for ln in lines]
    L.append("")
    open("lean/SIZES.md", "w").write("\n".join(L))
    print("\n".join(L))
    print(f"wrote lean/SIZES.md ({len(failures)} failures)")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
