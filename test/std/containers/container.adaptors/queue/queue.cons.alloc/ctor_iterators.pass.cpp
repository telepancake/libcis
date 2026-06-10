// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_queue_queue_cons_alloc_ctor_iterators).
// main -> test_containers_container_adaptors_queue_queue_cons_alloc_ctor_iterators; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <queue>

// template <class InputIterator, class Allocator>
// queue(InputIterator, InputIterator, const Allocator&);

#include <cassert>
#include <queue>

#include "test_allocator.h"

namespace libcis_ns_containers_container_adaptors_queue_queue_cons_alloc_ctor_iterators { // libcis: isolate file-scope helpers
using base_type = std::queue<int, std::deque<int, test_allocator<int>>>;

class GetAlloc : public base_type {
  test_allocator_statistics* stats;

public:
  explicit GetAlloc(test_allocator_statistics& stats_, const int* begin, const int* end)
      : base_type(begin, end, test_allocator<int>(&stats_)), stats(&stats_) {}
  void check() {
    assert(size() == 4);
    assert(stats->alloc_count > 0);
  }
};
} using namespace libcis_ns_containers_container_adaptors_queue_queue_cons_alloc_ctor_iterators; // libcis


void test_containers_container_adaptors_queue_queue_cons_alloc_ctor_iterators() {
  const int a[] = {4, 3, 2, 1};
  test_allocator_statistics stats{};
  GetAlloc queue(stats, a, a + 4);
  assert(queue.front() == 4);
  queue.pop();
  assert(queue.front() == 3);
  queue.pop();
  assert(queue.front() == 2);
  queue.pop();
  assert(queue.front() == 1);
  queue.pop();
  assert(queue.empty());

  return;
}
