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
FLAGS = "-std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -Os -w".split()
LINK = "src/support.cpp -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc".split()
ORDERS = {
    "base": ["-nostdinc++", "-Iinclude"],
    "lean": ["-nostdinc++", "-Ilean/include", "-Iinclude"],
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


def build(src_text, inc, exe):
    src = exe + ".cpp"
    open(src, "w").write(src_text)
    p = subprocess.run([CXX] + FLAGS + inc + [src] + LINK + ["-o", exe],
                       capture_output=True, text=True)
    if p.returncode != 0:
        return p.stderr
    return None


def tdb(exe):
    """(text, data, bss) from size(1)."""
    out = subprocess.run(["size", exe], capture_output=True, text=True).stdout
    f = out.splitlines()[1].split()
    return int(f[0]), int(f[1]), int(f[2])


def main():
    tmp = tempfile.mkdtemp(prefix="lean_size_")
    rows = []       # (prog, {order: (marginal_text, marginal_data, marginal_bss) | error})
    baselines = {}
    failures = []
    for order, inc in ORDERS.items():
        exe = os.path.join(tmp, f"baseline_{order}")
        err = build(PROGRAMS["baseline"], inc, exe)
        if err:
            sys.exit(f"baseline failed for {order}:\n{err}")
        baselines[order] = tdb(exe)
    for prog, text in PROGRAMS.items():
        if prog == "baseline":
            continue
        row = {}
        for order, inc in ORDERS.items():
            exe = os.path.join(tmp, f"{prog}_{order}")
            err = build(text, inc, exe)
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
    for order, inc in ORDERS.items():
        exe = os.path.join(tmp, f"sizeof_{order}")
        err = build(SIZEOF_PROBE, inc, exe)
        sizeofs[order] = ("BUILD FAILED" if err else
                          subprocess.run([exe], capture_output=True, text=True).stdout.strip())

    L = ["# lean vs base: measured size (-Os, marginal over empty main)\n",
         f"Compiler: `{CXX}`. text+data+bss from `size(1)`, minus the same "
         "include-order's empty-`main` baseline. A FAIL row means that program "
         "did not build/run — a missing number is a defect, not a zero.\n",
         "| program | base text+data+bss | lean text+data+bss | delta |",
         "|---|---|---|---|"]
    for prog, row in rows:
        cells = {}
        for order in ORDERS:
            v = row[order]
            cells[order] = "FAIL" if v is None else f"{sum(v)} ({v[0]}+{v[1]}+{v[2]})"
        if row["base"] and row["lean"]:
            b, l = sum(row["base"]), sum(row["lean"])
            delta = f"{100.0 * (l - b) / b:+.0f}%" if b else "-"
        else:
            delta = "-"
        L.append(f"| {prog} | {cells['base']} | {cells['lean']} | {delta} |")
    L += ["\n## sizeof (bytes)\n"]
    for order in ORDERS:
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
