// speedprobe.cpp — per-method SPEED via DETERMINISTIC instruction count.
//
// The earlier wall-clock version was the rigged-microbenchmark trap on the speed
// axis: a tiny L1-hot, branch-predicted loop plus process startup measures jitter,
// not the method. This version runs ONE realistic workload per process and is
// measured under cachegrind by the harness — instruction reads (Ir) are
// deterministic, so there is no timing noise to average away. The harness also
// runs `noop` and subtracts it, removing fixed startup so the number is the
// method's own executed instructions over realistic work. libcis vs the
// non-type-erased reference differ ONLY in the library, so the Ir delta is
// attributable to the method.
//
// Usage: speedprobe <workload>   (the harness picks the workload; cachegrind counts)

#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

struct H { char pad[7]; std::string s; H() : s("x") {} H(int i) : s(i & 1 ? "ab" : "abcd") {}
           bool operator<(const H& o) const { return s < o.s; } };

static volatile unsigned long g_sink;

// operator[] in the structure-of-arrays / parallel-array idiom.
static void w_index() {
    const int N = 8192; std::vector<int> a(N), b(N), c(N);
    for (int i = 0; i < N; ++i) { a[i] = i; b[i] = 2 * i; c[i] = i ^ 5; }
    long long acc = 0;
    for (int r = 0; r < 200; ++r) for (int i = 0; i < N; ++i) acc += a[i] + b[i] + c[i];
    g_sink = acc;
}
// end() as the loop bound in an iterator traversal.
static void w_end() {
    const int N = 8192; std::vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = i;
    long long acc = 0;
    for (int r = 0; r < 200; ++r) for (auto it = v.begin(); it != v.end(); ++it) acc += *it;
    g_sink = acc;
}
static void w_push_int() {
    for (int r = 0; r < 80; ++r) { std::vector<int> v; for (int i = 0; i < 4096; ++i) v.push_back(i); g_sink = v.size(); }
}
static void w_push_H() {
    for (int r = 0; r < 40; ++r) { std::vector<H> v; for (int i = 0; i < 1024; ++i) v.push_back(H(i)); g_sink = v.size(); }
}
static void w_insert_H() {
    for (int r = 0; r < 60; ++r) { std::vector<H> v; for (int i = 0; i < 512; ++i) v.insert(v.begin(), H(i)); g_sink = v.size(); }
}
static void w_erase_H() {
    std::vector<H> base; for (int i = 0; i < 512; ++i) base.push_back(H(i));
    for (int r = 0; r < 60; ++r) { std::vector<H> v(base); while (!v.empty()) v.erase(v.begin()); g_sink = v.size(); }
}
static void w_sort_H() {
    std::vector<H> base; for (int i = 0; i < 2048; ++i) base.push_back(H((i * 2654435761u) & 0xffff));
    for (int r = 0; r < 60; ++r) { std::vector<H> v(base); std::sort(v.begin(), v.end()); g_sink = v.size(); }
}
static void w_str_append() {
    for (int r = 0; r < 200; ++r) { std::string s; for (int i = 0; i < 4096; ++i) s += (char)('a' + (i & 15)); g_sink = s.size(); }
}

int main(int argc, char** argv) {
    if (argc < 2) return 0;
    const char* m = argv[1];
    if      (!strcmp(m, "noop"))            { /* startup baseline */ }
    else if (!strcmp(m, "cs_vec_index_i"))  w_index();
    else if (!strcmp(m, "cs_vec_end_i"))    w_end();
    else if (!strcmp(m, "cs_vec_push_int")) w_push_int();
    else if (!strcmp(m, "cs_vec_push_H"))   w_push_H();
    else if (!strcmp(m, "cs_vec_insert_H")) w_insert_H();
    else if (!strcmp(m, "cs_vec_erase_H"))  w_erase_H();
    else if (!strcmp(m, "cs_vec_sort_H"))   w_sort_H();
    else if (!strcmp(m, "cs_str_append"))   w_str_append();
    return (int)(g_sink & 1);
}
