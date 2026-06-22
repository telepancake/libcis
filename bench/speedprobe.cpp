// speedprobe.cpp — per-method SPEED at -Os, one realistic workload per method.
//
// Companion to callsites.cpp (which measures per-method code SIZE). Built twice
// by the harness — libcis and the non-type-erased reference — at the SAME -Os
// flags the size measurement uses, so it's the speed of the actually-shipped
// code, not a -O2 fantasy. Each workload isolates one method, runs it over
// realistic data, self-times with CLOCK_MONOTONIC, and prints:
//
//     SPEED <op> <ns_per_op>
//
// keyed to the same op names as the size table so the two axes line up per
// method. Two positives (smaller AND faster) = clear keep; two negatives = clear
// drop; mixed = a trade for a human to weigh. The harness never auto-decides.

#include <ctime>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

static inline double now_ns() {
    timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1e9 + (double)ts.tv_nsec;
}
template <class T> static inline void sink(const T& x) {
    asm volatile("" : : "r,m"(x) : "memory");
}

struct H { char pad[7]; std::string s; H() : s("x") {} H(int i) : s(i & 1 ? "ab" : "abcd") {}
           bool operator<(const H& o) const { return s < o.s; } };

// operator[] in the structure-of-arrays / parallel-array idiom (index loop).
static double sp_index() {
    const int N = 4096; std::vector<int> a(N), b(N), c(N);
    for (int i = 0; i < N; ++i) { a[i] = i; b[i] = 2 * i; c[i] = i ^ 5; }
    const int R = 30000; long long acc = 0;
    double t0 = now_ns();
    for (int r = 0; r < R; ++r) for (int i = 0; i < N; ++i) acc += a[i] + b[i] + c[i];
    double t = now_ns() - t0; sink(acc);
    return t / ((double)R * N * 3);
}
// end() as the loop bound in an iterator traversal.
static double sp_end() {
    const int N = 4096; std::vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = i;
    const int R = 30000; long long acc = 0;
    double t0 = now_ns();
    for (int r = 0; r < R; ++r) for (auto it = v.begin(); it != v.end(); ++it) acc += *it;
    double t = now_ns() - t0; sink(acc);
    return t / ((double)R * N);
}
static double sp_push_int() {
    const int N = 2048, R = 8000; double best = 1e30;
    for (int rep = 0; rep < 3; ++rep) {
        double t0 = now_ns();
        for (int r = 0; r < R; ++r) { std::vector<int> v; for (int i = 0; i < N; ++i) v.push_back(i); sink(v.data()); }
        double t = (now_ns() - t0) / ((double)R * N); if (t < best) best = t;
    }
    return best;
}
static double sp_push_H() {
    const int N = 512, R = 4000; double best = 1e30;
    for (int rep = 0; rep < 3; ++rep) {
        double t0 = now_ns();
        for (int r = 0; r < R; ++r) { std::vector<H> v; for (int i = 0; i < N; ++i) v.push_back(H(i)); sink(v.data()); }
        double t = (now_ns() - t0) / ((double)R * N); if (t < best) best = t;
    }
    return best;
}
static double sp_insert_H() {
    const int N = 256, R = 4000; double best = 1e30;
    for (int rep = 0; rep < 3; ++rep) {
        double t0 = now_ns();
        for (int r = 0; r < R; ++r) { std::vector<H> v; for (int i = 0; i < N; ++i) v.insert(v.begin(), H(i)); sink(v.data()); }
        double t = (now_ns() - t0) / ((double)R * N); if (t < best) best = t;
    }
    return best;
}
static double sp_erase_H() {
    const int N = 256, R = 4000; double best = 1e30;
    for (int rep = 0; rep < 3; ++rep) {
        std::vector<H> base; for (int i = 0; i < N; ++i) base.push_back(H(i));
        double t0 = now_ns();
        for (int r = 0; r < R; ++r) { std::vector<H> v(base); while (!v.empty()) v.erase(v.begin()); sink(v.data()); }
        double t = (now_ns() - t0) / ((double)R * N); if (t < best) best = t;
    }
    return best;
}
static double sp_sort_H() {
    const int N = 1024, R = 1500; double best = 1e30;
    std::vector<H> base; for (int i = 0; i < N; ++i) base.push_back(H((i * 2654435761u) & 0xffff));
    for (int rep = 0; rep < 3; ++rep) {
        double t0 = now_ns();
        for (int r = 0; r < R; ++r) { std::vector<H> v(base); std::sort(v.begin(), v.end()); sink(v.data()); }
        double t = (now_ns() - t0) / ((double)R * N); if (t < best) best = t;
    }
    return best;
}
static double sp_str_append() {
    const int N = 4096, R = 8000; double best = 1e30;
    for (int rep = 0; rep < 3; ++rep) {
        double t0 = now_ns();
        for (int r = 0; r < R; ++r) { std::string s; for (int i = 0; i < N; ++i) s += (char)('a' + (i & 15)); sink(s.data()); }
        double t = (now_ns() - t0) / ((double)R * N); if (t < best) best = t;
    }
    return best;
}

int main() {
    printf("SPEED cs_vec_index_i %.4f\n", sp_index());
    printf("SPEED cs_vec_end_i %.4f\n", sp_end());
    printf("SPEED cs_vec_push_int %.4f\n", sp_push_int());
    printf("SPEED cs_vec_push_H %.4f\n", sp_push_H());
    printf("SPEED cs_vec_insert_H %.4f\n", sp_insert_H());
    printf("SPEED cs_vec_erase_H %.4f\n", sp_erase_H());
    printf("SPEED cs_vec_sort_H %.4f\n", sp_sort_H());
    printf("SPEED cs_str_append %.4f\n", sp_str_append());
    return 0;
}
