// AST-transferred from libc++ by tools/transfer.py (slug=re_re_alg_re_alg_search_grep).
// main -> test_re_re_alg_re_alg_search_grep; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class BidirectionalIterator, class Allocator, class charT, class traits>
//     bool
//     regex_search(BidirectionalIterator first, BidirectionalIterator last,
//                  match_results<BidirectionalIterator, Allocator>& m,
//                  const basic_regex<charT, traits>& e,
//                  regex_constants::match_flag_type flags = regex_constants::match_default);

#include <regex>
#include <cstring>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

extern "C" void LLVMFuzzerTestOneInput(const char *data)
{
#ifndef TEST_HAS_NO_EXCEPTIONS
    std::size_t size = std::strlen(data);
    if (size > 0)
    {
        try
        {
            std::regex::flag_type flag = std::regex_constants::grep;
            std::string s((const char *)data, size);
            std::regex re(s, flag);
            TEST_IGNORE_NODISCARD std::regex_match(s, re);
        }
        catch (std::regex_error &) {}
    }
#else
    ((void)data);
#endif
}


namespace libcis_ns_re_re_alg_re_alg_search_grep { // libcis: isolate file-scope helpers
void fuzz_tests()  // patterns that the fuzzer has found
{
// Raw string literals are a C++11 feature.
#if TEST_STD_VER >= 11
    LLVMFuzzerTestOneInput(R"XX(Õ)_%()()((\8'_%()_%()_%()_%(()_%()_%()_%(.t;)()¥f()_%()(.)_%;)()!¥f(((()()XX");
#endif
}

i
} using namespace libcis_ns_re_re_alg_re_alg_search_grep; // libcis
ntvoid test_re_re_alg_re_alg_search_grep()    {
        std::cmatch m;
        const char s[] = "tournament";
        assert(std::regex_search(s, m, std::regex("tour\nto\ntournament",
                std::regex_constants::grep)));
        assert(m.size() == 1);
        assert(!m.prefix().matched);
        assert(m.prefix().first == s);
        assert(m.prefix().second == m[0].first);
        assert(!m.suffix().matched);
        assert(m.suffix().first == m[0].second);
        assert(m.suffix().second == s + std::char_traits<char>::length(s));
        assert(m.length(0) == 10);
        assert(m.position(0) == 0);
        assert(m.str(0) == "tournament");
    }
    {
        std::cmatch m;
        const char s[] = "ment";
        assert(std::regex_search(s, m, std::regex("tour\n\ntournament",
                std::regex_constants::grep)));
        assert(m.size() == 1);
        assert(!m.prefix().matched);
        assert(m.prefix().first == s);
        assert(m.prefix().second == m[0].first);
        assert(m.suffix().matched);
        assert(m.suffix().first == m[0].second);
        assert(m.suffix().second == s + std::char_traits<char>::length(s));
        assert(m.length(0) == 0);
        assert(m.position(0) == 0);
        assert(m.str(0) == "");
    }
    fuzz_tests();

  return 0;
}
