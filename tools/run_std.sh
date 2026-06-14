#!/usr/bin/env bash
# run_std.sh - compile+run the AST-TRANSFERRED libc++ suite (test/std/, produced
# by tools/transfer.py) against the three backends and report honest tallies.
#
# Each transferred *.pass.cpp no longer has main(): transfer.py renamed it to
# `void test_<slug>()` (slug recorded in manifest.json) and isolated the file's
# helpers in an anonymous namespace.  For run-tests we therefore compile the
# transformed source TOGETHER with a tiny generated driver that calls
# test_<slug>() -- proving the rename integrates with a libcis-style harness
# (one main per binary, helpers can't collide).  *.compile.pass.cpp are
# -fsyntax-only (no driver, no run).
#
#   libcxx     : clang++-20 -std=gnu++2c -stdlib=libc++
#   libstdcxx  : g++-14     -std=gnu++26
#   libcis     : g++-10     -std=gnu++20 -nostdinc++ -Iinclude (freestanding)
#
# All backends: -fno-exceptions -fno-rtti, -I test/std/support for the harness.
# Per-file ADDITIONAL_COMPILE_FLAGS recorded in manifest.json are applied.
#
# Usage: tools/run_std.sh [libcis|libcxx|libstdcxx|all] [N] [path-substr]
set -uo pipefail
cd "$(dirname "$0")/.."
ROOT=test/std
MAN=$ROOT/manifest.json
mkdir -p build/std

want="all"; limit=0; filter=""
for a in "$@"; do
  case "$a" in
    libcis|libcxx|libstdcxx|all) want="$a";;
    [0-9]*) limit="$a";;
    *) filter="$a";;
  esac
done

SUPPORT="-I$ROOT/support"
COMMON="-fno-exceptions -fno-rtti -O0 -w $SUPPORT"

# manifest -> rows: file<TAB>slug<TAB>kind<TAB>flags
mapfile -t LINES < <(python3 - "$MAN" "$filter" "$limit" <<'PY'
import json,sys
man=json.load(open(sys.argv[1])); filt=sys.argv[2]; lim=int(sys.argv[3])
rows=[r for r in man["transferred"] if filt in r["file"]]
if lim>0: rows=rows[:lim]
for r in rows:
    print(r["file"]+"\t"+r["slug"]+"\t"+r["kind"]+"\t"+" ".join(r.get("flags",[])))
PY
)

backend() {
  local name="$1" cc="$2" link="$3"
  local cpass=0 cfail=0 rpass=0 rfail=0
  : > build/std/$name.cfail.txt
  : > build/std/$name.rfail.txt
  for line in "${LINES[@]}"; do
    IFS=$'\t' read -r file slug kind flags <<<"$line"
    local src="$ROOT/$file"
    local tag; tag="$(echo "$file" | tr '/' '_')"
    local out="build/std/${tag}.${name}"
    if [ "$kind" = compile ]; then
      if eval "$cc $COMMON $flags -fsyntax-only \"$src\"" >/dev/null 2>&1; then
        cpass=$((cpass+1))
      else
        cfail=$((cfail+1)); echo "$file" >> build/std/$name.cfail.txt
      fi
    else
      # generate a driver that calls the renamed entry point.
      local drv="build/std/${tag}.driver.cpp"
      printf 'void test_%s();\nint main(){ test_%s(); return 0; }\n' "$slug" "$slug" > "$drv"
      if eval "$cc $COMMON $flags \"$src\" \"$drv\" $link -o \"$out\"" >/dev/null 2>&1; then
        cpass=$((cpass+1))
        if timeout 30 "./$out" >/dev/null 2>&1; then rpass=$((rpass+1));
        else rfail=$((rfail+1)); echo "$file" >> build/std/$name.rfail.txt; fi
      else
        cfail=$((cfail+1)); echo "$file" >> build/std/$name.cfail.txt
      fi
    fi
  done
  local ctot=$((cpass+cfail))
  printf "%-11s compile %4d/%-4d   run-pass %4d/%-4d   (run-fail %d)\n" \
    "$name" "$cpass" "$ctot" "$rpass" "$((rpass+rfail))" "$rfail"
}

echo "=== AST-transferred libc++ suite: ${#LINES[@]} tests  filter='${filter}' ==="
if [ "$want" = all ] || [ "$want" = libcxx ]; then
  backend libcxx "clang++-20 -std=gnu++2c -stdlib=libc++" "-pthread"
fi
if [ "$want" = all ] || [ "$want" = libstdcxx ]; then
  backend libstdcxx "g++-14 -std=gnu++26" "-pthread"
fi
if [ "$want" = all ] || [ "$want" = libcis ]; then
  backend libcis "g++-10 -std=gnu++20 -nostdinc++ -Iinclude" \
    "src/support.cpp -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc"
fi
true
