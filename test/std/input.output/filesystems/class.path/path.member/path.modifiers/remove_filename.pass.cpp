// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_class_path_path_member_path_modifiers_remove_filename).
// main -> test_input_output_filesystems_class_path_path_member_path_modifiers_remove_filename; file-scope helpers isolated in anon namespace.
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

// path& remove_filename()

#include <filesystem>
#include <type_traits>
#include <cassert>

#include "test_iterators.h"
#include "count_new.h"
namespace fs = std::filesystem;

namespace libcis_ns_input_output_filesystems_class_path_path_member_path_modifiers_remove_filename { // libcis: isolate file-scope helpers
struct RemoveFilenameTestcase {
  const char* value;
  const char* expect;
};

const RemoveFilenameTestcase TestCases[] =
  {
      {"", ""}
    , {"/", "/"}
    , {"//", "//"}
    , {"///", "///"}
#ifdef _WIN32
    , {"\\", "\\"}
#else
    , {"\\", ""}
#endif
    , {".", ""}
    , {"..", ""}
    , {"/foo", "/"}
    , {"foo/bar", "foo/"}
    , {"foo/", "foo/"}
#ifdef _WIN32
    , {"//foo", "//foo"}
#else
    , {"//foo", "//"}
#endif
    , {"//foo/", "//foo/"}
    , {"//foo///", "//foo///"}
    , {"///foo", "///"}
    , {"///foo/", "///foo/"}
    , {"/foo/", "/foo/"}
    , {"/foo/.", "/foo/"}
    , {"/foo/..", "/foo/"}
    , {"/foo/////", "/foo/////"}
#ifdef _WIN32
    , {"/foo\\\\", "/foo\\\\"}
#else
    , {"/foo\\\\", "/"}
#endif
    , {"/foo//\\/", "/foo//\\/"}
    , {"///foo", "///"}
    , {"file.txt", ""}
    , {"bar/../baz/./file.txt", "bar/../baz/./"}
  };
} using namespace libcis_ns_input_output_filesystems_class_path_path_member_path_modifiers_remove_filename; // libcis


void test_input_output_filesystems_class_path_path_member_path_modifiers_remove_filename()
{
  using namespace fs;
  for (auto const & TC : TestCases) {
    path const p_orig(TC.value);
    path p(p_orig);
    assert(p == TC.value);
    path& Ref = (p.remove_filename());
    assert(p == TC.expect);
    assert(&Ref == &p);
    assert(!p.has_filename());
  }

  return;
}
