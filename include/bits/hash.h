#pragma once

// std::hash — primary template + scalar/pointer/enum specialisations.
// Split out of <functional> ([unord.hash], libc++ __functional/hash.h) so that
// <variant> can use std::hash without including <functional> (which now
// includes <iterator> -> <variant>: a cycle).

#include <type_traits>
#include <utility>
#include <cstring>          // memcpy
#include <cstdint>          // uint32_t
#include <bits/pointer.h>   // addressof

namespace std {

//===----------------------------------------------------------------------===//
// hash — primary template + specialisations
// (from libc++ __functional/hash.h)
//
// Implementation: murmur2 (32-bit size_t) or cityhash64 (64-bit size_t).
//===----------------------------------------------------------------------===//

// Forward declaration
template<class T>
struct hash;

namespace detail {

// loadword: copy N bytes from arbitrary address into a size_t
template<class Size>
inline Size hash_loadword(const void* p) noexcept {
    Size r;
    __builtin_memcpy(&r, p, sizeof(r));
    return r;
}

struct hash_pair_t {
    size_t first;
    size_t second;
};

// murmur2 (32-bit size_t)
template<class Size, size_t = sizeof(Size) * __CHAR_BIT__>
struct murmur2_or_cityhash;

template<class Size>
struct murmur2_or_cityhash<Size, 32> {
    Size operator()(const void* key, Size len) const noexcept {
        const Size m = 0x5bd1e995u;
        const Size r = 24;
        Size h = len;
        const unsigned char* data = static_cast<const unsigned char*>(key);
        for (; len >= 4; data += 4, len -= 4) {
            Size k = hash_loadword<Size>(data);
            k *= m; k ^= k >> r; k *= m;
            h *= m; h ^= k;
        }
        switch (len) {
        case 3: h ^= static_cast<Size>(data[2] << 16); [[fallthrough]];
        case 2: h ^= static_cast<Size>(data[1] << 8);  [[fallthrough]];
        case 1: h ^= data[0]; h *= m;
        }
        h ^= h >> 13; h *= m; h ^= h >> 15;
        return h;
    }
};

// cityhash64 (64-bit size_t)
template<class Size>
struct murmur2_or_cityhash<Size, 64> {
    // Some primes between 2^63 and 2^64.
    static constexpr Size k0 = 0xc3a5c85c97cb3127ULL;
    static constexpr Size k1 = 0xb492b66fbe98f273ULL;
    static constexpr Size k2 = 0x9ae16a3b2f90404fULL;
    static constexpr Size k3 = 0xc949d7c7509e6557ULL;

    static Size rotate(Size val, int shift) noexcept {
        return shift == 0 ? val : ((val >> shift) | (val << (64 - shift)));
    }
    static Size rotate_by_at_least_1(Size val, int shift) noexcept {
        return (val >> shift) | (val << (64 - shift));
    }
    static Size shift_mix(Size val) noexcept { return val ^ (val >> 47); }

    static Size hash_len_16(Size u, Size v) noexcept {
        const Size mul = 0x9ddfea08eb382d69ULL;
        Size a = (u ^ v) * mul; a ^= (a >> 47);
        Size b = (v ^ a) * mul; b ^= (b >> 47); b *= mul;
        return b;
    }

    static Size hash_len_0_to_16(const char* s, Size len) noexcept {
        if (len > 8) {
            const Size a = hash_loadword<Size>(s);
            const Size b = hash_loadword<Size>(s + len - 8);
            return hash_len_16(a, rotate_by_at_least_1(b + len, static_cast<int>(len))) ^ b;
        }
        if (len >= 4) {
            const uint32_t a = hash_loadword<uint32_t>(s);
            const uint32_t b = hash_loadword<uint32_t>(s + len - 4);
            return hash_len_16(len + (static_cast<Size>(a) << 3), b);
        }
        if (len > 0) {
            const unsigned char a = static_cast<unsigned char>(s[0]);
            const unsigned char b = static_cast<unsigned char>(s[len >> 1]);
            const unsigned char c = static_cast<unsigned char>(s[len - 1]);
            const uint32_t y = static_cast<uint32_t>(a) + (static_cast<uint32_t>(b) << 8);
            const uint32_t z = static_cast<uint32_t>(len) + (static_cast<uint32_t>(c) << 2);
            return shift_mix(y * k2 ^ z * k3) * k2;
        }
        return k2;
    }

    static Size hash_len_17_to_32(const char* s, Size len) noexcept {
        const Size a = hash_loadword<Size>(s) * k1;
        const Size b = hash_loadword<Size>(s + 8);
        const Size c = hash_loadword<Size>(s + len - 8) * k2;
        const Size d = hash_loadword<Size>(s + len - 16) * k0;
        return hash_len_16(
            rotate(a - b, 43) + rotate(c, 30) + d,
            a + rotate(b ^ k3, 20) - c + len);
    }

    static hash_pair_t weak_hash_len_32_with_seeds(
        Size w, Size x, Size y, Size z, Size a, Size b) noexcept
    {
        a += w;
        b = rotate(b + a + z, 21);
        const Size c = a;
        a += x; a += y;
        b += rotate(a, 44);
        return { a + z, b + c };
    }

    static hash_pair_t weak_hash_len_32_with_seeds(
        const char* s, Size a, Size b) noexcept
    {
        return weak_hash_len_32_with_seeds(
            hash_loadword<Size>(s),
            hash_loadword<Size>(s + 8),
            hash_loadword<Size>(s + 16),
            hash_loadword<Size>(s + 24),
            a, b);
    }

    static Size hash_len_33_to_64(const char* s, size_t len) noexcept {
        Size z = hash_loadword<Size>(s + 24);
        Size a = hash_loadword<Size>(s) + (len + hash_loadword<Size>(s + len - 16)) * k0;
        Size b = rotate(a + z, 52);
        Size c = rotate(a, 37);
        a += hash_loadword<Size>(s + 8);
        c += rotate(a, 7);
        a += hash_loadword<Size>(s + 16);
        Size vf = a + z;
        Size vs = b + rotate(a, 31) + c;
        a = hash_loadword<Size>(s + 16) + hash_loadword<Size>(s + len - 32);
        z += hash_loadword<Size>(s + len - 8);
        b = rotate(a + z, 52);
        c = rotate(a, 37);
        a += hash_loadword<Size>(s + len - 24);
        c += rotate(a, 7);
        a += hash_loadword<Size>(s + len - 16);
        Size wf = a + z;
        Size ws = b + rotate(a, 31) + c;
        Size r = shift_mix((vf + ws) * k2 + (wf + vs) * k0);
        return shift_mix(r * k0 + vs) * k2;
    }

    Size operator()(const void* key, Size len) const noexcept {
        const char* s = static_cast<const char*>(key);
        if (len <= 32) {
            if (len <= 16) return hash_len_0_to_16(s, len);
            else           return hash_len_17_to_32(s, len);
        } else if (len <= 64) {
            return hash_len_33_to_64(s, len);
        }
        Size x = hash_loadword<Size>(s + len - 40);
        Size y = hash_loadword<Size>(s + len - 16) + hash_loadword<Size>(s + len - 56);
        Size z = hash_len_16(hash_loadword<Size>(s + len - 48) + len,
                              hash_loadword<Size>(s + len - 24));
        hash_pair_t v = weak_hash_len_32_with_seeds(s + len - 64, len, z);
        hash_pair_t w = weak_hash_len_32_with_seeds(s + len - 32, y + k1, x);
        x = x * k1 + hash_loadword<Size>(s);
        len = (len - 1) & ~static_cast<Size>(63);
        do {
            x = rotate(x + y + v.first + hash_loadword<Size>(s + 8), 37) * k1;
            y = rotate(y + v.second + hash_loadword<Size>(s + 48), 42) * k1;
            x ^= w.second;
            y += v.first + hash_loadword<Size>(s + 40);
            z = rotate(z + w.first, 33) * k1;
            v = weak_hash_len_32_with_seeds(s, v.second * k1, x + w.first);
            w = weak_hash_len_32_with_seeds(s + 32, z + w.second,
                                             y + hash_loadword<Size>(s + 16));
            Size tmp = z; z = x; x = tmp; // swap(z, x)
            s += 64;
            len -= 64;
        } while (len != 0);
        return hash_len_16(
            hash_len_16(v.first, w.first) + shift_mix(y) * k1 + z,
            hash_len_16(v.second, w.second) + x);
    }
};

inline size_t hash_memory(const void* ptr, size_t size) noexcept {
    return murmur2_or_cityhash<size_t>()(ptr, size);
}

// scalar_hash: hashes a scalar value by interpreting its bytes
template<class T, size_t N = sizeof(T) / sizeof(size_t)>
struct scalar_hash;

template<class T>
struct scalar_hash<T, 0> {
    size_t operator()(T v) const noexcept {
        union { T t; size_t a; } u;
        u.a = 0;
        u.t = v;
        return u.a;
    }
};

template<class T>
struct scalar_hash<T, 1> {
    size_t operator()(T v) const noexcept {
        union { T t; size_t a; } u;
        u.t = v;
        return u.a;
    }
};

template<class T>
struct scalar_hash<T, 2> {
    size_t operator()(T v) const noexcept {
        union { T t; struct { size_t a; size_t b; } s; } u;
        u.t = v;
        return hash_memory(std::addressof(u), sizeof(u));
    }
};

template<class T>
struct scalar_hash<T, 3> {
    size_t operator()(T v) const noexcept {
        union { T t; struct { size_t a; size_t b; size_t c; } s; } u;
        u.t = v;
        return hash_memory(std::addressof(u), sizeof(u));
    }
};

template<class T>
struct scalar_hash<T, 4> {
    size_t operator()(T v) const noexcept {
        union { T t; struct { size_t a; size_t b; size_t c; size_t d; } s; } u;
        u.t = v;
        return hash_memory(std::addressof(u), sizeof(u));
    }
};

inline size_t hash_combine(size_t lhs, size_t rhs) noexcept {
    hash_pair_t p = { lhs, rhs };
    return hash_memory(std::addressof(p), sizeof(p));
}

} // namespace detail

// hash primary template — disabled by default (no operator())
template<class T>
struct hash {
    hash()                   = delete;
    hash(const hash&)        = delete;
    hash& operator=(const hash&) = delete;
};

// hash<T*>
template<class T>
struct hash<T*> {
    size_t operator()(T* v) const noexcept {
        union { T* t; size_t a; } u;
        u.t = v;
        return detail::hash_memory(std::addressof(u), sizeof(u));
    }
};

// hash<nullptr_t>
template<>
struct hash<nullptr_t> {
    [[nodiscard]] size_t operator()(nullptr_t) const noexcept { return 662607004ull; }
};

// hash<integral types>: small types -> cast; large types -> scalar_hash
namespace detail {
    template<class T>
    struct hash_integral {
        size_t operator()(T v) const noexcept {
            if constexpr (sizeof(T) <= sizeof(size_t))
                return static_cast<size_t>(v);
            else
                return scalar_hash<T>{}(v);
        }
    };

    // hash<floating point>: -0.0 == 0.0 case handled
    template<class T>
    struct hash_floating : scalar_hash<T> {
        size_t operator()(T v) const noexcept {
            if (v == static_cast<T>(0)) return 0;
            return scalar_hash<T>::operator()(v);
        }
    };
} // namespace detail

// Specialise hash for all integral types
template<> struct hash<bool>               : detail::hash_integral<bool> {};
template<> struct hash<char>               : detail::hash_integral<char> {};
template<> struct hash<signed char>        : detail::hash_integral<signed char> {};
template<> struct hash<unsigned char>      : detail::hash_integral<unsigned char> {};
template<> struct hash<wchar_t>            : detail::hash_integral<wchar_t> {};
template<> struct hash<char8_t>            : detail::hash_integral<char8_t> {};
template<> struct hash<char16_t>           : detail::hash_integral<char16_t> {};
template<> struct hash<char32_t>           : detail::hash_integral<char32_t> {};
template<> struct hash<short>              : detail::hash_integral<short> {};
template<> struct hash<unsigned short>     : detail::hash_integral<unsigned short> {};
template<> struct hash<int>                : detail::hash_integral<int> {};
template<> struct hash<unsigned int>       : detail::hash_integral<unsigned int> {};
template<> struct hash<long>               : detail::hash_integral<long> {};
template<> struct hash<unsigned long>      : detail::hash_integral<unsigned long> {};
template<> struct hash<long long>          : detail::hash_integral<long long> {};
template<> struct hash<unsigned long long> : detail::hash_integral<unsigned long long> {};
#if defined(__SIZEOF_INT128__)
template<> struct hash<__int128>           : detail::hash_integral<__int128> {};
template<> struct hash<unsigned __int128>  : detail::hash_integral<unsigned __int128> {};
#endif

// hash for floating point
template<> struct hash<float>       : detail::hash_floating<float> {};
template<> struct hash<double>      : detail::hash_floating<double> {};

// hash<long double>: platform-specific padding zeroing (x86/x86_64)
template<>
struct hash<long double> : detail::scalar_hash<long double> {
    size_t operator()(long double v) const noexcept {
        if (v == 0.0L) return 0;
#if defined(__i386__) || (defined(__x86_64__) && defined(__ILP32__))
        union {
            long double t;
            struct { size_t a; size_t b; size_t c; size_t d; } s;
        } u;
        u.s.a = 0; u.s.b = 0; u.s.c = 0; u.s.d = 0;
        u.t = v;
        return u.s.a ^ u.s.b ^ u.s.c ^ u.s.d;
#elif defined(__x86_64__)
        union {
            long double t;
            struct { size_t a; size_t b; } s;
        } u;
        u.s.a = 0; u.s.b = 0;
        u.t = v;
        return u.s.a ^ u.s.b;
#else
        return scalar_hash<long double>::operator()(v);
#endif
    }
};

// hash for enums — dispatch through underlying_type
template<class T> requires is_enum_v<T>
struct hash<T> {
    size_t operator()(T v) const noexcept {
        using U = underlying_type_t<T>;
        return hash<U>{}(static_cast<U>(v));
    }
};


} // namespace std
