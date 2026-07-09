// Foundation self-test: the lean block + type-ops kernel (bits/lean.h).
#include <bits/lean.h>
#include <string.h>
#include "lean_test.h"

using namespace std::detail;

void test_block_roundtrip() {
  void* p = lean_alloc(alignof(int), 10 * sizeof(int));
  CHECK(lean_used(p) == 0);
  CHECK(reinterpret_cast<unsigned long>(p) % alignof(int) == 0);
  CHECK(lean_capacity_bytes(p, alignof(int)) >= 10 * sizeof(int));
  int* v = static_cast<int*>(p);
  for (int i = 0; i < 10; ++i) v[i] = i * 3;
  lean_used(p) = 10;
  p = lean_realloc(p, alignof(int), 1000 * sizeof(int));
  CHECK(lean_used(p) == 10);
  CHECK(lean_capacity_bytes(p, alignof(int)) >= 1000 * sizeof(int));
  v = static_cast<int*>(p);
  for (int i = 0; i < 10; ++i) CHECK(v[i] == i * 3);
  lean_free(p, alignof(int));
}

void test_alignment_offset() {
  CHECK(lean_offset(1) == sizeof(size_t));
  CHECK(lean_offset(8) == 8);
  CHECK(lean_offset(16) == 16);
  void* p = lean_alloc(16, 64);
  CHECK(reinterpret_cast<unsigned long>(p) % 16 == 0);
  lean_free(p, 16);
}

struct Counted {
  static inline int live = 0;
  int v;
  explicit Counted(int x) : v(x) { ++live; }
  Counted(const Counted& o) : v(o.v) { ++live; }
  Counted(Counted&& o) noexcept : v(o.v) { o.v = -1; ++live; }
  ~Counted() { --live; }
};

void test_ops_trivial_vs_not() {
  constexpr const std::detail::lean_ops& ti = lean_ops_for<int>;
  CHECK(ti.size == sizeof(int));
  CHECK(ti.relocate == nullptr);
  CHECK(ti.copy == nullptr);
  CHECK(ti.destroy == nullptr);

  constexpr const std::detail::lean_ops& tc = lean_ops_for<Counted>;
  CHECK(tc.relocate != nullptr);
  CHECK(tc.relocate_backward != nullptr);
  CHECK(tc.copy != nullptr);
  CHECK(tc.destroy != nullptr);

  alignas(Counted) char a[4 * sizeof(Counted)];
  alignas(Counted) char b[4 * sizeof(Counted)];
  Counted* ca = reinterpret_cast<Counted*>(a);
  for (int i = 0; i < 4; ++i) ::new (static_cast<void*>(ca + i)) Counted(i);
  CHECK(Counted::live == 4);
  tc.relocate(b, a, 4);                       // move to fresh storage
  CHECK(Counted::live == 4);                  // sources destroyed, dests live
  Counted* cb = reinterpret_cast<Counted*>(b);
  for (int i = 0; i < 4; ++i) CHECK(cb[i].v == i);
  tc.destroy(cb + 3, 1);                      // open a raw slot at the end
  CHECK(Counted::live == 3);
  tc.relocate_backward(cb + 1, cb, 3);        // overlapping shift right into it
  CHECK(cb[1].v == 0 && cb[2].v == 1 && cb[3].v == 2);
  tc.destroy(cb + 1, 3);
  CHECK(Counted::live == 0);
}

struct MoveOnly {
  int v = 1;
  MoveOnly() = default;
  MoveOnly(MoveOnly&& o) noexcept : v(o.v) { o.v = 0; }  // non-trivial on purpose
  MoveOnly(const MoveOnly&) = delete;
  ~MoveOnly() {}
};

void test_move_only_has_null_copy() {
  // must COMPILE for move-only types (copy thunk not instantiated) and
  // report copy as unavailable.
  CHECK(lean_ops_for<MoveOnly>.copy == nullptr);
  CHECK(lean_ops_for<MoveOnly>.relocate != nullptr);
}

void test_grow_advice() {
  CHECK(lean_grow_bytes(0, 5) == 5);
  CHECK(lean_grow_bytes(16, 17) == 32);
  CHECK(lean_grow_bytes(16, 100) == 100);
}

int main() {
  test_block_roundtrip();
  test_alignment_offset();
  test_ops_trivial_vs_not();
  test_move_only_has_null_copy();
  test_grow_advice();
  return 0;
}
