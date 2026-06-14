// unordered_dense.cpp - exercise martinus/unordered_dense (header-only).
//
// A dense, cache-friendly hash map/set; pulls in <string>, <vector>, hashing,
// and the container machinery so its instantiated code lands in .text.
#include <ankerl/unordered_dense.h>

#include <cstdio>
#include <string>

int main() {
    ankerl::unordered_dense::map<std::string, int> m;
    for (int i = 0; i < 1000; ++i)
        m[std::string("k") + std::to_string(i % 100)] += i;

    ankerl::unordered_dense::set<long long> s;
    for (int i = 0; i < 1000; ++i)
        s.insert(static_cast<long long>(i) * 2654435761ULL);

    long long acc = 0;
    for (auto& kv : m) acc += kv.second;
    acc += static_cast<long long>(s.size());
    m.erase("k7");
    std::printf("%lld %zu\n", acc, m.size());
    return 0;
}
