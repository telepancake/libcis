// memprobe.cpp — representative MEMORY benchmarks.
//
// For each benchmark it reports three numbers, machine-readable, one row each:
//   internal : bytes the structure logically occupies (capacity-based, the
//              library's own accounting — sizeof(container) + capacity*elem etc.)
//   heap     : process heap actually in use for it, glibc mallinfo2().uordblks
//              delta — the REAL external cost incl. allocator rounding/overhead.
//   stack    : peak stack bytes touched while building/using it (SP watermark).
//
// Built twice by the harness — against libcis and against the non-type-erased
// reference — so every number is reported as an overhead vs the reference.
// Output line format (parsed by size_slope.py):
//   MEM <name> <n> <internal> <heap> <stack>

#include <vector>
#include <string>
#include <map>
#include <malloc.h>
#include <cstdio>
#include <cstddef>

static char*  g_stack_top;
static size_t g_stack_peak;

__attribute__((noinline)) static void stack_sample() {
    char here;
    size_t d = (size_t)(g_stack_top - &here);
    if (d > g_stack_peak) g_stack_peak = d;
}
#define STACK_BEGIN() do { char anchor; g_stack_top = &anchor; g_stack_peak = 0; } while (0)

static size_t heap_inuse() { return (size_t)mallinfo2().uordblks; }

static void emit(const char* name, size_t n, size_t internal, size_t heap, size_t stack) {
    printf("MEM %s %zu %zu %zu %zu\n", name, n, internal, heap, stack);
}

__attribute__((noinline)) static void bench_vec_int(size_t n) {
    STACK_BEGIN();
    size_t h0 = heap_inuse();
    std::vector<int> v;
    for (size_t i = 0; i < n; ++i) { v.push_back((int)i); stack_sample(); }
    size_t heap = heap_inuse() - h0;
    size_t internal = sizeof(v) + v.capacity() * sizeof(int);
    emit("vec_int", n, internal, heap, g_stack_peak);
}

struct Big { double a, b, c; std::string s; Big() : a(0), b(0), c(0), s("xy") {} };

__attribute__((noinline)) static void bench_vec_big(size_t n) {
    STACK_BEGIN();
    size_t h0 = heap_inuse();
    std::vector<Big> v;
    for (size_t i = 0; i < n; ++i) { v.emplace_back(); stack_sample(); }
    size_t heap = heap_inuse() - h0;
    size_t internal = sizeof(v) + v.capacity() * sizeof(Big);
    emit("vec_big", n, internal, heap, g_stack_peak);
}

__attribute__((noinline)) static void bench_string(size_t n) {
    STACK_BEGIN();
    size_t h0 = heap_inuse();
    std::string s;
    for (size_t i = 0; i < n; ++i) { s.push_back('a' + (char)(i & 15)); stack_sample(); }
    size_t heap = heap_inuse() - h0;
    size_t internal = sizeof(s) + s.capacity() + 1;
    emit("string", n, internal, heap, g_stack_peak);
}

__attribute__((noinline)) static void bench_map(size_t n) {
    STACK_BEGIN();
    size_t h0 = heap_inuse();
    std::map<int, int> m;
    for (size_t i = 0; i < n; ++i) { m[(int)((i * 2654435761u) & 0x7fffffff)] = (int)i; stack_sample(); }
    size_t heap = heap_inuse() - h0;
    // node ~= key+val + 3 pointers + color/pad; report self-accounting estimate.
    size_t node = sizeof(std::pair<const int, int>) + 3 * sizeof(void*) + sizeof(int);
    size_t internal = sizeof(m) + m.size() * node;
    emit("map", m.size(), internal, heap, g_stack_peak);
}

// map_big: a value type sized so the rb-tree node's 8-byte color packing
// (color folded into the parent pointer's low bit instead of a padded bool
// field) crosses a glibc malloc size-class boundary. The reference libstdc++
// node still carries a separate color byte, so its node lands one malloc bin
// higher; libcis's packed node lands a bin lower. This is where the node shrink
// shows as REAL heap (mallinfo2) overhead, not just sizeof — for map<int,int>
// both nodes round to the same bin so the shrink is invisible to glibc, but a
// finer-granularity (embedded / pool) allocator sees it on every node.
// V12 makes pair<const int, V12> == 16 bytes, so the node payload is
// 24+16=40 (libcis, packed) vs 32+16=48 (reference, separate color byte) —
// straddling glibc's 40/56-byte usable bins (chunk 48 vs 64), a real 16-byte
// heap saving per node that mallinfo2 reports.
struct V12 { char b[12]; V12() { for (char& c : b) c = 0; } };

__attribute__((noinline)) static void bench_map_big(size_t n) {
    STACK_BEGIN();
    size_t h0 = heap_inuse();
    std::map<int, V12> m;
    for (size_t i = 0; i < n; ++i) { m[(int)((i * 2654435761u) & 0x7fffffff)]; stack_sample(); }
    size_t heap = heap_inuse() - h0;
    size_t node = sizeof(std::pair<const int, V12>) + 3 * sizeof(void*);
    size_t internal = sizeof(m) + m.size() * node;
    emit("map_big", m.size(), internal, heap, g_stack_peak);
}

int main() {
    bench_vec_int(10000);
    bench_vec_big(2000);
    bench_string(50000);
    bench_map(5000);
    bench_map_big(5000);
    return 0;
}
