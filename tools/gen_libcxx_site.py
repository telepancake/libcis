#!/usr/bin/env python3
"""Generate libc++'s two CMake-generated headers from their source templates,
so the transfer can parse against a plain llvm-project checkout with no prebuilt
libc++ (and thus no system / distro package).

  gen_libcxx_site.py <libcxx/include dir>

  __config_site      <- __config_site.in, applying the substitutions CMake's
                        configure_file() would for a default, hardening=none,
                        non-Apple build (the values below mirror libcxx's
                        CMakeLists defaults for that configuration).
  __assertion_handler <- vendor/llvm/default_assertion_handler.in (a COPYONLY in
                        libcxx/include/CMakeLists.txt).

Idempotent: rewrites both files each run.
"""
import os
import re
import shutil
import sys

# The site values CMake would substitute for a default Linux build.  Keys map to
# the substituted text; a key present with "" is a bare `#define VAR` (toggle on);
# any #cmakedefine VAR not listed here becomes `/* #undef VAR */` (toggle off).
SITE_VALUES = {
    "_LIBCPP_ABI_VERSION": "1",
    "_LIBCPP_ABI_NAMESPACE": "__1",
    "_LIBCPP_HARDENING_MODE_DEFAULT": "_LIBCPP_HARDENING_MODE_NONE",
    "_LIBCPP_PSTL_CPU_BACKEND_SERIAL": "",
    # platform toggles CMake turns ON for a default non-Apple/Linux build:
    "_LIBCPP_HAS_NO_VENDOR_AVAILABILITY_ANNOTATIONS": "",
    "_LIBCPP_HAS_THREAD_API_PTHREAD": "",
}


def generate(inc):
    out = []
    for line in open(os.path.join(inc, "__config_site.in")):
        m = re.match(r"#cmakedefine (\w+) @(\w+)@", line)        # valued
        m2 = re.match(r"#cmakedefine (\w+)\s*$", line)           # bare toggle
        if m:
            v = m.group(1)
            out.append(f"#define {v} {SITE_VALUES[v]}\n"
                       if v in SITE_VALUES else f"/* #undef {v} */\n")
        elif m2:
            v = m2.group(1)
            out.append(f"#define {v}\n"
                       if v in SITE_VALUES else f"/* #undef {v} */\n")
        else:
            # the @_LIBCPP_*_DEFINES@ expansion slots are empty by default
            out.append(re.sub(r"@\w+@", "", line))
    with open(os.path.join(inc, "__config_site"), "w") as fh:
        fh.write("".join(out))
    shutil.copyfile(
        os.path.join(inc, os.pardir, "vendor", "llvm", "default_assertion_handler.in"),
        os.path.join(inc, "__assertion_handler"))


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("usage: gen_libcxx_site.py <libcxx/include dir>")
    generate(sys.argv[1])
