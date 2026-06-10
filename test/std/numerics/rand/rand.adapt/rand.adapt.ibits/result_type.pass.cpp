// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_ibits_result_type).
// main -> test_numerics_rand_rand_adapt_rand_adapt_ibits_result_type; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class Engine, size_t w, class UIntType>
// class independent_bits_engine
// {
// public:
//     // types
//     typedef UIntType result_type;

#include <random>
#include <type_traits>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
template <class UIntType, UIntType Min, UIntType Max>
class rand1
{
public:
    // types
    typedef UIntType result_type;

private:
    result_type x_;

    static_assert(Min < Max, "rand1 invalid parameters");
public:

#if TEST_STD_VER < 11 && defined(_LIBCPP_VERSION)
    // Workaround for lack of constexpr in C++03
    static const result_type _Min = Min;
    static const result_type _Max = Max;
#endif

    static TEST_CONSTEXPR result_type min() {return Min;}
    static TEST_CONSTEXPR result_type max() {return Max;}

    explicit rand1(result_type sd = Min) : x_(sd)
    {
        if (x_ < Min)
            x_ = Min;
        if (x_ > Max)
            x_ = Max;
    }

    result_type operator()()
    {
        result_type r = x_;
        if (x_ < Max)
            ++x_;
        else
            x_ = Min;
        return r;
    }
};

void
test1()
{
    static_assert((std::is_same<
        std::independent_bits_engine<rand1<unsigned long, 0, 10>, 16, unsigned>::result_type,
        unsigned>::value), "");
}

void
test2()
{
    static_assert((std::is_same<
        std::independent_bits_engine<rand1<unsigned long, 0, 10>, 16, unsigned long long>::result_type,
        unsigned long long>::value), "");
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_adapt_rand_adapt_ibits_result_type()
{
    test1();
    test2();

  return;
}
