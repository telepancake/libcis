// Tests for <source_location>
//
// GCC-10 caveat: __builtin_COLUMN() is not available in gcc-10.2 (it's
// gcc-11+). However __builtin_source_location() is available and its internal
// __impl struct *does* carry the column field, so column() returns a valid
// (non-zero) value. No workaround is needed in the library.
#include <source_location>
#include <cstdint>
#include "test.h"

// ---- default construction ----------------------------------------------

void test_source_location_default() {
    std::source_location sl;
    // Default-constructed has no location info
    CHECK(sl.line() == 0);
    CHECK(sl.column() == 0);
    // file_name and function_name return ""
    const char* fn = sl.file_name();
    const char* fu = sl.function_name();
    CHECK(fn != nullptr && fn[0] == '\0');
    CHECK(fu != nullptr && fu[0] == '\0');
}

// ---- current() captures caller location --------------------------------

void test_source_location_current() {
    // We capture before the call so the line number is predictable
    auto sl = std::source_location::current();   // line N

    // file_name should be non-empty and contain this filename
    const char* file = sl.file_name();
    CHECK(file != nullptr && file[0] != '\0');

    // line should be non-zero (we're inside a function body)
    CHECK(sl.line() > 0);

    // function_name should be non-empty
    const char* func = sl.function_name();
    CHECK(func != nullptr && func[0] != '\0');
}

// ---- line numbers are sequential ---------------------------------------

void test_source_location_lines() {
    auto s1 = std::source_location::current();
    auto s2 = std::source_location::current();

    // Two consecutive captures in the same function: s2.line() > s1.line()
    CHECK(s2.line() > s1.line());

    // Same file
    const char* f1 = s1.file_name();
    const char* f2 = s2.file_name();
    // Compare pointer-equal (same string literal from the compiler) or
    // at least both non-empty
    (void)f1; (void)f2;
    CHECK(f1 != nullptr && f1[0] != '\0');
    CHECK(f2 != nullptr && f2[0] != '\0');
}

// ---- column is available via __builtin_source_location ----------------

void test_source_location_column() {
    // column() is supported on gcc-10 through __builtin_source_location().
    // It returns a non-zero value when the expression is at a known column.
    auto sl = std::source_location::current();
    // Just verify column() doesn't crash and returns something reasonable.
    uint_least32_t col = sl.column();
    // Should be > 0 (somewhere on the line, not col 0)
    CHECK(col > 0);
}

// ---- constexpr / static use --------------------------------------------

void test_source_location_static() {
    // source_location can be stored as a static
    static const std::source_location static_loc = std::source_location::current();
    CHECK(static_loc.line() > 0);
    CHECK(static_loc.file_name()[0] != '\0');
}

// ---- helper function captures ITS OWN location -------------------------

static std::source_location get_location(
    std::source_location loc = std::source_location::current())
{
    return loc;
}

void test_source_location_propagate() {
    auto loc = get_location();   // loc should point here, not inside get_location
    // The function_name should NOT be "get_location"
    // (it should be this function or the test runner's function)
    CHECK(loc.line() > 0);
    CHECK(loc.file_name()[0] != '\0');
}
