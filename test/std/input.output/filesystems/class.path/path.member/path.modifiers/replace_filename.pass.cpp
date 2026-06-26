// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_filesystems_class_path_path_member_path_modifiers_replace_filename_ae843029).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// UNSUPPORTED: availability-filesystem-missing

// <filesystem>

// class path

// path& replace_filename( const path& replacement );

#include <filesystem>
#include <cassert>
#include <string>
#include <type_traits>

#include "test_iterators.h"
#include "count_new.h"
namespace libcis_ns_input_output_filesystems_class_path_path_member_path_modifiers_replace_filename_ae843029 { // libcis
namespace fs = std::filesystem;

struct ReplaceFilenameTestcase {
  const char* value;
  const char* expect;
  const char* filename;
};

const ReplaceFilenameTestcase TestCases[] =
  {
      {"/foo", "/bar", "bar"}
    , {"/foo", "/", ""}
    , {"foo", "bar", "bar"}
    , {"/", "/bar", "bar"}
#ifdef _WIN32
    , {"\\", "\\bar", "bar"}
#else
    , {"\\", "bar", "bar"}
#endif
    , {"///", "///bar", "bar"}
#ifdef _WIN32
    , {"\\\\", "\\\\bar", "bar"}
    , {"\\/\\", "\\/\\bar", "bar"}
#else
    , {"\\\\", "bar", "bar"}
    , {"\\/\\", "\\/bar", "bar"}
#endif
    , {".", "bar", "bar"}
    , {"..", "bar", "bar"}
    , {"/foo\\baz/bong/", "/foo\\baz/bong/bar", "bar"}
    , {"/foo\\baz/bong", "/foo\\baz/bar", "bar"}
  };

int main(int, char**)
{
  using namespace fs;
  for (auto const & TC : TestCases) {
    path p(TC.value);
    assert(p == TC.value);
    path& Ref = p.replace_filename(TC.filename);
    assert(p == TC.expect);
    assert(&Ref == &p);
    // Tests Effects "as-if": remove_filename() append(filename)
    {
      path p2(TC.value);
      path replace(TC.filename);
      p2.remove_filename();
      p2 /= replace;
      assert(p == p2);
    }
  }

  return 0;

    return 0;
}
} // libcis_ns_input_output_filesystems_class_path_path_member_path_modifiers_replace_filename_ae843029 (libcis)

