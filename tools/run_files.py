#!/usr/bin/env python3
"""Run each transferred test of a subtree INDIVIDUALLY against a backend.

The consolidated feature-group TU is all-or-nothing: one compile error blanks
every test's signal.  For the foundational headers (string, vector, the big
containers) that is exactly what has hidden their real status.  This compiles
and runs each file on its own -> per-file pass / compile-fail / run-fail, with
the first error of each failure bucketed, so a buggy vector can't hide behind a
TU that won't compile.

Usage: tools/run_files.py <subtree-prefix> [limit]
"""
import json
import os
import re
import subprocess
import sys
import collections

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)
sys.path.insert(0, os.path.join(ROOT, "tools"))
import config as cfg  # noqa: E402

# -fcoroutines: gcc-10 gates the C++20 co_* keywords behind it (later gccs do not)
CIS = (cfg.require_cxx(cfg.CXX_LIBCIS, "libcis") +
       " -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ -Iinclude "
       "-Itest/std -Itest/std/support -O0 -w").split()
LINK = "build/groups/libcis/libsupport.a -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc".split()

# tools/test_overrides/ holds hand-adapted copies of transferred tests whose
# upstream form trips PROVEN gcc-10 frontend defects (evidence in the files'
# "libcis:" comments).  test/std is regenerated (gitignored), so the transfer
# would silently revert those adaptations; re-applying them here makes the
# gate self-healing regardless of transfer order.
OVR = os.path.join(ROOT, "tools", "test_overrides")
for dirp, _, files in os.walk(OVR):
    for fn in files:
        src = os.path.join(dirp, fn)
        dst = os.path.join(ROOT, "test/std", os.path.relpath(src, OVR))
        if os.path.exists(dst) and open(src).read() != open(dst).read():
            open(dst, "w").write(open(src).read())

# ---------------------------------------------------------------------------
# Per-test ADDITIONAL_COMPILE_FLAGS.  Most are libc++-only -D_LIBCPP_* knobs that
# are harmless no-ops here, but some localization tests need a real value for the
# lit substitution %{LOCALE_CONV_<LOCALE>_<MEMBER>} (e.g. the locale's monetary
# thousands separator).  We compute that exactly as libc++'s features.py does:
# the localeconv member rendered as a wide-string literal L"\uXXXX".
import locale as _locale

_CONV_MEMBERS = {
    "MON_THOUSANDS_SEP": "mon_thousands_sep",
    "THOUSANDS_SEP": "thousands_sep",
    "DECIMAL_POINT": "decimal_point",
}
# define-name (as features.py builds it) -> actual locale name
_LOCALE_BY_DEFINE = {}
for _loc in ("en_US.UTF-8", "fr_FR.UTF-8", "ru_RU.UTF-8", "zh_CN.UTF-8",
             "fr_CA.ISO8859-1", "cs_CZ.ISO8859-2", "ja_JP.UTF-8"):
    _LOCALE_BY_DEFINE[re.sub(r"[.-]", "_", _loc).upper()] = _loc


def _locale_conv_literal(define_name, member):
    loc = _LOCALE_BY_DEFINE.get(define_name)
    if loc is None:
        return None
    try:
        _locale.setlocale(_locale.LC_ALL, loc)
        v = _locale.localeconv()[_CONV_MEMBERS[member]]
    except Exception:
        return None
    finally:
        try:
            _locale.setlocale(_locale.LC_ALL, "C")
        except Exception:
            pass
    esc = "".join("\\u%04x" % ord(c) if (ord(c) < 0x20 or ord(c) > 0x7e) else c
                  for c in v)
    return 'L"%s"' % esc


_CONV_RE = re.compile(r"%\{LOCALE_CONV_([A-Z0-9_]+?)_(" +
                      "|".join(_CONV_MEMBERS) + r")\}")


def _extra_flags(src_path):
    """Parse ADDITIONAL_COMPILE_FLAGS from a test, expanding LOCALE_CONV substitutions.
    Returns (flags, ok); ok is False if a substitution could not be resolved."""
    flags, ok = [], True
    with open(src_path, errors="replace") as fh:
        for line in fh:
            s = line.strip()
            if s.startswith("int main") or s.startswith("#include"):
                break  # lit header is over once real code starts
            if "ADDITIONAL_COMPILE_FLAGS:" not in s:
                continue
            spec = s.split("ADDITIONAL_COMPILE_FLAGS:", 1)[1].strip()

            def sub(m):
                nonlocal ok
                lit = _locale_conv_literal(m.group(1), m.group(2))
                if lit is None:
                    ok = False
                    return m.group(0)
                return lit
            spec = _CONV_RE.sub(sub, spec)
            if "%{" in spec:
                continue  # unsupported substitution: drop this flag line
            flags += spec.split()
    return flags, ok


pre = sys.argv[1]
limit = int(sys.argv[2]) if len(sys.argv) > 2 else 10**9
man = json.load(open("test/std/manifest.json"))

# Excluded tests carry a written justification (gcc-10 compiler limits proven by
# the discriminator, target-impossible facilities).  They are NOT the work queue
# and NOT counted against the verdict: every test NOT excluded must pass.
excl = {k: v for k, v in json.load(open("tools/exclusions.json")).items()
        if not k.startswith("_")}
tests = [r for r in man["transferred"]
         if r["file"].startswith(pre) and r["kind"] == "run" and r.get("entry")
         and r["file"] not in excl][:limit]

buckets = collections.Counter()
cerr = collections.Counter()
rfail = []
cfail = []
for r in tests:
    src = os.path.abspath(os.path.join("test/std", r["file"]))
    # PID-unique temp paths: concurrent run_files invocations (parallel
    # subtree gates) must not clobber each other's driver/executable.
    drv = f"/tmp/rf_drv_{os.getpid()}.cpp"
    # Forward the process's real argc/argv to the test's main: lit runs tests as
    # real executables, so tests may legitimately read argv[0] (e.g. the procfs
    # copy_file test compares against argv[0]'s basename).  The manifest entry
    # bakes in "NS::main(0, nullptr)"; route the call through an overloaded shim
    # so it adapts to either standard main signature -- some tests declare
    # `int main(int, const char**)`, and char** -> const char** is not an
    # implicit conversion in C++.
    entry = r["entry"]
    if entry.endswith("(0, nullptr)"):
        fn = entry[: -len("(0, nullptr)")]
        entry = f"cis_call_main(&{fn}, argc, argv)"
    drv_src = (
        f'#include "{src}"\n'
        "template<class R> R cis_call_main(R(*f)(int, char**), int c, char** v)"
        "{ return f(c, v); }\n"
        "template<class R> R cis_call_main(R(*f)(int, const char**), int c, char** v)"
        "{ return f(c, const_cast<const char**>(v)); }\n"
        f"int main(int argc, char** argv){{ return {entry}; }}\n")
    open(drv, "w").write(drv_src)
    exe = f"/tmp/rf_exe_{os.getpid()}"
    xflags, _ = _extra_flags(src)
    try:
        p = subprocess.run(CIS + xflags + [drv] + LINK + ["-o", exe],
                           capture_output=True, text=True, timeout=180)
    except subprocess.TimeoutExpired:
        buckets["compile-fail"] += 1
        cerr["compile-timeout"] += 1
        cfail.append((r["file"], "compile-timeout (180s)"))
        continue
    if p.returncode != 0:
        buckets["compile-fail"] += 1
        m = re.search(r"error: (.+)", p.stderr)
        key = re.sub(r"'[^']*'", "'X'", m.group(1))[:62] if m else "?"
        cerr[key] += 1
        cfail.append((r["file"], m.group(1)[:80] if m else "?"))
        continue
    try:
        rc = subprocess.run([exe], capture_output=True, timeout=60).returncode
    except subprocess.TimeoutExpired:
        rc = -99
    if rc == 0:
        buckets["PASS"] += 1
    else:
        buckets["run-fail"] += 1
        rfail.append(r["file"])

# State is binary: a subtree is CLEAN iff nothing in it fails.  We don't count
# worms -- we list them, so each can be fixed or moved (justified) into the
# exclusion set.  No pass-count, no ratio, no error-class census.
for f in rfail:
    print(f"FAIL(run)     {f}")
for f, e in cfail:
    print(f"FAIL(compile) {f}\n              {e}")
print("CLEAN" if not (rfail or cfail) else "NOT CLEAN")
sys.exit(0 if not (rfail or cfail) else 1)
