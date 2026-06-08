// bits/identity.h — std::identity functor
// Internal header: shared by <iterator> and <functional>.
// Depends on nothing heavy (only <utility> for std::forward).
//
// Mechanically ported from:
//   libc++ include/__functional/identity.h
// Target: gcc-10.2, -std=gnu++20 -fno-exceptions -fno-rtti
#pragma once

#include <utility>

namespace std {

//===----------------------------------------------------------------------===//
// identity
// (from libc++ __functional/identity.h, C++20 path)
//===----------------------------------------------------------------------===//

struct identity {
    template<class T>
    [[nodiscard]] constexpr T&& operator()(T&& t) const noexcept {
        return forward<T>(t);
    }
    using is_transparent = void;
};

} // namespace std
