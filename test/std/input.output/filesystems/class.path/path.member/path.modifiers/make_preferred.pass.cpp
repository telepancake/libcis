// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_class_path_path_member_path_modifiers_make_preferred).
// main -> test_input_output_filesystems_class_path_path_member_path_modifiers_make_preferred; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <filesystem>

// class path

// path& make_preferred()

#include <filesystem>
#include <cassert>
#include <string>
#include <type_traits>

#include "test_iterators.h"
#include "count_new.h"
namespace fs = std::filesystem;

namespace libcis_ns_input_output_filesystems_class_path_path_member_path_modifiers_make_preferred { // libcis: isolate file-scope helpers
struct MakePreferredTestcase {
  const char* value;
  const char* expected_posix;
  const char* expected_windows;
};

const MakePreferredTestcase TestCases[] =
  {
      {"", "", ""}
    , {"hello_world", "hello_world", "hello_world"}
    , {"/", "/", "\\"}
    , {"/foo/bar/baz/", "/foo/bar/baz/", "\\foo\\bar\\baz\\"}
    , {"\\", "\\", "\\"}
    , {"\\foo\\bar\\baz\\", "\\foo\\bar\\baz\\", "\\foo\\bar\\baz\\"}
    , {"\\foo\\/bar\\/baz\\", "\\foo\\/bar\\/baz\\", "\\foo\\\\bar\\\\baz\\"}
  };
} using namespace libcis_ns_input_output_filesystems_class_path_path_member_path_modifiers_make_preferred; // libcis


void test_input_output_filesystems_class_path_path_member_path_modifiers_make_preferred()
{
  // This operation is an identity operation on linux.
  // On windows, compare with preferred_win, if set.
  using namespace fs;
  for (auto const & TC : TestCases) {
    path p(TC.value);
    assert(p == TC.value);
    path& Ref = (p.make_preferred());
#ifdef _WIN32
    std::string s(TC.expected_windows);
#else
    std::string s(TC.expected_posix);
#endif
    assert(p.string() == s);
    assert(&Ref == &p);
  }

  return;
}
