// Minimal test harness for libcis.
//
// A failing CHECK prints the failed expression and location, then traps so that
// running under gdb or valgrind yields a backtrace at the exact failing line.
// A successful CHECK increments libcis_checks_passed; the auto-generated main
// prints the number of test functions run and CHECKs passed at the very end.
// (Compile-time properties are checked with static_assert and are validated by
// the build itself.)
//
// __builtin_printf / __builtin_trap are used so this header pulls in no system
// headers (which would otherwise clash with libcis's own namespace std).
#pragma once

// Defined once in the auto-generated main (so test files that use only
// static_assert and never include this header still link standalone).
extern unsigned long long libcis_checks_passed;

#define CHECK(...)                                                             \
  do {                                                                         \
    if (!(__VA_ARGS__)) {                                                       \
      __builtin_printf("CHECK failed: %s\n  at %s:%d\n", #__VA_ARGS__,          \
                       __FILE__, __LINE__);                                     \
      __builtin_trap();                                                         \
    } else {                                                                    \
      ++libcis_checks_passed;                                                   \
    }                                                                           \
  } while (0)
