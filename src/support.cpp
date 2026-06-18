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
#include <string.h>    // memcpy / memmove for the trivial-relocation cores
#include <stdint.h>    // int64_t, uint8_t
#include <sched.h>     // sched_yield — for guard spin loop
#include <bits/vector_cores.h>

// ---------------------------------------------------------------------------
// Container cores — non-template bodies that vector/string/etc. forward into.
// Each is keyed by a const type_ops* (element leaves) and, for grow, a
// const storage_ops* (buffer state vocabulary) + void* st_ctx + void* el_ctx.
// ---------------------------------------------------------------------------
namespace std {
namespace detail {

// Trivial-fast-path predicates: BOTH the type is trivially X AND the
// allocator has default lifecycle (so memcpy/memset/skip is semantically
// equivalent to calling the leaf).
static inline bool triv_destroy(const type_ops* ops) {
    return (ops->flags & f_triv_destroy) && (ops->flags & f_alloc_default_life);
}
static inline bool triv_default(const type_ops* ops) {
    return (ops->flags & f_triv_default) && (ops->flags & f_alloc_default_life);
}
static inline bool triv_copy(const type_ops* ops) {
    return (ops->flags & f_triv_copy) && (ops->flags & f_alloc_default_life);
}
static inline bool triv_reloc(const type_ops* ops) {
    return (ops->flags & f_triv_reloc) && (ops->flags & f_alloc_default_life);
}

// Fetch the element ctx lazily: only when the type_ops flag says the
// allocator has per-instance state, ask storage_ops for &alloc_. Stateless
// allocators (std::allocator, min_allocator, ...) never trigger the slot.
static inline void* fetch_ec(const type_ops* ops, const storage_ops* sops,
                             void* st_ctx) {
    return (ops->flags & f_alloc_ctx) ? sops->alloc_ctx(st_ctx) : nullptr;
}

void destroy_range(const type_ops* ops, const storage_ops* sops,
                   void* st_ctx, void* begin, void* end) {
    if (begin > end) __builtin_trap();
    if (triv_destroy(ops)) return;
    void* el_ctx = fetch_ec(ops, sops, st_ctx);
    const size_t sz = ops->size;
    auto p = static_cast<unsigned char*>(end);
    auto q = static_cast<unsigned char*>(begin);
    while (p != q) {
        p -= sz;
        ops->destroy(el_ctx, p);
    }
}

void construct_default_n(const type_ops* ops, const storage_ops* sops,
                         void* st_ctx, void* dst, size_t n) {
    if (triv_default(ops)) {
        __builtin_memset(dst, 0, n * ops->size);
        return;
    }
    void* el_ctx = fetch_ec(ops, sops, st_ctx);
    const size_t sz = ops->size;
    auto p = static_cast<unsigned char*>(dst);
    for (size_t i = 0; i < n; ++i, p += sz)
        ops->default_construct(el_ctx, p);
}

void construct_copy_one_n(const type_ops* ops, const storage_ops* sops,
                          void* st_ctx, void* dst, const void* src, size_t n) {
    const size_t sz = ops->size;
    auto p = static_cast<unsigned char*>(dst);
    if (triv_copy(ops)) {
        for (size_t i = 0; i < n; ++i, p += sz)
            __builtin_memcpy(p, src, sz);
        return;
    }
    void* el_ctx = fetch_ec(ops, sops, st_ctx);
    for (size_t i = 0; i < n; ++i, p += sz)
        ops->copy_construct(el_ctx, p, src);
}

// Relocate every element of [src, src+size_bytes) into [dst, ...). Iterates
// forward — safe for non-overlapping ranges or for shift-DOWN (dst < src).
// Source elements are destroyed once moved.
static void relocate_block(const type_ops* ops, void* el_ctx,
                           void* dst, void* src, size_t size_bytes) {
    if (triv_reloc(ops)) {
        // triv-reloc + default lifecycle: memmove handles both overlap cases.
        __builtin_memmove(dst, src, size_bytes);
        return;
    }
    const size_t sz = ops->size;
    auto* d = static_cast<unsigned char*>(dst);
    auto* s = static_cast<unsigned char*>(src);
    const size_t n = size_bytes / sz;
    for (size_t i = 0; i < n; ++i, d += sz, s += sz) {
        ops->move_construct(el_ctx, d, s);
        if (!triv_destroy(ops)) ops->destroy(el_ctx, s);
    }
}

// Same, but iterates BACKWARD — required for the leaf-path shift-UP
// (dst > src) where the source range overlaps with the destination range
// from above.
static void relocate_block_rev(const type_ops* ops, void* el_ctx,
                               void* dst, void* src, size_t size_bytes) {
    if (triv_reloc(ops)) {
        __builtin_memmove(dst, src, size_bytes);
        return;
    }
    const size_t sz = ops->size;
    auto* d = static_cast<unsigned char*>(dst) + size_bytes;
    auto* s = static_cast<unsigned char*>(src) + size_bytes;
    const size_t n = size_bytes / sz;
    for (size_t i = 0; i < n; ++i) {
        d -= sz; s -= sz;
        ops->move_construct(el_ctx, d, s);
        if (!triv_destroy(ops)) ops->destroy(el_ctx, s);
    }
}

// Public wrapper: storage_ops::resize implementations call this to relocate
// live bytes from an old buffer to a freshly-allocated new one.
void relocate_live(const type_ops* ops, void* el_ctx,
                   unsigned char* dst, unsigned char* src, size_t live_bytes) {
    relocate_block(ops, el_ctx, dst, src, live_bytes);
}

void grow(const type_ops* tops, const storage_ops* sops,
          void* st_ctx, size_t min_bytes) {
    // resize fetches el_ctx internally via sops->alloc_ctx if needed.
    sops->resize(tops, st_ctx, min_bytes);
}

// ---------------------------------------------------------------------------
// rotate: standard std::rotate semantics, type-erased.
//
// Rearranges [first, last) so that *middle becomes the new *first and
// *(middle - 1) becomes the new *(last - 1). Pre: every slot in [first, last)
// is constructed.
//
// Algorithm: block-swap via scratch. Save the smaller block to scratch,
// shift the larger block to its new position (memmove or leaf-loop), then
// restore scratch to the other end.
//
// For trivially-relocatable + default-lifecycle T: pure byte ops. Scratch
// allocation uses ::malloc for blocks > 256 bytes (the common insert-by-one
// case has a 1-element block so the stack alternative covers it).
// ---------------------------------------------------------------------------
void rotate(const type_ops* ops, const storage_ops* sops, void* st_ctx,
            void* first, void* middle, void* last,
            ptrdiff_t size_delta_bytes) {
    auto* f = static_cast<unsigned char*>(first);
    auto* m = static_cast<unsigned char*>(middle);
    auto* l = static_cast<unsigned char*>(last);
    if (f > m || m > l) __builtin_trap();
    if (f != m && m != l) {
        const size_t left  = static_cast<size_t>(m - f);
        const size_t right = static_cast<size_t>(l - m);
        const size_t small = left <= right ? left : right;

        constexpr size_t kStack = 256;
        alignas(::max_align_t) unsigned char stack_scratch[kStack];
        void* scratch = small <= kStack ? static_cast<void*>(stack_scratch)
                                        : ::malloc(small);

        if (triv_reloc(ops)) {
            if (left <= right) {
                __builtin_memcpy(scratch, f, left);
                __builtin_memmove(f, m, right);
                __builtin_memcpy(f + right, scratch, left);
            } else {
                __builtin_memcpy(scratch, m, right);
                __builtin_memmove(f + right, f, left);
                __builtin_memcpy(f, scratch, right);
            }
        } else {
            void* el_ctx = fetch_ec(ops, sops, st_ctx);
            if (left <= right) {
                relocate_block    (ops, el_ctx, scratch, f, left);
                relocate_block    (ops, el_ctx, f, m, right);
                relocate_block    (ops, el_ctx, f + right, scratch, left);
            } else {
                relocate_block    (ops, el_ctx, scratch, m, right);
                relocate_block_rev(ops, el_ctx, f + right, f, left);
                relocate_block    (ops, el_ctx, f, scratch, right);
            }
        }

        if (small > kStack) ::free(scratch);
    }

    if (size_delta_bytes != 0) {
        if (size_delta_bytes < 0) {
            unsigned char* db = l + size_delta_bytes;
            if (!triv_destroy(ops))
                destroy_range(ops, sops, st_ctx, db, l);
        }
        byte_span data = sops->data(st_ctx);
        sops->set_size(st_ctx, data.size + size_delta_bytes);
    }
}

} // namespace detail
} // namespace std

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
