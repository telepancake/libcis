// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_optional_optional_object_optional_object_assign_nullopt_t_50965cf1).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// optional<T>& operator=(nullopt_t) noexcept;

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "archetypes.h"

namespace libcis_ns_utilities_optional_optional_object_optional_object_assign_nullopt_t_50965cf1 { // libcis
using std::optional;
using std::nullopt_t;
using std::nullopt;

TEST_CONSTEXPR_CXX20 bool test()
{
    enum class State { inactive, constructed, destroyed };
    State state = State::inactive;

    struct StateTracker {
      TEST_CONSTEXPR_CXX20 StateTracker(State& s)
      : state_(&s)
      {
        s = State::constructed;
      }
      TEST_CONSTEXPR_CXX20 ~StateTracker() { *state_ = State::destroyed; }

      State* state_;
    };
    {
        optional<int> opt;
        static_assert(noexcept(opt = nullopt) == true, "");
        opt = nullopt;
        assert(static_cast<bool>(opt) == false);
    }
    {
        optional<int> opt(3);
        opt = nullopt;
        assert(static_cast<bool>(opt) == false);
    }
    {
        optional<StateTracker> opt;
        opt = nullopt;
        assert(state == State::inactive);
        assert(static_cast<bool>(opt) == false);
    }
    {
        optional<StateTracker> opt(state);
        assert(state == State::constructed);
        opt = nullopt;
        assert(state == State::destroyed);
        assert(static_cast<bool>(opt) == false);
    }
    return true;
}


int main(int, char**)
{
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    test();
    using TT = TestTypes::TestType;
    TT::reset();
    {
        optional<TT> opt;
        static_assert(noexcept(opt = nullopt) == true, "");
        assert(TT::destroyed == 0);
        opt = nullopt;
        assert(TT::constructed == 0);
        assert(TT::alive == 0);
        assert(TT::destroyed == 0);
        assert(static_cast<bool>(opt) == false);
    }
    assert(TT::alive == 0);
    assert(TT::destroyed == 0);
    TT::reset();
    {
        optional<TT> opt(42);
        assert(TT::destroyed == 0);
        TT::reset_constructors();
        opt = nullopt;
        assert(TT::constructed == 0);
        assert(TT::alive == 0);
        assert(TT::destroyed == 1);
        assert(static_cast<bool>(opt) == false);
    }
    assert(TT::alive == 0);
    assert(TT::destroyed == 1);
    TT::reset();

  return 0;

    return 0;
}
} // libcis_ns_utilities_optional_optional_object_optional_object_assign_nullopt_t_50965cf1 (libcis)

