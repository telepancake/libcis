// lean <format> — the runtime formatting engine relocated out-of-line (declared
// extern-template in <format>, defined + explicitly instantiated in
// lean/src/kernels.cpp).  This test exercises the ENGINE through the full public
// surface, so that when it is reached only as an out-of-line reference (static
// kernels.o, or liblean.so) every formatting path still produces the exact bytes
// base produces.  No printing; every check traps on failure (lean_test.h).
//
// Coverage: {} / positional+indexed args / width+precision+fill+align /
// integer bases b,o,x,X (+ '#') / float presentations f,e,g,a (+ precision) /
// char,bool,string,pointer / nested {} width & precision args / format_to,
// format_to_n, formatted_size / a user-defined formatter (parse + format) /
// vformat + vformat_to / wchar_t formatting / a consteval-checked good format
// string / a mixed-type stress loop against expected strings.  All must pass at
// -O0 and -Os.
#include <format>
#include <string>
#include <cstring>
#include <iterator>
#include <vector>
#include "lean_test.h"

using namespace std;

// EQ: format(...) must equal the expected literal, byte for byte.
#define EQ(expected, ...) CHECK(std::format(__VA_ARGS__) == string(expected))
#define WEQ(expected, ...) CHECK(std::format(__VA_ARGS__) == wstring(expected))

// ===========================================================================
// Basic replacement, positional / indexed args, escapes
// ===========================================================================
void test_basic_and_positional() {
  EQ("hello", "hello");
  EQ("{}", "{{}}");
  EQ("a{b}c", "a{{b}}c");
  EQ("42", "{}", 42);
  EQ("1 2 3", "{} {} {}", 1, 2, 3);
  // explicit argument indexing, including reuse and reordering
  EQ("b a", "{1} {0}", string("a"), string("b"));
  EQ("x x x", "{0} {0} {0}", string("x"));
  EQ("2 1 2 1", "{1} {0} {1} {0}", 1, 2);
}

// ===========================================================================
// Width, precision, fill, align
// ===========================================================================
void test_width_precision_fill_align() {
  EQ("   42", "{:5}", 42);          // default right-align for numbers
  EQ("42   ", "{:<5}", 42);
  EQ(" 42  ", "{:^5}", 42);   // pad 3 => floor(3/2)=1 before, ceil=2 after
  EQ("42   ", "{:5}", string("42")); // default left-align for strings
  EQ("***42", "{:*>5}", 42);
  EQ("42***", "{:*<5}", 42);
  EQ("*42**", "{:*^5}", 42);
  EQ("+42", "{:+}", 42);
  EQ("-42", "{:+}", -42);
  EQ(" 42", "{: }", 42);
  EQ("007", "{:03}", 7);
  EQ("-007", "{:04}", -7);
  // string precision truncates
  EQ("abc", "{:.3}", string("abcdef"));
  EQ("abcde", "{:.10}", string("abcde"));
  EQ("  abc", "{:>5.3}", string("abcdefff"));
}

// ===========================================================================
// Integer bases, alternate form, sign
// ===========================================================================
void test_integer_bases() {
  EQ("101010", "{:b}", 42);
  EQ("0b101010", "{:#b}", 42);
  EQ("0B101010", "{:#B}", 42);
  EQ("52", "{:o}", 42);
  EQ("052", "{:#o}", 42);
  EQ("0", "{:o}", 0);
  EQ("2a", "{:x}", 42);
  EQ("2A", "{:X}", 42);
  EQ("0x2a", "{:#x}", 42);
  EQ("0X2A", "{:#X}", 42);
  EQ("0xff", "{:#x}", 255);
  EQ("0x00ff", "{:#06x}", 255);   // zero-pad counts the 0x prefix
  EQ("-0x2a", "{:#x}", -42);
  EQ("+42", "{:+d}", 42);
  EQ("101", "{:b}", 5);
  EQ("1111111111111111111111111111111111111111111111111111111111111111",
     "{:b}", (unsigned long long)-1);
}

// ===========================================================================
// Floating point: f, e, g, a and precision
// ===========================================================================
void test_floats() {
  EQ("3.140000", "{:f}", 3.14);
  EQ("3.14", "{:.2f}", 3.14159);
  EQ("3.1", "{:.1f}", 3.14159);
  EQ("0.00", "{:.2f}", 0.0);
  EQ("-2.50", "{:.2f}", -2.5);
  EQ("1.234500e+03", "{:e}", 1234.5);
  EQ("1.2e+03", "{:.1e}", 1234.5);
  EQ("1.5E+00", "{:.1E}", 1.5);
  EQ("100", "{:g}", 100.0);
  EQ("0.0001", "{:g}", 0.0001);
  EQ("1e-05", "{:g}", 0.00001);
  EQ("1.5", "{:g}", 1.5);
  // shortest round-trip (no type char)
  EQ("3.14", "{}", 3.14);
  EQ("0.5", "{}", 0.5);
  EQ("1", "{}", 1.0);
  EQ("-0", "{}", -0.0);
  EQ("inf", "{}", 1.0 / 0.0 * (1.0));   // +inf via runtime
  // hex float carries a 'p' exponent
  {
    string h = std::format("{:a}", 1.0);
    CHECK(h.find('p') != string::npos);
  }
  // width + precision + fill on a float
  EQ("**3.14", "{:*>6.2f}", 3.14159);
  EQ("+3.14", "{:+.2f}", 3.14159);
}

// ===========================================================================
// char, bool, string, pointer
// ===========================================================================
void test_char_bool_string_pointer() {
  EQ("Z", "{}", 'Z');
  EQ("  Z", "{:>3}", 'Z');
  EQ("90", "{:d}", 'Z');            // char as integer
  EQ("0x5a", "{:#x}", 'Z');
  EQ("true", "{}", true);
  EQ("false", "{}", false);
  EQ("1", "{:d}", true);
  EQ("hi there", "{}", "hi there");
  EQ("hi there", "{}", string("hi there"));
  EQ("view", "{}", string_view("view"));
  // pointer prints as 0x... hex
  {
    int x = 0;
    string p = std::format("{}", (void*)&x);
    CHECK(p.size() >= 3 && p[0] == '0' && p[1] == 'x');
    EQ("0x0", "{}", (void*)nullptr);
    EQ("0x0", "{}", nullptr);
  }
}

// ===========================================================================
// Nested {} for dynamic width and precision
// ===========================================================================
void test_nested_width_precision() {
  EQ("   42", "{:{}}", 42, 5);
  EQ("42   ", "{:<{}}", 42, 5);
  EQ("3.14", "{:.{}f}", 3.14159, 2);
  EQ("  3.142", "{:{}.{}f}", 3.14159, 7, 3);
  // indexed nested args
  EQ("  abc", "{0:>{1}.{2}}", string("abcdef"), 5, 3);
}

// ===========================================================================
// format_to / format_to_n / formatted_size
// ===========================================================================
void test_output_iterators() {
  // format_to into a char buffer (raw pointer)
  {
    char buf[32];
    auto end = std::format_to(buf, "{}-{}", 12, 34);
    *end = '\0';
    CHECK(std::strcmp(buf, "12-34") == 0);
  }
  // format_to into a back_inserter
  {
    string s;
    std::format_to(std::back_inserter(s), "{:03}!", 7);
    CHECK(s == "007!");
  }
  // format_to into a vector<char> (container inserter path)
  {
    vector<char> v;
    std::format_to(std::back_inserter(v), "{}{}", 'a', 'b');
    CHECK(v.size() == 2 && v[0] == 'a' && v[1] == 'b');
  }
  // format_to_n truncates and reports full size
  {
    char buf[8];
    auto r = std::format_to_n(buf, 4, "{}{}{}", 111, 222, 333);
    CHECK(r.size == 9);              // "111222333" is 9 chars
    CHECK((r.out - buf) == 4);       // only 4 written
    CHECK(std::strncmp(buf, "1112", 4) == 0);
  }
  // formatted_size returns the exact byte count without writing
  CHECK(std::formatted_size("{}-{}", 12, 345) == 6);
  CHECK(std::formatted_size("{:>10}", 1) == 10);
  CHECK(std::formatted_size("{:.2f}", 3.14159) == 4);
}

// ===========================================================================
// vformat / vformat_to (type-erased args directly)
// ===========================================================================
void test_vformat() {
  CHECK(std::vformat("{} {}", std::make_format_args(*new int(1), *new int(2)))
        == "1 2");
  {
    int a = 7, b = 8;
    CHECK(std::vformat("{1}/{0}", std::make_format_args(a, b)) == "8/7");
  }
  {
    string s;
    int a = 5;
    std::vformat_to(std::back_inserter(s), "x={}", std::make_format_args(a));
    CHECK(s == "x=5");
  }
  // wide vformat
  {
    int a = 3;
    CHECK(std::vformat(L"w{}", std::make_wformat_args(a)) == wstring(L"w3"));
  }
}

// ===========================================================================
// A user-defined formatter (parse + format) — the handle dispatch path
// ===========================================================================
struct Point { int x, y; };

template <>
struct std::formatter<Point, char> {
  bool bare = false;  // 'n' spec => no parentheses

  constexpr auto parse(std::basic_format_parse_context<char>& ctx)
      -> decltype(ctx.begin()) {
    auto it = ctx.begin();
    if (it != ctx.end() && *it == 'n') {
      bare = true;
      ++it;
    }
    // it now points at '}' (or end) — the terminating brace check is the
    // engine's responsibility.
    return it;
  }

  template <class FormatContext>
  auto format(const Point& p, FormatContext& ctx) const {
    return bare ? std::format_to(ctx.out(), "{}, {}", p.x, p.y)
                : std::format_to(ctx.out(), "({}, {})", p.x, p.y);
  }
};

void test_user_formatter() {
  EQ("(3, 4)", "{}", Point{3, 4});
  EQ("3, 4", "{:n}", Point{3, 4});
  EQ("pt=(1, -2)!", "pt={}!", Point{1, -2});
  // user formatter nested inside width-less mixed args
  EQ("(0, 0) and 5", "{} and {}", Point{0, 0}, 5);
}

// ===========================================================================
// wchar_t formatting
// ===========================================================================
void test_wchar() {
  WEQ(L"42", L"{}", 42);
  WEQ(L"  hi", L"{:>4}", L"hi");
  WEQ(L"0x2a", L"{:#x}", 42);
  WEQ(L"3.14", L"{:.2f}", 3.14159);
  WEQ(L"A", L"{}", L'A');
  WEQ(L"true", L"{}", true);
  WEQ(L"7/8", L"{}/{}", 7, 8);
  WEQ(L"  wide  ", L"{:^8}", L"wide");
}

// ===========================================================================
// Compile-time format-string checking (consteval path).  A GOOD format string
// must construct as a core constant expression; exercising basic_format_string's
// consteval constructor here proves the compile-time validator is intact and
// that a well-formed spec is accepted.  (A malformed spec would be a hard
// compile error — not something a passing test can catch at runtime.)
// ===========================================================================
void test_consteval_format_string() {
  constexpr std::format_string<int> good1{"{:#06x}"};
  // dynamic width: the {} in the spec consumes arg 1, which MUST be a standard
  // integer type — the consteval validator rejects anything else at compile time.
  constexpr std::wformat_string<int, int> good2{L"{:>{}}"};
  CHECK(good1.get() == string_view("{:#06x}"));
  CHECK(good2.get() == wstring_view(L"{:>{}}"));
  // and it actually formats
  EQ("0x002a", good1, 42);
}

// ===========================================================================
// Mixed-type stress loop against expected strings
// ===========================================================================
void test_stress() {
  // Deterministic mix; expected strings computed by hand / from base semantics.
  for (int i = 0; i < 500; ++i) {
    int n = i - 250;
    double d = n * 0.5;

    // integer round trips through several bases
    {
      string dec = std::format("{}", n);
      long back = std::strtol(dec.c_str(), nullptr, 10);
      CHECK(back == n);
    }
    {
      unsigned u = (unsigned)i;
      string hex = std::format("{:x}", u);
      unsigned back = (unsigned)std::strtoul(hex.c_str(), nullptr, 16);
      CHECK(back == u);
    }
    // width is honoured: length is at least the requested field width
    {
      string w = std::format("{:>8}", n);
      CHECK(w.size() >= 8);
      // trimming the leading fill recovers the plain form
      size_t pos = w.find_first_not_of(' ');
      CHECK(w.substr(pos) == std::format("{}", n));
    }
    // fixed float precision: exactly 3 fractional digits
    {
      string f = std::format("{:.3f}", d);
      size_t dot = f.find('.');
      CHECK(dot != string::npos);
      CHECK(f.size() - dot - 1 == 3);
    }
    // a compound line, all argument kinds at once
    {
      string line = std::format("[{:04d}|{:#x}|{:.2f}|{}|{}]",
                                i, i, d, (i & 1) != 0, Point{i, n});
      CHECK(line.front() == '[' && line.back() == ']');
      CHECK(line.find('|') != string::npos);
    }
  }

  // A couple of exact compound expectations to pin the byte layout.
  EQ("[0042|0x2a|+3.50|true|(a, Z)]",
     "[{:04d}|{:#x}|{:+.2f}|{}|({}, {})]", 42, 42, 3.5, true, 'a', 'Z');
  EQ("   -7 |   0b101 | ABC  ",
     "{:>5} | {:>#7b} | {:<5}", -7, 5, string("ABC"));
}

int main() {
  test_basic_and_positional();
  test_width_precision_fill_align();
  test_integer_bases();
  test_floats();
  test_char_bool_string_pointer();
  test_nested_width_precision();
  test_output_iterators();
  test_vformat();
  test_user_formatter();
  test_wchar();
  test_consteval_format_string();
  test_stress();
  return 0;
}
