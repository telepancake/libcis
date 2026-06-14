// tomlplusplus.cpp - exercise marzer/tomlplusplus (single header).
//
// TOML_EXCEPTIONS 0 matches libcis's -fno-exceptions target (toml++ returns a
// parse_result you query instead of throwing). Parses a document and walks it so
// the parser, the TOML value tree, and their STL backing land in .text.
#define TOML_EXCEPTIONS 0
#include <toml++/toml.hpp>

#include <cstdio>
#include <string_view>

static constexpr std::string_view DOC = R"(
title = "libcis"
count = 3
ratio = 2.5
tags = ["a", "b", "c"]

[server]
host = "localhost"
port = 8080
enabled = true
)";

int main() {
    auto result = toml::parse(DOC);
    if (!result) {
        std::puts("parse error");
        return 1;
    }
    const toml::table& t = result.table();
    long acc = t["count"].value_or(0L);
    acc += t["server"]["port"].value_or(0L);
    if (auto arr = t["tags"].as_array()) acc += static_cast<long>(arr->size());
    std::printf("%ld %s\n", acc, t["title"].value_or("?"));
    return 0;
}
