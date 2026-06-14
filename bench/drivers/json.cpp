// json.cpp - exercise nlohmann/json (single header).
//
// JSON_NOEXCEPTION matches libcis's -fno-exceptions target (json then aborts
// instead of throwing). Builds an object, serializes, re-parses, and reads back
// so the (de)serialization and the std::map/std::vector machinery land in .text.
#define JSON_NOEXCEPTION 1
#include <nlohmann/json.hpp>

#include <cstdio>
#include <string>

using nlohmann::json;

int main() {
    json j;
    j["name"] = "libcis";
    j["nums"] = {1, 2, 3, 4};
    j["nested"] = {{"a", true}, {"b", 3.5}, {"c", nullptr}};
    for (int i = 0; i < 8; ++i)
        j["list"].push_back({{"i", i}, {"sq", i * i}});

    std::string s = j.dump(2);
    json k = json::parse(s);
    long sum = 0;
    for (auto& e : k["list"]) sum += e["sq"].get<long>();
    std::printf("%zu %ld %s\n", s.size(), sum, k["name"].get<std::string>().c_str());
    return 0;
}
