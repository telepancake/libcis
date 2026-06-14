# bench/ — compiled code-size benchmark

Tracks how much machine code (`.text`) real, STL-heavy C++ projects pull in when
built against **libcis** at `-Os`. This is the size-regression dashboard for the
library: shrink the STL, watch these numbers drop.

## Projects (git submodules under `projects/`)

Header-only, no third-party dependencies, heavy STL users, and buildable on
libcis's target (`g++-10 -std=gnu++20 -fno-exceptions -fno-rtti`, freestanding):

| project | pinned | what it exercises |
|---|---|---|
| [fmtlib/fmt](https://github.com/fmtlib/fmt) | `11.2.0` | formatting: `<string>`, charconv, ranges |
| [martinus/unordered_dense](https://github.com/martinus/unordered_dense) | `v4.8.1` | dense hash map/set, hashing, `<vector>` |
| [Neargye/magic_enum](https://github.com/Neargye/magic_enum) | `v0.9.8` | enum reflection, `<string_view>`, `<array>` |
| [nlohmann/json](https://github.com/nlohmann/json) | `v3.12.0` | (de)serialization, `<map>`, `<vector>`, `<string>` |
| [marzer/tomlplusplus](https://github.com/marzer/tomlplusplus) | `v3.4.0` | parser + value tree, `<string_view>`, `<variant>` |
| [doctest/doctest](https://github.com/doctest/doctest) | `v2.5.2` | test runner, registry, reporters |

Each is built with exceptions/RTTI disabled via its own opt-out macro
(`JSON_NOEXCEPTION`, `TOML_EXCEPTIONS 0`, `FMT_EXCEPTIONS 0`,
`DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS`), set in the driver.

Pull them in with:

```sh
git submodule update --init --depth 1
```

## Running

```sh
bench/codesize.py            # table for every project
bench/codesize.py fmt        # just one
bench/codesize.py --json     # machine-readable
```

Each project's driver (`drivers/<name>.cpp`) is compiled + linked against libcis
the same way `tools/run_std.sh` links the conformance suite — freestanding,
`-nostdinc++ -Iinclude`, `support.cpp -nodefaultlibs ...` — but at `-Os`, then
**run** (a size for a binary that doesn't work is meaningless) and measured with
`size -A`. Exit status is 0 iff every selected project compiled, linked and ran,
so the script doubles as a test.

The compiler defaults to `g++-10` (the canonical target; see `CONVENTIONS.md`,
and `tools/run_std.sh` which hardcodes it). Override with `CXX=...` (the bench
also builds cleanly on g++-13). The default is pinned rather than probed because
`.text` differs between g++ majors, so comparable numbers need a fixed toolchain.

## Reading the table

```
project                .text     +base   .rodata   .data    .bss
baseline               80633                2689      16    5184
fmt                   132111    +51478      8097      24    5328
unordered_dense        85743     +5110      2721      16    5192
magic_enum             81343      +710      3201      16    5192
json                  123179    +42546     27777      16    5200
tomlplusplus          138599    +57966     12129      16    5296
doctest               163279    +82646     13921     128   10480
```

`.text` is the whole binary's code; `baseline` is an empty `int main(){}`, so its
`.text` is the fixed cost of `src/support.cpp` + the freestanding link. The
`+base` column is each project's **marginal** code over that baseline — the part
that actually reflects libcis's STL footprint. (Numbers above are illustrative;
they move with libcis and the pinned project versions.)
