// function_probe — std::function instantiated across MANY distinct captured
// callables, each at a different signature flavour. This is the workload that
// the stage-3 callable_ops<Sig> port targets: per the design (§3.4),
// lifecycle ops fold across F at the byte level via the const callable_ops*
// indirection, so N distinct captures share one set of lifecycle bodies (the
// three func_* helpers in support.cpp) instead of paying for N copies of a
// per-F virtual table + concrete fn_func subclass.
//
// The captures are picked to hit both branches of the small-buffer
// discriminator:
//   - several stateless / one-pointer-captured lambdas (small, inline path)
//   - several lambdas that capture a heavy struct (large, heap path)
//
// Two signatures are used so the per-Sig invoke template instantiates twice
// but the lifecycle bodies fold to one.
#include <functional>
#include <string>
#include <utility>

template <int N>
struct Heavy {
    char pad[sizeof(void*) * 5 + N];
    int  seed;
    explicit Heavy(int s) : seed(s) { for (char& c : pad) c = static_cast<char>(s); }
    int operator()(int x) const { return pad[0] + seed + x; }
};

template <int N>
__attribute__((noinline)) int small_capture_exercise(int seed) {
    int local = seed;
    std::function<int(int)> f = [local](int x) { return local * x + N; };
    auto g = f;                          // copy
    auto h = std::move(g);               // move
    f.swap(h);                           // swap
    f = nullptr;                         // reset
    f = [local](int x) { return local + x + N; };  // assign
    return f(seed) ^ (int)static_cast<bool>(f);
}

template <int N>
__attribute__((noinline)) int large_capture_exercise(int seed) {
    Heavy<N> heavy{seed};
    std::function<int(int)> f = heavy;
    auto g = f;
    auto h = std::move(g);
    f.swap(h);
    return f(seed);
}

template <int N>
__attribute__((noinline)) long pair_sig_exercise(int seed) {
    int local = seed;
    std::function<long(int, int)> f = [local](int x, int y) { return long(local) * x + y + N; };
    auto g = f;
    g = std::move(f);
    return g(seed, seed + 1);
}

template <int... Ns>
void all(std::integer_sequence<int, Ns...>) {
    int acc = 0;
    ((acc ^= small_capture_exercise<Ns>(Ns)), ...);
    ((acc ^= large_capture_exercise<Ns>(Ns)), ...);
    long lacc = 0;
    ((lacc ^= pair_sig_exercise<Ns>(Ns)), ...);
    volatile auto sink = acc + (int)lacc;
    (void)sink;
}

int main() {
    all(std::make_integer_sequence<int, 12>{});
    return 0;
}
