// Minimal test harness for libcis.
//
// A failing CHECK prints the failed expression and location, then traps so that
// running under gdb or valgrind yields a backtrace at the exact failing line.
// No progress is printed; a passing run is silent.
//
// Compile-time properties should be checked with static_assert instead.
//
// __builtin_printf / __builtin_trap are used so this header pulls in no system
// headers (which would otherwise clash with libcis's own namespace std).
#pragma once

#define CHECK(...)                                                             \
  do {                                                                         \
    if (!(__VA_ARGS__)) {                                                      \
      __builtin_printf("CHECK failed: %s\n  at %s:%d\n", #__VA_ARGS__,         \
                       __FILE__, __LINE__);                                    \
      __builtin_trap();                                                        \
    }                                                                          \
  } while (0)
