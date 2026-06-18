// regex_probe — measurement-only baseline for v8's decision on the <regex>
// walker port. Stage 10 in the cores-design migration order; the design
// explicitly does NOT propose a stage that lands a regex core, but the v8
// author needs a real number to argue from.
//
// The probe instantiates `basic_regex<char>` over a handful of compiled
// patterns (constant-folded inside the compiler if any are trivial; the
// traits dispatch lives inside the search hot loop), then exercises
// `regex_search` / `regex_match` / `regex_replace` against each. char-only:
// the wchar_t variant is a separate axis stage 10 would measure if needed.
#include <regex>
#include <string>

template <int N>
__attribute__((noinline)) void exercise(const std::string& pat,
                                        const std::string& subject) {
    std::regex re(pat);
    std::smatch m;
    bool matched = std::regex_search(subject, m, re);
    (void)matched;

    bool exact = std::regex_match(subject, re);
    (void)exact;

    std::string out = std::regex_replace(subject, re, std::string("X"));
    volatile auto sink = out.size() + (matched ? 1 : 0) + (exact ? 1 : 0) + N;
    (void)sink;
}

int main() {
    exercise<0>("a+b*c?",       "aaabbc");
    exercise<1>("[0-9]{2,4}",   "abc1234def");
    exercise<2>("(foo|bar)+",   "foobarfoo");
    exercise<3>("^.{3,}$",      "hello");
    return 0;
}
