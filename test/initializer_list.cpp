// Tests for <initializer_list>.
// Each test_ function is picked up by tools/gen_main.sh.
#include "test.h"
#include <initializer_list>

// ---------------------------------------------------------------------------
// Type checks (compile-time)
// ---------------------------------------------------------------------------
void test_initializer_list_types() {
    static_assert(__is_same(std::initializer_list<int>::value_type,      int));
    static_assert(__is_same(std::initializer_list<int>::reference,       const int&));
    static_assert(__is_same(std::initializer_list<int>::const_reference, const int&));
    static_assert(__is_same(std::initializer_list<int>::size_type,       std::size_t));
    static_assert(__is_same(std::initializer_list<int>::iterator,        const int*));
    static_assert(__is_same(std::initializer_list<int>::const_iterator,  const int*));
}

// ---------------------------------------------------------------------------
// Default construction — empty list
// ---------------------------------------------------------------------------
void test_initializer_list_default_ctor() {
    std::initializer_list<int> il;
    CHECK(il.size() == 0);
    CHECK(il.begin() == il.end());
    CHECK(il.begin() == nullptr);
}

// ---------------------------------------------------------------------------
// Braced-init-list construction via compiler magic
// ---------------------------------------------------------------------------
void test_initializer_list_braced_init() {
    std::initializer_list<int> il = {10, 20, 30};
    CHECK(il.size() == 3);
    CHECK(il.begin() != nullptr);
    CHECK(il.begin()[0] == 10);
    CHECK(il.begin()[1] == 20);
    CHECK(il.begin()[2] == 30);
    CHECK(il.end() == il.begin() + 3);
}

// ---------------------------------------------------------------------------
// begin() / end() member functions
// ---------------------------------------------------------------------------
void test_initializer_list_begin_end_members() {
    std::initializer_list<double> il = {1.0, 2.0, 3.0, 4.0};
    CHECK(il.size() == 4);

    const double* p = il.begin();
    const double* e = il.end();
    CHECK(e - p == 4);
    CHECK(p[0] == 1.0);
    CHECK(p[3] == 4.0);
}

// ---------------------------------------------------------------------------
// Free begin() / end() functions (ADL)
// ---------------------------------------------------------------------------
void test_initializer_list_free_begin_end() {
    std::initializer_list<int> il = {5, 6, 7};
    const int* b = std::begin(il);
    const int* e = std::end(il);
    CHECK(e - b == 3);
    CHECK(b[0] == 5);
    CHECK(b[2] == 7);
}

// ---------------------------------------------------------------------------
// Range-based for loop over an initializer_list
// ---------------------------------------------------------------------------
void test_initializer_list_range_for() {
    int sum = 0;
    for (int v : {1, 2, 3, 4, 5}) {
        sum += v;
    }
    CHECK(sum == 15);
}

// ---------------------------------------------------------------------------
// Range-based for over a named initializer_list variable
// ---------------------------------------------------------------------------
void test_initializer_list_range_for_named() {
    std::initializer_list<int> il = {10, 20, 30};
    int total = 0;
    for (int v : il) {
        total += v;
    }
    CHECK(total == 60);
}

// ---------------------------------------------------------------------------
// Works with non-trivial element type
// ---------------------------------------------------------------------------
struct Point { int x, y; };

void test_initializer_list_struct_elements() {
    std::initializer_list<Point> il = {{1, 2}, {3, 4}};
    CHECK(il.size() == 2);
    CHECK(il.begin()[0].x == 1);
    CHECK(il.begin()[0].y == 2);
    CHECK(il.begin()[1].x == 3);
    CHECK(il.begin()[1].y == 4);
}
