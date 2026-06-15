# libcis code-size journal

Append a block here after each meaningful change (`bench/codesize.py`).
Numbers are .text bytes at -Os on the canonical g++-10 target; '+base' is
marginal .text over the empty-main baseline. Lower is better.

## baseline — 2026-06-15 05:29 UTC

- libcis commit: 0cdbece (pre-optimization baseline)
- compiler: g++-10 (Ubuntu 10.5.0-4ubuntu2.1) 10.5.0
- flags: -std=gnu++20 -nostdinc++ -Iinclude -fno-exceptions -fno-rtti -Os
- project pins:
  doctest (v2.5.2)
  fmt (11.2.0)
  json (v3.12.0)
  magic_enum (v0.9.8)
  tomlplusplus (v3.4.0)
  unordered_dense (v4.8.1)

```
libcis code size @ -Os   (CXX=g++-10)
project                .text     +base   .rodata   .data    .bss
----------------------------------------------------------------
baseline               80633                2689      16    5184
fmt                   132111    +51478      8097      24    5328
unordered_dense        85743     +5110      2721      16    5192
magic_enum             81343      +710      3201      16    5192
json                  123179    +42546     27777      16    5200
tomlplusplus          138599    +57966     12129      16    5296
doctest               163279    +82646     13921     128   10480
```
