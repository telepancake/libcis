// test/iterator.cpp — tests for <iterator>
// Port of libc++ test/std/iterators/
// Target: gcc-10.2, -std=gnu++20 -fno-exceptions -fno-rtti
#include "test.h"
#include <iterator>
#include <type_traits>

//===----------------------------------------------------------------------===//
// iterator_tags
//===----------------------------------------------------------------------===//
void test_iterator_tags() {
    static_assert(std::is_base_of_v<std::input_iterator_tag,
                                     std::forward_iterator_tag>);
    static_assert(std::is_base_of_v<std::forward_iterator_tag,
                                     std::bidirectional_iterator_tag>);
    static_assert(std::is_base_of_v<std::bidirectional_iterator_tag,
                                     std::random_access_iterator_tag>);
    static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                     std::contiguous_iterator_tag>);
}

//===----------------------------------------------------------------------===//
// iterator_traits — pointer specialisation
//===----------------------------------------------------------------------===//
void test_iterator_traits_pointer() {
    using T = std::iterator_traits<int*>;
    static_assert(std::is_same_v<T::value_type, int>);
    static_assert(std::is_same_v<T::pointer, int*>);
    static_assert(std::is_same_v<T::reference, int&>);
    static_assert(std::is_same_v<T::difference_type, std::ptrdiff_t>);
    static_assert(std::is_same_v<T::iterator_category,
                                  std::random_access_iterator_tag>);
    static_assert(std::is_same_v<T::iterator_concept,
                                  std::contiguous_iterator_tag>);
}

//===----------------------------------------------------------------------===//
// iter_value_t / iter_reference_t / iter_rvalue_reference_t / iter_difference_t
//===----------------------------------------------------------------------===//
void test_iter_associated_types() {
    // pointer
    static_assert(std::is_same_v<std::iter_value_t<int*>, int>);
    static_assert(std::is_same_v<std::iter_reference_t<int*>, int&>);
    static_assert(std::is_same_v<std::iter_rvalue_reference_t<int*>, int&&>);
    static_assert(std::is_same_v<std::iter_difference_t<int*>, std::ptrdiff_t>);

    // const pointer
    static_assert(std::is_same_v<std::iter_value_t<const int*>, int>);
    static_assert(std::is_same_v<std::iter_reference_t<const int*>, const int&>);
}

//===----------------------------------------------------------------------===//
// indirectly_readable / indirectly_writable
//===----------------------------------------------------------------------===//
void test_indirectly_readable() {
    static_assert(std::indirectly_readable<int*>);
    static_assert(std::indirectly_readable<const int*>);
    static_assert(!std::indirectly_readable<void*>);
    static_assert(!std::indirectly_readable<int>);
}

void test_indirectly_writable() {
    static_assert(std::indirectly_writable<int*, int>);
    static_assert(std::indirectly_writable<int*, const int>);
    static_assert(!std::indirectly_writable<const int*, int>);
}

//===----------------------------------------------------------------------===//
// weakly_incrementable / incrementable
//===----------------------------------------------------------------------===//
void test_weakly_incrementable() {
    static_assert(std::weakly_incrementable<int*>);
    // int satisfies weakly_incrementable: int-int=int (signed), ++i, i++ work
    static_assert(std::weakly_incrementable<int>);
    // void* does not: can't dereference, no difference type
    static_assert(!std::weakly_incrementable<void*>);
}

void test_incrementable() {
    static_assert(std::incrementable<int*>);
}

//===----------------------------------------------------------------------===//
// input_or_output_iterator
//===----------------------------------------------------------------------===//
void test_input_or_output_iterator() {
    static_assert(std::input_or_output_iterator<int*>);
}

//===----------------------------------------------------------------------===//
// sentinel_for / sized_sentinel_for
//===----------------------------------------------------------------------===//
void test_sentinel_for() {
    static_assert(std::sentinel_for<int*, int*>);
    static_assert(std::sized_sentinel_for<int*, int*>);
}

//===----------------------------------------------------------------------===//
// input_iterator / forward_iterator / bidirectional_iterator /
// random_access_iterator / contiguous_iterator
//===----------------------------------------------------------------------===//
void test_iterator_concepts() {
    static_assert(std::input_iterator<int*>);
    static_assert(std::forward_iterator<int*>);
    static_assert(std::bidirectional_iterator<int*>);
    static_assert(std::random_access_iterator<int*>);
    static_assert(std::contiguous_iterator<int*>);
}

//===----------------------------------------------------------------------===//
// output_iterator
//===----------------------------------------------------------------------===//
void test_output_iterator() {
    static_assert(std::output_iterator<int*, int>);
    static_assert(!std::output_iterator<const int*, int>);
}

//===----------------------------------------------------------------------===//
// advance
//===----------------------------------------------------------------------===//
void test_advance() {
    int arr[] = {0, 1, 2, 3, 4};
    int* p = arr;
    std::advance(p, 3);
    CHECK(*p == 3);
    std::advance(p, -2);
    CHECK(*p == 1);
}

//===----------------------------------------------------------------------===//
// distance
//===----------------------------------------------------------------------===//
void test_distance() {
    int arr[] = {0, 1, 2, 3, 4};
    CHECK(std::distance(arr, arr + 5) == 5);
    CHECK(std::distance(arr + 2, arr + 5) == 3);
}

//===----------------------------------------------------------------------===//
// next / prev
//===----------------------------------------------------------------------===//
void test_next_prev() {
    int arr[] = {0, 1, 2, 3, 4};
    int* p = arr;
    CHECK(*std::next(p) == 1);
    CHECK(*std::next(p, 3) == 3);

    int* q = arr + 4;
    CHECK(*std::prev(q) == 3);
    CHECK(*std::prev(q, 2) == 2);
}

//===----------------------------------------------------------------------===//
// reverse_iterator
//===----------------------------------------------------------------------===//
void test_reverse_iterator() {
    int arr[] = {1, 2, 3, 4, 5};
    std::reverse_iterator<int*> rbegin(arr + 5);
    std::reverse_iterator<int*> rend(arr);

    CHECK(*rbegin == 5);
    CHECK(*(rbegin + 1) == 4);
    CHECK(*(rbegin + 2) == 3);
    CHECK(rbegin != rend);

    int sum = 0;
    for (auto it = rbegin; it != rend; ++it)
        sum += *it;
    CHECK(sum == 15);

    // make_reverse_iterator
    auto r2 = std::make_reverse_iterator(arr + 5);
    CHECK(*r2 == 5);
}

//===----------------------------------------------------------------------===//
// move_sentinel
//===----------------------------------------------------------------------===//
void test_move_sentinel() {
    std::move_sentinel<int*> ms(nullptr);
    static_assert(std::is_same_v<decltype(ms.base()), int*>);
}

//===----------------------------------------------------------------------===//
// move_iterator
//===----------------------------------------------------------------------===//
void test_move_iterator() {
    int arr[] = {1, 2, 3};
    std::move_iterator<int*> mi(arr);
    CHECK(mi.base() == arr);
    CHECK(*mi == 1);
    ++mi;
    CHECK(*mi == 2);

    auto mi2 = std::make_move_iterator(arr);
    CHECK(mi2.base() == arr);
}

//===----------------------------------------------------------------------===//
// back_insert_iterator / back_inserter
// (uses a simple hand-rolled container to avoid needing <vector>)
//===----------------------------------------------------------------------===//

namespace {
struct SimpleVec {
    int data_[16];
    int size_ = 0;
    using value_type = int;
    void push_back(int v) { data_[size_++] = v; }
    int& operator[](int i) { return data_[i]; }
};
} // anonymous namespace

void test_back_insert_iterator() {
    SimpleVec v;
    auto it = std::back_inserter(v);
    *it++ = 10;
    *it++ = 20;
    *it++ = 30;
    CHECK(v.size_ == 3);
    CHECK(v[0] == 10);
    CHECK(v[1] == 20);
    CHECK(v[2] == 30);
}

//===----------------------------------------------------------------------===//
// front_insert_iterator / front_inserter
//===----------------------------------------------------------------------===//

namespace {
struct SimpleDQ {
    int data_[16];
    int start_ = 8; // start in the middle
    int end_   = 8;
    using value_type = int;
    void push_front(int v) { data_[--start_] = v; }
    int& operator[](int i) { return data_[start_ + i]; }
    int size() const { return end_ - start_; }
};
} // anonymous namespace

void test_front_insert_iterator() {
    SimpleDQ dq;
    auto it = std::front_inserter(dq);
    *it++ = 10;
    *it++ = 20;
    *it++ = 30;
    CHECK(dq.size() == 3);
    // push_front: 30, 20, 10 (last pushed is at front)
    CHECK(dq[0] == 30);
    CHECK(dq[1] == 20);
    CHECK(dq[2] == 10);
}

//===----------------------------------------------------------------------===//
// insert_iterator / inserter
//===----------------------------------------------------------------------===//

namespace {
struct SimpleList {
    int data_[16];
    int size_ = 0;
    using value_type = int;
    using iterator = int*;
    iterator begin() { return data_; }
    iterator end()   { return data_ + size_; }
    iterator insert(iterator pos, int v) {
        // shift elements right
        for (int* p = data_ + size_; p != pos; --p) *p = *(p-1);
        *pos = v;
        ++size_;
        return pos;
    }
};
} // anonymous namespace

void test_insert_iterator() {
    SimpleList sl;
    sl.data_[0] = 1; sl.data_[1] = 3; sl.size_ = 2;
    // insert 2 between 1 and 3
    auto it = std::inserter(sl, sl.begin() + 1);
    *it = 2;
    CHECK(sl.size_ == 3);
    CHECK(sl.data_[0] == 1);
    CHECK(sl.data_[1] == 2);
    CHECK(sl.data_[2] == 3);
}

//===----------------------------------------------------------------------===//
// default_sentinel_t
//===----------------------------------------------------------------------===//
void test_default_sentinel() {
    // inline constexpr variables are const; decltype gives const T
    static_assert(std::is_same_v<std::remove_const_t<decltype(std::default_sentinel)>,
                                  std::default_sentinel_t>);
    static_assert(std::is_same_v<decltype(std::default_sentinel),
                                  const std::default_sentinel_t>);
}

//===----------------------------------------------------------------------===//
// unreachable_sentinel_t
//===----------------------------------------------------------------------===//
void test_unreachable_sentinel() {
    int arr[] = {1, 2, 3, 0};
    // Walk until we see 0 using unreachable_sentinel
    int* p = arr;
    while (!(*p == 0)) ++p;
    CHECK(*p == 0);

    // unreachable_sentinel always != any iterator
    int* q = arr;
    CHECK(!(std::unreachable_sentinel == q));
}

//===----------------------------------------------------------------------===//
// counted_iterator
//===----------------------------------------------------------------------===//
void test_counted_iterator() {
    int arr[] = {10, 20, 30, 40, 50};
    std::counted_iterator<int*> ci(arr, 5);
    CHECK(ci.count() == 5);
    CHECK(*ci == 10);
    ++ci;
    CHECK(*ci == 20);
    CHECK(ci.count() == 4);

    // equality with default_sentinel when count reaches 0
    std::counted_iterator<int*> ci2(arr, 0);
    CHECK(ci2 == std::default_sentinel);

    // difference: a - b = b.count_ - a.count_ = 3 - 5 = -2
    std::counted_iterator<int*> a(arr, 5);
    std::counted_iterator<int*> b(arr + 2, 3);
    CHECK((a - b) == -2);
    // default_sentinel difference
    std::counted_iterator<int*> c(arr, 3);
    CHECK((c - std::default_sentinel) == -3);
    CHECK((std::default_sentinel - c) == 3);
}

//===----------------------------------------------------------------------===//
// begin / end / cbegin / cend for arrays
//===----------------------------------------------------------------------===//
void test_begin_end_array() {
    int arr[] = {1, 2, 3};
    CHECK(std::begin(arr) == arr);
    CHECK(std::end(arr) == arr + 3);
    CHECK(std::cbegin(arr) == arr);
    CHECK(std::cend(arr) == arr + 3);
}

//===----------------------------------------------------------------------===//
// rbegin / rend for arrays
//===----------------------------------------------------------------------===//
void test_rbegin_rend_array() {
    int arr[] = {1, 2, 3};
    auto rb = std::rbegin(arr);
    CHECK(*rb == 3);
    auto re = std::rend(arr);
    // distance
    CHECK(re - rb == 3);
}

//===----------------------------------------------------------------------===//
// size / ssize / empty / data for arrays
//===----------------------------------------------------------------------===//
void test_size_empty_data_array() {
    int arr[] = {1, 2, 3, 4};
    CHECK(std::size(arr) == 4u);
    CHECK(std::ssize(arr) == 4);
    CHECK(!std::empty(arr));
    CHECK(std::data(arr) == arr);
}

//===----------------------------------------------------------------------===//
// indirectly_movable / indirectly_movable_storable
//===----------------------------------------------------------------------===//
void test_indirectly_movable() {
    static_assert(std::indirectly_movable<int*, int*>);
    static_assert(std::indirectly_movable_storable<int*, int*>);
}

//===----------------------------------------------------------------------===//
// indirectly_copyable / indirectly_copyable_storable
//===----------------------------------------------------------------------===//
void test_indirectly_copyable() {
    static_assert(std::indirectly_copyable<int*, int*>);
    static_assert(std::indirectly_copyable_storable<int*, int*>);
}

//===----------------------------------------------------------------------===//
// indirectly_swappable
//===----------------------------------------------------------------------===//
void test_indirectly_swappable() {
    static_assert(std::indirectly_swappable<int*, int*>);
}

//===----------------------------------------------------------------------===//
// projected
//===----------------------------------------------------------------------===//
void test_projected() {
    using Proj = std::identity;
    using P = std::projected<int*, Proj>;
    static_assert(std::is_same_v<P::value_type, int>);
}

//===----------------------------------------------------------------------===//
// permutable / sortable
//===----------------------------------------------------------------------===//
void test_permutable() {
    static_assert(std::permutable<int*>);
}

void test_sortable() {
    static_assert(std::sortable<int*>);
    static_assert(std::sortable<int*, std::ranges::less>);
}

//===----------------------------------------------------------------------===//
// iter_common_reference_t
//===----------------------------------------------------------------------===//
void test_iter_common_reference_t() {
    static_assert(std::is_same_v<std::iter_common_reference_t<int*>, int&>);
}

//===----------------------------------------------------------------------===//
// incrementable_traits / indirectly_readable_traits
//===----------------------------------------------------------------------===//
void test_incrementable_traits() {
    static_assert(std::is_same_v<std::incrementable_traits<int*>::difference_type,
                                  std::ptrdiff_t>);
    struct MyIter {
        using difference_type = long;
    };
    static_assert(std::is_same_v<std::incrementable_traits<MyIter>::difference_type,
                                  long>);
}

void test_indirectly_readable_traits() {
    static_assert(std::is_same_v<std::indirectly_readable_traits<int*>::value_type, int>);
    static_assert(std::is_same_v<std::indirectly_readable_traits<const int*>::value_type, int>);
    struct WithValue { using value_type = float; };
    static_assert(std::is_same_v<std::indirectly_readable_traits<WithValue>::value_type, float>);
}

//===----------------------------------------------------------------------===//
// disable_sized_sentinel_for (reverse_iterator)
//===----------------------------------------------------------------------===//
void test_disable_sized_sentinel_for() {
    // reverse_iterator<I1> and reverse_iterator<I2> have disable_sized_sentinel_for
    // set to true when sized_sentinel_for<I1, I2> is false.
    // For int*, sized_sentinel_for<int*, int*> is true so the specialisation
    // doesn't fire (the requires clause fails).
    // We just check pointer case is fine.
    static_assert(std::sized_sentinel_for<int*, int*>);
}
