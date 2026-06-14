// fmt.cpp - exercise fmtlib/fmt (header-only) so its code lands in .text.
//
// fmt is built with exceptions disabled (FMT_EXCEPTIONS 0) to match libcis's
// -fno-exceptions target; FMT_HEADER_ONLY pulls the implementation into this TU.
#define FMT_HEADER_ONLY 1
#define FMT_EXCEPTIONS 0
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <string>
#include <vector>

int main() {
    std::string s = fmt::format("{} {} {:.3f} {:>8} {:#x}", 1, "two", 3.14159, "r", 255);
    std::vector<int> v{1, 2, 3, 4};
    s += fmt::format(" {}", v);
    s += fmt::format(" {:d}/{:b}/{:o}", 42, 42, 42);
    fmt::print("{}\n", s);
    return s.empty() ? 1 : 0;
}
