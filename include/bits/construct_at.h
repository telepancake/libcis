#pragma once

// std::construct_at, split out of <memory> ([specialized.construct]) because
// <optional> needs it for constexpr engagement but cannot include <memory>:
// <memory> -> bits/memory_ranges.h -> <ranges> -> <optional> is a cycle.
// gcc-10 only blesses constexpr placement-new inside the function literally
// named std::construct_at, so every constexpr in-place construction in the
// library must funnel through this one definition.

#include <type_traits>
#include <utility>
#include <new>

namespace std {

template<class T, class... Args>
    requires requires { ::new(declval<void*>()) T(declval<Args>()...); }
constexpr T* construct_at(T* loc, Args&&... args) {
    return ::new(static_cast<void*>(loc)) T(std::forward<Args>(args)...);
}

} // namespace std
