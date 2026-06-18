// two_deque_probe — artificial code-size probe: two std::deque<T> flavors,
// one with a trivially-relocatable T1 (memcpy-eligible on every segment
// boundary) and one with a non-relocatable T2 (per-element move/destroy
// loop). Stage 4 introduces three `segmented_*` cores plus a
// `segment_map_ops` vocab, and the deltas claimed there only make sense if
// both flavors have a baseline that exercises every modifier under test.
//
// Exercised surface per flavor: push_back / pop_front / pop_back /
// insert(pos, n, x) (cross-segment shift), erase(pos), iterator traversal,
// copy/move ctor, destructor.
#include <deque>
#include <string>
#include <utility>

// T1 — trivially-relocatable: pure POD (no allocations, no padding bytes the
// compiler cares about), so the segment-aligned memcpy fast path stage 4
// targets is hot.
struct T1 {
    int a, b, c, d;
};

// T2 — NOT trivially relocatable: holds a std::string, so move/destroy
// happens element-by-element per segment.
struct T2 {
    std::string s;
    int n;
    T2() : s(""), n(0) {}
    explicit T2(int i) : s("x"), n(i) {}
};

template <class T>
__attribute__((noinline)) void exercise(const T& z) {
    std::deque<T> d;
    // Grow across multiple segments. Default deque_block_size for both T1
    // (16 bytes) and T2 (~40 bytes) is 4096/sizeof(T) → ≥100, so push 400
    // to guarantee multi-segment work.
    for (int i = 0; i < 400; ++i) d.push_back(z);

    // Front/back peel — segment_map's pop_back_segment / push_back_segment
    // territory in stage 4.
    for (int i = 0; i < 50; ++i) d.pop_front();
    for (int i = 0; i < 20; ++i) d.pop_back();

    // Cross-segment shift: insert n copies at the front-ish position.
    d.insert(d.begin() + 5, 30, z);

    // Erase one element near the front: rotate over a segment boundary.
    d.erase(d.begin() + 10);

    // Iterator traversal: exercise next_segment hops.
    long sum = 0;
    for (auto it = d.begin(); it != d.end(); ++it) {
        // Just touch each element so the loop is not elided.
        sum += static_cast<long>(reinterpret_cast<const char*>(&*it) != nullptr);
    }

    // Copy ctor + move ctor — segmented_copy territory.
    std::deque<T> e(d);
    std::deque<T> f(std::move(e));

    volatile auto sink = d.size() + f.size() + sum;
    (void)sink;
}

int main() {
    exercise<T1>(T1{1, 2, 3, 4});
    exercise<T2>(T2{42});
    return 0;
}
