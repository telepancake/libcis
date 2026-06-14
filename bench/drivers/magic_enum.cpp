// magic_enum.cpp - exercise Neargye/magic_enum (header-only).
//
// Compile-time enum reflection: name<->value, iteration, flags. Forces the
// reflection templates and the std::string_view / std::array machinery they
// rest on into .text.
#include <magic_enum/magic_enum.hpp>

#include <cstdio>
#include <string_view>

enum class Color { red = 1, green = 2, blue = 4, yellow = 8 };

int main() {
    int hits = 0;
    for (auto c : magic_enum::enum_values<Color>())
        hits += static_cast<int>(magic_enum::enum_name(c).size());

    auto v = magic_enum::enum_cast<Color>("green");
    if (v.has_value()) hits += static_cast<int>(*v);

    hits += static_cast<int>(magic_enum::enum_count<Color>());
    std::printf("%d %.*s\n", hits,
                static_cast<int>(magic_enum::enum_name(Color::blue).size()),
                magic_enum::enum_name(Color::blue).data());
    return 0;
}
