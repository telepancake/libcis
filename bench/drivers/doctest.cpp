// doctest.cpp - exercise doctest (single header) test framework.
//
// NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS matches libcis's -fno-exceptions target
// while keeping the assertion machinery. IMPLEMENT_WITH_MAIN pulls in the full
// runner, registry, and reporters so the framework's code lands in .text.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest/doctest.h>

#include <string>
#include <vector>

TEST_CASE("arithmetic") {
    int a = 2, b = 3;
    CHECK(a + b == 5);
    CHECK(a * b == 6);
    REQUIRE(b > a);
}

TEST_CASE("containers") {
    std::vector<int> v{3, 1, 2};
    CHECK(v.size() == 3);
    std::string s = "doctest";
    CHECK(s.substr(0, 3) == "doc");
    SUBCASE("growth") {
        v.push_back(4);
        CHECK(v.back() == 4);
    }
}
