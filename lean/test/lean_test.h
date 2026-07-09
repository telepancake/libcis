// Trap-on-failure check for the lean smoke tests (per CONVENTIONS.md: a test
// crashes at the failing line, so gdb shows exactly where; no printing, no
// pass counters, no fallbacks).
#pragma once

#define CHECK(expr) ((expr) ? (void)0 : __builtin_trap())
