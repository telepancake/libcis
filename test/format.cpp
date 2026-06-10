// test/format.cpp — tests for <format>
#include <format>
#include <string>
#include <vector>
#include <iterator>
#include "test.h"

void test_format_basic() {
    // Integer
    CHECK(std::format("{}", 42)   == "42");
    CHECK(std::format("{}", -7)   == "-7");
    CHECK(std::format("{}", 0)    == "0");
    // String
    CHECK(std::format("{}", "hi") == "hi");
    // Bool
    CHECK(std::format("{}", true)  == "true");
    CHECK(std::format("{}", false) == "false");
}

void test_format_width_align() {
    // Right-align (default for numbers)
    CHECK(std::format("{:5}",  42) == "   42");
    CHECK(std::format("{:>5}", 42) == "   42");
    // Left-align
    CHECK(std::format("{:<5}", 42) == "42   ");
    // Center
    CHECK(std::format("{:^5}", 42) == " 42  ");
    // String defaults left
    CHECK(std::format("{:5}",  "hi") == "hi   ");
    CHECK(std::format("{:<5}", "hi") == "hi   ");
    CHECK(std::format("{:>5}", "hi") == "   hi");
}

void test_format_fill() {
    CHECK(std::format("{:*<5}", 42) == "42***");
    CHECK(std::format("{:*>5}", 42) == "***42");
    CHECK(std::format("{:*^5}", 42) == "*42**");
    CHECK(std::format("{:0>5}", 42) == "00042");
    CHECK(std::format("{:05}",  42) == "00042");
}

void test_format_hex() {
    CHECK(std::format("{:x}",  255)  == "ff");
    CHECK(std::format("{:X}",  255)  == "FF");
    CHECK(std::format("{:#x}", 255)  == "0xff");
    CHECK(std::format("{:#X}", 255)  == "0XFF");
    CHECK(std::format("{:#010x}", 255) == "0x000000ff");
}

void test_format_octal_binary() {
    CHECK(std::format("{:o}",  8)  == "10");
    CHECK(std::format("{:#o}", 8)  == "010");
    CHECK(std::format("{:b}",  5)  == "101");
    CHECK(std::format("{:#b}", 5)  == "0b101");
}

void test_format_float() {
    // Basic float
    auto s = std::format("{:.2f}", 3.14159);
    CHECK(s == "3.14");
    // default (g) format
    auto s2 = std::format("{}", 1.5);
    CHECK(s2 == "1.5");
    // explicit precision
    CHECK(std::format("{:.4f}", 1.0) == "1.0000");
    // scientific
    // snprintf "e" style: 1.000000e+00
    auto se = std::format("{:.2e}", 12345.6789);
    CHECK(se == "1.23e+04");
}

void test_format_string_precision() {
    // String precision truncates
    CHECK(std::format("{:.3}", "hello") == "hel");
    // Width + precision
    CHECK(std::format("{:8.3}", "hello") == "hel     ");
}

void test_format_positional() {
    CHECK(std::format("{1} {0}", "world", "hello") == "hello world");
    CHECK(std::format("{0}{1}{0}", "ab", "cd")      == "abcdab");
}

void test_format_to() {
    std::string result;
    std::format_to(std::back_inserter(result), "{} + {} = {}", 1, 2, 3);
    CHECK(result == "1 + 2 = 3");
}

void test_format_formatted_size() {
    size_t n = std::formatted_size("{}", 12345);
    CHECK(n == 5);
    size_t n2 = std::formatted_size("{:.2f}", 3.14159);
    CHECK(n2 == 4); // "3.14"
}

void test_format_sign() {
    CHECK(std::format("{:+}", 42)  == "+42");
    CHECK(std::format("{:+}", -42) == "-42");
    CHECK(std::format("{: }", 42)  == " 42");
    CHECK(std::format("{: }", -42) == "-42");
}

void test_format_char() {
    CHECK(std::format("{}", 'A') == "A");
    CHECK(std::format("{:c}", 65) == "A");
}

void test_format_escape() {
    CHECK(std::format("{{}}") == "{}");
    CHECK(std::format("{{ {0} }}", 42) == "{ 42 }");
}

void test_format_strings_by_value() {
    std::string s = "world";
    CHECK(std::format("hello {}", s) == "hello world");
    std::string_view sv = "view";
    CHECK(std::format("{}", sv) == "view");
}

void test_format_to_n() {
    char buf[10] = {};
    auto r = std::format_to_n(buf, 5, "{}", 123456789);
    buf[5] = '\0';
    CHECK(std::string_view(buf, 5) == "12345");
    CHECK(r.size == 9); // total digits
}
