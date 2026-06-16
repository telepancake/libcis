// src/support.cpp — freestanding C++ runtime support for libcis.
//
// Provides ALL symbols normally imported from libstdc++/libsupc++ so that
// the final binary can be linked with -nodefaultlibs (only libc + compiler
// runtime needed).
//
// Compilation:
//   g++-10 -std=gnu++20 -fno-exceptions -fno-rtti \
//          -nostdinc++ -Iinclude -c src/support.cpp
//
// We pull in the C standard headers (stdlib.h, string.h, stdint.h, sched.h)
// and our own libcis headers for the types that must match (<new>, <exception>).

#include <new>         // nothrow_t, align_val_t, new_handler declarations
#include <exception>   // exception, bad_exception, terminate_handler declarations
#include <stdlib.h>    // malloc, free, abort, aligned_alloc / posix_memalign
#include <string.h>    // (not strictly needed but keeps things clean)
#include <stdint.h>    // int64_t, uint8_t
#include <sched.h>     // sched_yield — for guard spin loop

// ---------------------------------------------------------------------------
// std::nothrow
// ---------------------------------------------------------------------------
namespace std {
    const nothrow_t nothrow{};
} // namespace std

// ---------------------------------------------------------------------------
// std::new_handler / set_new_handler / get_new_handler
//
// An atomic pointer to the current new-handler (initially nullptr).
// ---------------------------------------------------------------------------
namespace std {

namespace {
    // _Atomic-style storage using __atomic builtins.
    new_handler g_new_handler = nullptr;
} // anonymous

new_handler set_new_handler(new_handler h) noexcept {
    return __atomic_exchange_n(&g_new_handler, h, __ATOMIC_SEQ_CST);
}

new_handler get_new_handler() noexcept {
    return __atomic_load_n(&g_new_handler, __ATOMIC_SEQ_CST);
}

} // namespace std

// ---------------------------------------------------------------------------
// std::terminate_handler / set_terminate / get_terminate / terminate
// ---------------------------------------------------------------------------
namespace std {

namespace {
    terminate_handler g_terminate_handler = nullptr;
} // anonymous

terminate_handler set_terminate(terminate_handler h) noexcept {
    return __atomic_exchange_n(&g_terminate_handler, h, __ATOMIC_SEQ_CST);
}

terminate_handler get_terminate() noexcept {
    return __atomic_load_n(&g_terminate_handler, __ATOMIC_SEQ_CST);
}

// terminate(): call the installed handler (if any), then abort.
// Must not return — declared [[noreturn]] in <exception>.
[[noreturn]] void terminate() noexcept {
    terminate_handler h = __atomic_load_n(&g_terminate_handler, __ATOMIC_SEQ_CST);
    if (h) {
        h(); // handler must not return; if it does, fall through to abort
    }
    ::abort();
    __builtin_unreachable();
}

} // namespace std

// ---------------------------------------------------------------------------
// Internal helper: new-handler loop.
//
// Called when malloc/aligned_alloc returns nullptr.  Repeatedly invokes the
// new-handler (if any); each time we re-try the allocation.  If no handler
// is installed (or the handler keeps failing), we call std::terminate()
// because we cannot throw std::bad_alloc under -fno-exceptions.
// ---------------------------------------------------------------------------
namespace {

// Try to allocate `size` bytes (unaligned).  Returns non-null or terminates.
[[nodiscard]] void* new_loop(std::size_t size) noexcept {
    for (;;) {
        void* p = ::malloc(size == 0 ? 1 : size);
        if (p) return p;
        std::new_handler h = std::get_new_handler();
        if (!h) std::terminate();
        h(); // handler may free memory or install a different handler
    }
}

// Try to allocate `size` bytes with alignment `align`.
// Returns non-null or terminates.
[[nodiscard]] void* new_loop_aligned(std::size_t size, std::size_t align) noexcept {
    for (;;) {
        // posix_memalign requires alignment >= sizeof(void*) and power-of-two.
        // aligned_alloc requires size to be a multiple of alignment.
        // Use posix_memalign for simplicity; it works on glibc and can be
        // freed with free().
        if (size == 0) size = 1;
        // Round size up to multiple of alignment so aligned_alloc is happy.
        // posix_memalign has no such requirement, but be defensive.
        void* p = nullptr;
        int rc = ::posix_memalign(&p, align < sizeof(void*) ? sizeof(void*) : align, size);
        if (rc == 0 && p) return p;
        std::new_handler h = std::get_new_handler();
        if (!h) std::terminate();
        h();
    }
}

} // anonymous namespace
// Replaceable allocation functions are WEAK: a test TU that defines its own
// operator new/delete (libc++'s count_new.h instrumentation) overrides these
// at link time, per ELF replaceable-function practice (glibc does the same).
// Without test definitions the weak ones are used.
#define LIBCIS_REPLACEABLE __attribute__((__weak__))

// ---------------------------------------------------------------------------
// operator new / new[]  (scalar and array)
// ---------------------------------------------------------------------------

// Plain scalar new — may loop through new_handler; terminates if OOM
// (cannot throw bad_alloc under -fno-exceptions — comment documents intent).
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new(std::size_t size) {
    return new_loop(size);
}

// Aligned scalar new
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new(std::size_t size, std::align_val_t alignment) {
    return new_loop_aligned(size, static_cast<std::size_t>(alignment));
}

// Nothrow scalar new — returns nullptr on failure instead of terminating.
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
    void* p = ::malloc(size == 0 ? 1 : size);
    if (p) return p;
    // Try new-handler once per iteration, but return nullptr rather than abort.
    for (;;) {
        std::new_handler h = std::get_new_handler();
        if (!h) return nullptr;
        h();
        p = ::malloc(size == 0 ? 1 : size);
        if (p) return p;
    }
}

// Aligned nothrow scalar new
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new(std::size_t size, std::align_val_t alignment,
                                  const std::nothrow_t&) noexcept {
    std::size_t align = static_cast<std::size_t>(alignment);
    if (align < sizeof(void*)) align = sizeof(void*);
    if (size == 0) size = 1;
    void* p = nullptr;
    if (::posix_memalign(&p, align, size) == 0 && p) return p;
    for (;;) {
        std::new_handler h = std::get_new_handler();
        if (!h) return nullptr;
        h();
        p = nullptr;
        if (::posix_memalign(&p, align, size) == 0 && p) return p;
    }
}

// Array new delegates to scalar new
[[nodiscard]] LIBCIS_REPLACEABLE void* operator new[](std::size_t size) {
    return ::operator new(size);
}

[[nodiscard]] LIBCIS_REPLACEABLE void* operator new[](std::size_t size, std::align_val_t alignment) {
    return ::operator new(size, alignment);
}

[[nodiscard]] LIBCIS_REPLACEABLE void* operator new[](std::size_t size, const std::nothrow_t& nt) noexcept {
    return ::operator new(size, nt);
}

[[nodiscard]] LIBCIS_REPLACEABLE void* operator new[](std::size_t size, std::align_val_t alignment,
                                    const std::nothrow_t& nt) noexcept {
    return ::operator new(size, alignment, nt);
}

// ---------------------------------------------------------------------------
// operator delete / delete[]
//
// On glibc, memory allocated with posix_memalign() is freed with free().
// ---------------------------------------------------------------------------

LIBCIS_REPLACEABLE void operator delete(void* ptr) noexcept {
    ::free(ptr);
}

// The default sized/nothrow forms MUST forward to the replaceable base form
// ([new.delete.single]): a program replacing only operator delete(void*) --
// e.g. the suite's count_new.h -- must still intercept these deallocations.
LIBCIS_REPLACEABLE void operator delete(void* ptr, std::size_t /*size*/) noexcept {
    ::operator delete(ptr);
}

LIBCIS_REPLACEABLE void operator delete(void* ptr, std::align_val_t /*alignment*/) noexcept {
    ::free(ptr);
}

LIBCIS_REPLACEABLE void operator delete(void* ptr, std::size_t /*size*/,
                     std::align_val_t alignment) noexcept {
    ::operator delete(ptr, alignment);
}

LIBCIS_REPLACEABLE void operator delete(void* ptr, const std::nothrow_t&) noexcept {
    ::operator delete(ptr);
}

LIBCIS_REPLACEABLE void operator delete(void* ptr, std::align_val_t alignment,
                     const std::nothrow_t&) noexcept {
    ::operator delete(ptr, alignment);
}

// Array delete
LIBCIS_REPLACEABLE void operator delete[](void* ptr) noexcept {
    ::free(ptr);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, std::size_t /*size*/) noexcept {
    ::operator delete[](ptr);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, std::align_val_t /*alignment*/) noexcept {
    ::free(ptr);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, std::size_t /*size*/,
                        std::align_val_t alignment) noexcept {
    ::operator delete[](ptr, alignment);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, const std::nothrow_t&) noexcept {
    ::operator delete[](ptr);
}

LIBCIS_REPLACEABLE void operator delete[](void* ptr, std::align_val_t alignment,
                        const std::nothrow_t&) noexcept {
    ::operator delete[](ptr, alignment);
}

#undef LIBCIS_REPLACEABLE
// ---------------------------------------------------------------------------
// __cxa_guard_acquire / __cxa_guard_release / __cxa_guard_abort
//
// Itanium C++ ABI §3.3.2: the guard object is a 64-bit value (on 64-bit
// platforms; 32-bit on 32-bit).  Byte 0 (LSB, little-endian) = 1 means
// "already initialized"; we use byte 1 as a spin-lock flag.
//
// Scheme:
//   acquire:
//     1. Atomically load byte 0 with ACQUIRE ordering.
//        If 1 → already done, return 0 (skip init).
//     2. CAS byte 1 from 0 → 1 (acquire the "in-progress" lock).
//        If CAS fails, spin + yield then go to step 1.
//     3. Re-check byte 0 (another thread may have finished while we spun).
//        If 1 → release byte-1 lock, return 0.
//     4. Return 1 → caller must run the initializer.
//   release:
//     Store 1 into byte 0 with RELEASE, then store 0 into byte 1 with RELEASE.
//   abort:
//     Store 0 into byte 1 (no init happened), RELEASE.
//
// The guard object is 8 bytes; we treat it as int64_t but address individual
// bytes via uint8_t* pointers.  All accesses use __atomic_* builtins so GCC
// generates the correct barriers without needing <stdatomic.h>.
// ---------------------------------------------------------------------------
extern "C" {

// The guard object is declared as int64_t by the ABI, but we need byte
// access.  We receive it as a pointer to 64-bit value.
int __cxa_guard_acquire(int64_t* guard_object) {
    uint8_t* initialized   = reinterpret_cast<uint8_t*>(guard_object);     // byte 0
    uint8_t* in_progress   = reinterpret_cast<uint8_t*>(guard_object) + 1; // byte 1

    for (;;) {
        // Fast path: already initialized.
        if (__atomic_load_n(initialized, __ATOMIC_ACQUIRE) != 0) {
            return 0; // skip initialization
        }

        // Try to grab the in-progress lock: CAS 0 → 1.
        uint8_t expected = 0;
        uint8_t desired  = 1;
        if (__atomic_compare_exchange_n(in_progress, &expected, desired,
                                        /*weak=*/0,
                                        __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) {
            // We own the lock.  Re-check byte 0 in case another thread finished
            // while we were spinning before we grabbed the lock.
            if (__atomic_load_n(initialized, __ATOMIC_ACQUIRE) != 0) {
                // Already done by another thread — release lock and return 0.
                __atomic_store_n(in_progress, (uint8_t)0, __ATOMIC_RELEASE);
                return 0;
            }
            return 1; // caller must run the initializer
        }

        // Another thread holds the lock; yield and retry.
        ::sched_yield();
    }
}

void __cxa_guard_release(int64_t* guard_object) {
    uint8_t* initialized = reinterpret_cast<uint8_t*>(guard_object);
    uint8_t* in_progress = reinterpret_cast<uint8_t*>(guard_object) + 1;

    // Mark as initialized (RELEASE so prior writes are visible).
    __atomic_store_n(initialized, (uint8_t)1, __ATOMIC_RELEASE);
    // Release the in-progress lock.
    __atomic_store_n(in_progress, (uint8_t)0, __ATOMIC_RELEASE);
}

void __cxa_guard_abort(int64_t* guard_object) {
    uint8_t* in_progress = reinterpret_cast<uint8_t*>(guard_object) + 1;
    // Initialization failed; release the lock so other threads can retry.
    __atomic_store_n(in_progress, (uint8_t)0, __ATOMIC_RELEASE);
}

// ---------------------------------------------------------------------------
// __cxa_pure_virtual
//
// Called when a pure-virtual function is invoked (undefined behavior, but we
// try to give a useful message before trapping).
// ---------------------------------------------------------------------------
[[noreturn]] void __cxa_pure_virtual() {
    // Use the write() syscall-like approach via fputs equivalent.
    // __builtin_fprintf is not available as a built-in; use write to stderr (fd 2).
    const char msg[] = "pure virtual function called\n";
    // Avoid pulling in stdio; use __builtin_write if available, otherwise trap.
    // On Linux/glibc, fileno(stderr)==2 and write() is in libc.
    extern int write(int, const void*, unsigned long) noexcept __asm__("write");
    write(2, msg, sizeof(msg) - 1);
    __builtin_trap();
    __builtin_unreachable();
}

} // extern "C"

// ---------------------------------------------------------------------------
// std::exception and std::bad_exception out-of-line definitions.
//
// By defining the key function (virtual destructor) here, this TU becomes
// the "key TU" that emits the vtable for these classes.  All other TUs that
// use std::exception get a reference to the vtable defined here.
//
// The declarations in include/exception must NOT carry inline bodies or
// gnu::visibility("default") import annotations — they are just declarations.
// ---------------------------------------------------------------------------
namespace std {

// exception
exception::~exception() noexcept {}
const char* exception::what() const noexcept { return "std::exception"; }

// bad_exception
bad_exception::~bad_exception() noexcept {}
const char* bad_exception::what() const noexcept { return "std::bad_exception"; }

} // namespace std

// ---------------------------------------------------------------------------
// __cxa_thread_atexit: register a thread_local object's destructor.
//
// Emitted by the compiler for `static thread_local T x;` with a non-trivial
// dtor (e.g. <future>'s at-thread-exit registry).  Normally provided by
// libstdc++/libc++abi, which we do NOT link (-nodefaultlibs).  glibc 2.18+
// ships the real machinery as __cxa_thread_atexit_impl; forward to it.
// ---------------------------------------------------------------------------
extern "C" int __cxa_thread_atexit_impl(void (*func)(void*), void* obj, void* dso_handle);

extern "C" int __cxa_thread_atexit(void (*func)(void*), void* obj, void* dso_handle) {
    return __cxa_thread_atexit_impl(func, obj, dso_handle);
}

// ---------------------------------------------------------------------------
// Classic stream-dependent locale facets (num_get/num_put/money*/time*/messages).
//
// These facets are defined in <locale> but reference a COMPLETE std::ios_base,
// which <locale> itself cannot see (the <ios> -> <locale> include cycle leaves
// ios_base incomplete while <locale> is parsed).  So their CLASSIC instances are
// instantiated and registered HERE, in a TU that includes <sstream> first and
// therefore has a complete ios_base.  Registration goes into the same global
// facet registry the header uses (locale::__register_facet); the facet `id`
// objects are shared inline statics, so lookups in any TU resolve correctly.
//
// Reported per the porting contract: this is the only src/support.cpp edit for
// the localization pass.
// ---------------------------------------------------------------------------
#include <sstream>   // pulls <ios>/<streambuf>/<istream>/<ostream> fully, then <locale>
#include <locale>

namespace std {
namespace detail {

// Construct the classic stream facets in STATIC STORAGE (placement-new into
// aligned buffers) rather than with ::operator new.  The libc++ test harness
// replaces the global operator new/delete to count outstanding allocations and
// asserts the count is 0 across locale construction, so these long-lived classic
// facets must not go through operator new.  refs=1 => never destroyed.
template <class Facet, class... Args>
static Facet* make_static_facet(Args&&... args) {
    alignas(Facet) static unsigned char buf[sizeof(Facet)];
    return ::new (static_cast<void*>(buf)) Facet(static_cast<Args&&>(args)...);
}

struct stream_facet_init {
    stream_facet_init() {
        locale::__register_facet(std::num_get<char>::id.__get(),    make_static_facet<std::num_get<char>>(1));
        locale::__register_facet(std::num_get<wchar_t>::id.__get(), make_static_facet<std::num_get<wchar_t>>(1));
        locale::__register_facet(std::num_put<char>::id.__get(),    make_static_facet<std::num_put<char>>(1));
        locale::__register_facet(std::num_put<wchar_t>::id.__get(), make_static_facet<std::num_put<wchar_t>>(1));
        locale::__register_facet(std::time_get<char>::id.__get(),    make_static_facet<std::time_get<char>>(1));
        locale::__register_facet(std::time_get<wchar_t>::id.__get(), make_static_facet<std::time_get<wchar_t>>(1));
        locale::__register_facet(std::time_put<char>::id.__get(),    make_static_facet<std::time_put<char>>(1));
        locale::__register_facet(std::time_put<wchar_t>::id.__get(), make_static_facet<std::time_put<wchar_t>>(1));
        locale::__register_facet(std::money_get<char>::id.__get(),    make_static_facet<std::money_get<char>>(1));
        locale::__register_facet(std::money_get<wchar_t>::id.__get(), make_static_facet<std::money_get<wchar_t>>(1));
        locale::__register_facet(std::money_put<char>::id.__get(),    make_static_facet<std::money_put<char>>(1));
        locale::__register_facet(std::money_put<wchar_t>::id.__get(), make_static_facet<std::money_put<wchar_t>>(1));
        locale::__register_facet(std::moneypunct<char, false>::id.__get(),    make_static_facet<std::moneypunct<char, false>>(1));
        locale::__register_facet(std::moneypunct<char, true>::id.__get(),     make_static_facet<std::moneypunct<char, true>>(1));
        locale::__register_facet(std::moneypunct<wchar_t, false>::id.__get(), make_static_facet<std::moneypunct<wchar_t, false>>(1));
        locale::__register_facet(std::moneypunct<wchar_t, true>::id.__get(),  make_static_facet<std::moneypunct<wchar_t, true>>(1));
        locale::__register_facet(std::messages<char>::id.__get(),    make_static_facet<std::messages<char>>(1));
        locale::__register_facet(std::messages<wchar_t>::id.__get(), make_static_facet<std::messages<wchar_t>>(1));
    }
};
stream_facet_init stream_facet_init_instance;

} // namespace detail
} // namespace std

// ---------------------------------------------------------------------------
// locale::__install_{monetary,time,messages}: install byname facets into a
// named locale's imp.  Defined here (not in <locale>) because constructing the
// byname facets instantiates their ios_base-using virtual bodies, which need a
// complete std::ios_base.
// ---------------------------------------------------------------------------
namespace std {

void locale::__install_monetary(imp* ni, const char* name, bool isC) {
    if (isC) return;
    ni->install(std::moneypunct<char, false>::id.__get(),    new std::moneypunct_byname<char, false>(name));
    ni->install(std::moneypunct<char, true>::id.__get(),     new std::moneypunct_byname<char, true>(name));
    ni->install(std::moneypunct<wchar_t, false>::id.__get(), new std::moneypunct_byname<wchar_t, false>(name));
    ni->install(std::moneypunct<wchar_t, true>::id.__get(),  new std::moneypunct_byname<wchar_t, true>(name));
}
void locale::__install_time(imp* ni, const char* name, bool isC) {
    if (isC) return;
    ni->install(std::time_get<char>::id.__get(),    new std::time_get_byname<char>(name));
    ni->install(std::time_get<wchar_t>::id.__get(), new std::time_get_byname<wchar_t>(name));
    ni->install(std::time_put<char>::id.__get(),    new std::time_put_byname<char>(name));
    ni->install(std::time_put<wchar_t>::id.__get(), new std::time_put_byname<wchar_t>(name));
}
void locale::__install_messages(imp* ni, const char* name, bool isC) {
    if (isC) return;
    ni->install(std::messages<char>::id.__get(),    new std::messages_byname<char>(name));
    ni->install(std::messages<wchar_t>::id.__get(), new std::messages_byname<wchar_t>(name));
}

} // namespace std

// --- libcis: shared size-parameterized shift cores for vector (bits/vector_core.h) ---
#include <cstring>
namespace std {
namespace detail {
void vec_open_gap(void* base, std::size_t n, std::size_t off, std::size_t gap, std::size_t elem) {
    unsigned char* b = static_cast<unsigned char*>(base);
    std::memmove(b + (off + gap) * elem, b + off * elem, (n - off) * elem);
}
void vec_close_gap(void* base, std::size_t n, std::size_t off, std::size_t gap, std::size_t elem) {
    unsigned char* b = static_cast<unsigned char*>(base);
    std::memmove(b + off * elem, b + (off + gap) * elem, (n - off - gap) * elem);
}
} // namespace detail
} // namespace std

// libcis: the const type_ops* relocate core (bits/type_ops.h) — one shared body
// for every element type. memcpy when trivially relocatable; else move-construct
// each element into the new storage and destroy the source.
#include <bits/type_ops.h>
namespace std {
namespace detail {
void core_relocate(void* dst, void* src, std::size_t n, const type_ops& ops, void* ctx) {
    if (ops.move_construct == nullptr) {                 // trivially relocatable
        __builtin_memcpy(dst, src, n * ops.size);
        return;
    }
    unsigned char* d = static_cast<unsigned char*>(dst);
    unsigned char* s = static_cast<unsigned char*>(src);
    for (std::size_t i = 0; i < n; ++i, d += ops.size, s += ops.size) {
        ops.move_construct(ctx, d, s);
        if (ops.destroy) ops.destroy(ctx, s);
    }
}

void* core_grow(void* old_base, std::size_t n_live, std::size_t old_cap,
                std::size_t new_cap, const type_ops& ops, const storage_ops& st,
                void* ctx, std::size_t* out_cap) {
    // ctx is the container; storage callbacks are bound to it.
    if (ops.move_construct == nullptr && st.reallocate) {   // trivially relocatable, reallocable
        *out_cap = new_cap;
        return st.reallocate(ctx, old_base, new_cap);
    }
    std::size_t cap;
    void* p = st.allocate(ctx, new_cap, &cap);
    // Materialize the allocator instance only when the element leaves customize
    // construct/destroy; otherwise the leaves run with a null allocator ctx.
    void* actx = (ops.flags & f_alloc_ctx) ? st.get_alloc(ctx) : nullptr;
    core_relocate(p, old_base, n_live, ops, actx);   // memcpy, or per-element move
    if (old_base) st.deallocate(ctx, old_base, old_cap);
    *out_cap = cap;
    return p;
}
} // namespace detail
} // namespace std
