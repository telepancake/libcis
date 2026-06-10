// test/sstream.cpp — unit tests for <sstream>
#include <sstream>
#include <string>
#include "test.h"

void test_sstream_ostringstream() {
    // ints and strings -> str()
    std::ostringstream oss;
    oss << 42 << " " << 100;
    CHECK(oss.str() == "42 100");

    std::ostringstream oss2;
    oss2 << "hello" << " " << "world";
    CHECK(oss2.str() == "hello world");

    std::ostringstream oss3;
    oss3 << -7 << " " << 0 << " " << 999;
    CHECK(oss3.str() == "-7 0 999");
}

void test_sstream_istringstream() {
    // parse ints
    std::istringstream iss("10 20 30");
    int a = 0, b = 0, c = 0;
    iss >> a >> b >> c;
    CHECK(a == 10);
    CHECK(b == 20);
    CHECK(c == 30);

    // parse words
    std::istringstream iss2("foo bar baz");
    std::string w1, w2, w3;
    iss2 >> w1 >> w2 >> w3;
    CHECK(w1 == "foo");
    CHECK(w2 == "bar");
    CHECK(w3 == "baz");
}

void test_sstream_stringstream_roundtrip() {
    // write then read back
    std::stringstream ss;
    ss << 123 << " " << 456;
    int x = 0, y = 0;
    ss >> x >> y;
    CHECK(x == 123);
    CHECK(y == 456);
}

void test_sstream_str_reset() {
    // str(set) reset
    std::ostringstream oss;
    oss << "first";
    CHECK(oss.str() == "first");

    oss.str("reset");
    // After str(set), the put pointer is at beginning, so new output appends from end of "reset"
    // Actually str(set) resets the buffer — let's just check str()
    CHECK(oss.str() == "reset");

    // Now write more
    oss.str("");
    oss << "new";
    CHECK(oss.str() == "new");
}

void test_sstream_view() {
    std::ostringstream oss;
    oss << "hello";
    std::string_view sv = oss.view();
    CHECK(sv == "hello");
}

void test_sstream_istringstream_with_string() {
    std::istringstream iss(std::string("42"));
    int v = 0;
    iss >> v;
    CHECK(v == 42);
    CHECK(iss.eof() || !iss.fail());
}

void test_sstream_move() {
    std::ostringstream oss;
    oss << "moved";
    std::ostringstream oss2(std::move(oss));
    CHECK(oss2.str() == "moved");
}
