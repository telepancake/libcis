// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char16_t_char8_t_unshift_1a451b85).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// XFAIL: availability-char8_t_support-missing

// <locale>

// template <> class codecvt<char16_t, char8_t, mbstate_t>

// result unshift(stateT& state,
//                externT* to, externT* to_end, externT*& to_next) const;

#include <cassert>
#include <locale>

namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char16_t_char8_t_unshift_1a451b85 { // libcis
int main(int, char**) {
  using F = std::codecvt<char16_t, char8_t, std::mbstate_t>;
  const F& f = std::use_facet<F>(std::locale::classic());
  std::mbstate_t mbs = {};
  char8_t to[3];
  char8_t* to_next = nullptr;
  assert(f.unshift(mbs, to, to + 3, to_next) == F::noconv);
  assert(to_next == to);
  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char16_t_char8_t_unshift_1a451b85 (libcis)

