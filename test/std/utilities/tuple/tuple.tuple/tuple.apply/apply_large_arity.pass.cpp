// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_tuple_apply_apply_large_arity).
// main -> test_utilities_tuple_tuple_tuple_tuple_apply_apply_large_arity; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <tuple>

// template <class F, class T> constexpr decltype(auto) apply(F &&, T &&)

// Stress testing large arities with tuple and array.

#include <tuple>
#include <array>
#include <utility>
#include <cassert>

#include "test_macros.h"

////////////////////////////////////////////////////////////////////////////////
namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_apply_apply_large_arity { // libcis: isolate file-scope helpers
template <class T, std::size_t Dummy = 0>
struct always_imp
{
    typedef T type;
};

template <class T, std::size_t Dummy = 0>
using always_t = typename always_imp<T, Dummy>::type;

////////////////////////////////////////////////////////////////////////////////
template <class Tuple, class Idx>
struct make_function;

template <class Tp, std::size_t ...Idx>
struct make_function<Tp, std::integer_sequence<std::size_t, Idx...>>
{
    using type = bool (*)(always_t<Tp, Idx>...);
};

template <class Tp, std::size_t Size>
using make_function_t = typename make_function<Tp, std::make_index_sequence<Size>>::type;

////////////////////////////////////////////////////////////////////////////////
template <class Tp, class Idx>
struct make_tuple_imp;

////////////////////////////////////////////////////////////////////////////////
template <class Tp, std::size_t ...Idx>
struct make_tuple_imp<Tp, std::integer_sequence<std::size_t, Idx...>>
{
    using type = std::tuple<always_t<Tp, Idx>...>;
};

template <class Tp, std::size_t Size>
using make_tuple_t = typename make_tuple_imp<Tp, std::make_index_sequence<Size>>::type;

template <class ...Types>
bool test_apply_fn(Types...) { return true; }


template <std::size_t Size>
void test_all()
{

    using A = std::array<int, Size>;
    using ConstA = std::array<int const, Size>;

    using Tuple = make_tuple_t<int, Size>;
    using CTuple = make_tuple_t<const int, Size>;

    using ValFn  = make_function_t<int, Size>;
    ValFn val_fn = &test_apply_fn;

    using RefFn  = make_function_t<int &, Size>;
    RefFn ref_fn = &test_apply_fn;

    using CRefFn = make_function_t<int const &, Size>;
    CRefFn cref_fn = &test_apply_fn;

    using RRefFn = make_function_t<int &&, Size>;
    RRefFn rref_fn = &test_apply_fn;

    {
        A a{};
        assert(std::apply(val_fn, a));
        assert(std::apply(ref_fn, a));
        assert(std::apply(cref_fn, a));
        assert(std::apply(rref_fn, std::move(a)));
    }
    {
        ConstA a{};
        assert(std::apply(val_fn, a));
        assert(std::apply(cref_fn, a));
    }
    {
        Tuple a{};
        assert(std::apply(val_fn, a));
        assert(std::apply(ref_fn, a));
        assert(std::apply(cref_fn, a));
        assert(std::apply(rref_fn, std::move(a)));
    }
    {
        CTuple a{};
        assert(std::apply(val_fn, a));
        assert(std::apply(cref_fn, a));
    }

}


template <std::size_t Size>
void test_one()
{
    using A = std::array<int, Size>;
    using Tuple = make_tuple_t<int, Size>;

    using ValFn  = make_function_t<int, Size>;
    ValFn val_fn = &test_apply_fn;

    {
        A a{};
        assert(std::apply(val_fn, a));
    }
    {
        Tuple a{};
        assert(std::apply(val_fn, a));
    }
}
} using namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_apply_apply_large_arity; // libcis


void test_utilities_tuple_tuple_tuple_tuple_apply_apply_large_arity()
{
    // Instantiate with 1-5 arguments.
    test_all<1>();
    test_all<2>();
    test_all<3>();
    test_all<4>();
    test_all<5>();

    // Stress test with 256
    test_one<256>();

  return;
}
