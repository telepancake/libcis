// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_class_rec_dir_itr_range_concept_conformance).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: no-filesystem

// recursive_directory_iterator

#include <filesystem>

#include <concepts>
#include <ranges>
namespace fs = std::filesystem;

namespace libcis_ns_input_output_filesystems_class_rec_dir_itr_range_concept_conformance { // libcis: isolate file-scope helpers
static_assert(std::same_as<std::ranges::iterator_t<fs::recursive_directory_iterator>, fs::recursive_directory_iterator>);
static_assert(std::ranges::common_range<fs::recursive_directory_iterator>);
static_assert(std::ranges::input_range<fs::recursive_directory_iterator>);
static_assert(std::ranges::view<fs::recursive_directory_iterator>);
static_assert(!std::ranges::sized_range<fs::recursive_directory_iterator>);
static_assert(std::ranges::borrowed_range<fs::recursive_directory_iterator>);
static_assert(std::ranges::viewable_range<fs::recursive_directory_iterator>);

static_assert(std::same_as<std::ranges::iterator_t<fs::recursive_directory_iterator&>, fs::recursive_directory_iterator>);
static_assert(std::ranges::common_range<fs::recursive_directory_iterator&>);
static_assert(std::ranges::input_range<fs::recursive_directory_iterator&>);
static_assert(!std::ranges::view<fs::recursive_directory_iterator&>);
static_assert(!std::ranges::sized_range<fs::recursive_directory_iterator&>);
static_assert(std::ranges::borrowed_range<fs::recursive_directory_iterator&>);
static_assert(std::ranges::viewable_range<fs::recursive_directory_iterator&>);

static_assert(std::same_as<std::ranges::iterator_t<const fs::recursive_directory_iterator&>, fs::recursive_directory_iterator>);
static_assert(std::ranges::common_range<const fs::recursive_directory_iterator&>);
static_assert(std::ranges::input_range<const fs::recursive_directory_iterator&>);
static_assert(!std::ranges::view<const fs::recursive_directory_iterator&>);
static_assert(!std::ranges::sized_range<const fs::recursive_directory_iterator&>);
static_assert(std::ranges::borrowed_range<const fs::recursive_directory_iterator&>);
static_assert(std::ranges::viewable_range<const fs::recursive_directory_iterator&>);
} using namespace libcis_ns_input_output_filesystems_class_rec_dir_itr_range_concept_conformance; // libcis

