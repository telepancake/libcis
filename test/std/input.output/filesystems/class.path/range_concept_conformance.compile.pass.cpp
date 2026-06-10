// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_class_path_range_concept_conformance).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// path

#include <filesystem>

#include <concepts>
#include <ranges>
namespace fs = std::filesystem;

namespace libcis_ns_input_output_filesystems_class_path_range_concept_conformance { // libcis: isolate file-scope helpers
static_assert(std::same_as<std::ranges::iterator_t<fs::path>, fs::path::iterator>);
static_assert(std::ranges::common_range<fs::path>);
static_assert(std::ranges::bidirectional_range<fs::path>);
static_assert(!std::ranges::view<fs::path>);
static_assert(!std::ranges::random_access_range<fs::path>);
static_assert(!std::ranges::sized_range<fs::path>);
static_assert(!std::ranges::borrowed_range<fs::path>);
static_assert(std::ranges::viewable_range<fs::path>);

static_assert(std::same_as<std::ranges::iterator_t<fs::path const>, fs::path::const_iterator>);
static_assert(std::ranges::common_range<fs::path const>);
static_assert(std::ranges::bidirectional_range<fs::path const>);
static_assert(!std::ranges::view<fs::path const>);
static_assert(!std::ranges::random_access_range<fs::path const>);
static_assert(!std::ranges::sized_range<fs::path const>);
static_assert(!std::ranges::borrowed_range<fs::path const>);
static_assert(!std::ranges::viewable_range<fs::path const>);
} using namespace libcis_ns_input_output_filesystems_class_path_range_concept_conformance; // libcis

