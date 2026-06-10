// test/iomanip.cpp — unit tests for <iomanip>
#include <iomanip>
#include <sstream>
#include <string>
#include "test.h"

void test_iomanip_setw_setfill() {
    // setw(5) + setfill('*') + right-align integer 42 -> "***42"
    std::ostringstream oss;
    oss << std::setw(5) << std::setfill('*') << 42;
    CHECK(oss.str() == "***42");
}

void test_iomanip_setw_string() {
    std::ostringstream oss;
    oss << std::setfill('-') << std::setw(8) << "hi";
    CHECK(oss.str() == "------hi");
}

void test_iomanip_setprecision() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << 3.14159;
    // Should produce "3.14"
    std::string s = oss.str();
    CHECK(s == "3.14");
}

void test_iomanip_setbase_hex() {
    std::ostringstream oss;
    oss << std::setbase(16) << 255;
    CHECK(oss.str() == "ff");
}

void test_iomanip_setbase_oct() {
    std::ostringstream oss;
    oss << std::setbase(8) << 8;
    CHECK(oss.str() == "10");
}

void test_iomanip_setbase_dec() {
    std::ostringstream oss;
    oss << std::hex << 255;             // set to hex first
    oss.str("");
    oss << std::setbase(10) << 255;     // now back to dec
    CHECK(oss.str() == "255");
}

void test_iomanip_setiosflags_resetiosflags() {
    // setiosflags just ORs in bits; to set hex base properly, first clear basefield
    std::ostringstream oss;
    oss << std::resetiosflags(std::ios_base::basefield)
        << std::setiosflags(std::ios_base::hex | std::ios_base::showbase) << 255;
    std::string s = oss.str();
    // Should be "0xff"
    CHECK(s == "0xff");

    // reset hex base, go back to dec (no basefield bits => decimal)
    std::ostringstream oss2;
    oss2 << std::hex << std::resetiosflags(std::ios_base::basefield) << 10;
    // After resetiosflags(basefield), no basefield bits set => dec output
    CHECK(oss2.str() == "10");
}

void test_iomanip_quoted_output() {
    std::ostringstream oss;
    oss << std::quoted("hello world");
    CHECK(oss.str() == "\"hello world\"");
}

void test_iomanip_quoted_escape() {
    std::ostringstream oss;
    oss << std::quoted("say \"hi\"");
    CHECK(oss.str() == "\"say \\\"hi\\\"\"");
}

void test_iomanip_quoted_roundtrip() {
    std::string original = "hello world";
    std::stringstream ss;
    ss << std::quoted(original);
    std::string result;
    ss >> std::quoted(result);
    CHECK(result == original);
}

void test_iomanip_quoted_string_const() {
    std::string s = "foo bar";
    std::ostringstream oss;
    oss << std::quoted(s);
    CHECK(oss.str() == "\"foo bar\"");
}
