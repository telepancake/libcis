# libcis code-size journal

Numbers are .text bytes on the canonical g++-10 target. The bench default is
the **flags-only optimized baseline** (-Os +opt: -flto + gc-sections +
merge-constants + hidden-visibility + gold/icf=safe + no-unwind-tables); this
is what library changes are measured on top of. Run `codesize.py --no-opt`
for the plain -Os comparison. '+base' is marginal .text over empty-main.

## baseline — 2026-06-15 07:13 UTC

- libcis commit: 4bef43d (flags-only optimized; pre-library-optimization)
- compiler: g++-10 (Ubuntu 10.5.0-4ubuntu2.1) 10.5.0
- opt flags: -flto -fmerge-all-constants -ffunction-sections -fdata-sections
  -Wl,--gc-sections -fvisibility=hidden -fvisibility-inlines-hidden
  -fno-asynchronous-unwind-tables -fno-unwind-tables -fuse-ld=gold -Wl,--icf=safe
- project pins:
  doctest (v2.5.2)
  fmt (11.2.0)
  json (v3.12.0)
  magic_enum (v0.9.8)
  tomlplusplus (v3.4.0)
  unordered_dense (v4.8.1)

```
libcis code size @ -Os +opt(flags)   (CXX=g++-10)
project                .text     +base   .rodata   .data    .bss
----------------------------------------------------------------
baseline               59518                2248       8    4952
fmt                   104510    +44992      7816      16    4992
unordered_dense        63688     +4170      2280       8    4968
magic_enum             59788      +270      2360       8    4952
json                   97518    +38000     30528       8    4920
tomlplusplus          111399    +51881     11328       8    5048
doctest               128721    +69203     13344       8   10016
```

For reference, plain -Os (`--no-opt`) .text — the ~20-25% LTO et al. buy back:
```
  fmt              132111
  unordered_dense  85743
  magic_enum       81343
  json             123179
  tomlplusplus     138599
  doctest          163279
```
