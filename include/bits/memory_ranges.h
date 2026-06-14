// libcis: std::ranges algorithms of <memory> ([specialized.algorithms] ranges
// versions): ranges::construct_at / destroy / destroy_at / destroy_n and the
// ranges::uninitialized_* family.  Included from <memory> only.
//
// Ported from libc++ include/__memory/ranges_construct_at.h,
// __memory/ranges_destroy.h and __memory/ranges_uninitialized_algorithms.h,
// with the exposition-only [special.mem.concepts] concepts from
// __memory/concepts.h and the backend loops from
// __memory/uninitialized_algorithms.h.
#pragma once

#include <ranges>     // range concepts, iterator_t, borrowed_iterator_t, ranges::begin/end
#include <algorithm>  // ranges::in_out_result

namespace std {
namespace ranges {

//===----------------------------------------------------------------------===//
// [special.mem.concepts] exposition-only concepts
// (from libc++ __memory/concepts.h)
//===----------------------------------------------------------------------===//

namespace detail {

    // This concept ensures that uninitialized algorithms can construct an
    // object at the address pointed-to by the iterator, which requires an
    // lvalue.  (libc++ __memory/concepts.h: __nothrow_input_iterator)
    template<class I>
    concept nothrow_input_iterator =
        input_iterator<I> &&
        is_lvalue_reference_v<iter_reference_t<I>> &&
        same_as<remove_cvref_t<iter_reference_t<I>>, iter_value_t<I>>;

    // libc++ __memory/concepts.h: __nothrow_sentinel_for
    template<class S, class I>
    concept nothrow_sentinel_for = sentinel_for<S, I>;

    // libc++ __memory/concepts.h: __nothrow_input_range
    template<class R>
    concept nothrow_input_range =
        range<R> &&
        nothrow_input_iterator<iterator_t<R>> &&
        nothrow_sentinel_for<sentinel_t<R>, iterator_t<R>>;

    // libc++ __memory/concepts.h: __nothrow_forward_iterator
    template<class I>
    concept nothrow_forward_iterator =
        nothrow_input_iterator<I> &&
        forward_iterator<I> &&
        nothrow_sentinel_for<I, I>;

    // libc++ __memory/concepts.h: __nothrow_forward_range
    template<class R>
    concept nothrow_forward_range =
        nothrow_input_range<R> && nothrow_forward_iterator<iterator_t<R>>;

} // namespace detail

//===----------------------------------------------------------------------===//
// Backend loops shared by the iterator/sentinel and counted forms.
// (from libc++ __memory/uninitialized_algorithms.h: __uninitialized_copy,
// __uninitialized_copy_n, __uninitialized_fill, __uninitialized_fill_n,
// __uninitialized_default_construct(_n), __uninitialized_value_construct(_n),
// __uninitialized_move(_n) and __memory/destroy.h: __destroy)
//
// libc++ rolls back partially-constructed output with __exception_guard; under
// -fno-exceptions a constructor cannot exit by throwing, so the guard is a
// no-op and is dropped here — exactly like the non-ranges uninitialized_*
// algorithms in this project's <memory>.
//===----------------------------------------------------------------------===//

namespace detail {

    // __memory/destroy.h: std::__destroy (returns the end iterator, unlike
    // C++17 std::destroy which returns void)
    template<class I, class S>
    constexpr I destroy_impl(I first, S last) noexcept {
        for (; first != last; ++first)
            std::destroy_at(std::addressof(*first));
        return first;
    }

    // __memory/uninitialized_algorithms.h: std::__uninitialized_copy
    template<class V, class I, class S1, class O, class EndPredicate>
    in_out_result<I, O> uninitialized_copy_impl(I ifirst, S1 ilast, O ofirst, EndPredicate stop_copying) {
        O idx = ofirst;
        for (; ifirst != ilast && !stop_copying(idx); ++ifirst, (void)++idx)
            ::new (static_cast<void*>(std::addressof(*idx))) V(*ifirst);
        return {std::move(ifirst), std::move(idx)};
    }

    // __memory/uninitialized_algorithms.h: std::__uninitialized_copy_n
    template<class V, class I, class Size, class O, class EndPredicate>
    in_out_result<I, O> uninitialized_copy_n_impl(I ifirst, Size n, O ofirst, EndPredicate stop_copying) {
        O idx = ofirst;
        for (; n > 0 && !stop_copying(idx); ++ifirst, (void)++idx, (void)--n)
            ::new (static_cast<void*>(std::addressof(*idx))) V(*ifirst);
        return {std::move(ifirst), std::move(idx)};
    }

    // __memory/uninitialized_algorithms.h: std::__uninitialized_fill
    template<class V, class I, class S, class T>
    I uninitialized_fill_impl(I first, S last, const T& x) {
        I idx = first;
        for (; idx != last; ++idx)
            ::new (static_cast<void*>(std::addressof(*idx))) V(x);
        return idx;
    }

    // __memory/uninitialized_algorithms.h: std::__uninitialized_fill_n
    template<class V, class I, class Size, class T>
    I uninitialized_fill_n_impl(I first, Size n, const T& x) {
        I idx = first;
        for (; n > 0; ++idx, (void)--n)
            ::new (static_cast<void*>(std::addressof(*idx))) V(x);
        return idx;
    }

    // __memory/uninitialized_algorithms.h: std::__uninitialized_default_construct
    template<class V, class I, class S>
    I uninitialized_default_construct_impl(I first, S last) {
        auto idx = first;
        for (; idx != last; ++idx)
            ::new (static_cast<void*>(std::addressof(*idx))) V;
        return idx;
    }

    // __memory/uninitialized_algorithms.h: std::__uninitialized_default_construct_n
    template<class V, class I, class Size>
    I uninitialized_default_construct_n_impl(I first, Size n) {
        auto idx = first;
        for (; n > 0; ++idx, (void)--n)
            ::new (static_cast<void*>(std::addressof(*idx))) V;
        return idx;
    }

    // __memory/uninitialized_algorithms.h: std::__uninitialized_value_construct
    template<class V, class I, class S>
    I uninitialized_value_construct_impl(I first, S last) {
        auto idx = first;
        for (; idx != last; ++idx)
            ::new (static_cast<void*>(std::addressof(*idx))) V();
        return idx;
    }

    // __memory/uninitialized_algorithms.h: std::__uninitialized_value_construct_n
    template<class V, class I, class Size>
    I uninitialized_value_construct_n_impl(I first, Size n) {
        auto idx = first;
        for (; n > 0; ++idx, (void)--n)
            ::new (static_cast<void*>(std::addressof(*idx))) V();
        return idx;
    }

    // ranges::iter_move dispatch used by uninitialized_move(_n), re-ported
    // from libc++ __iterator/iter_move.h.
    //
    // The iter_move CPO in this project's <iterator> never reaches an ADL
    // iter_move under g++-10.2: its poison pill is `void iter_move() = delete;`
    // and gcc-10 wrongly rejects an ADL call in a requires-expression whenever
    // unqualified lookup finds a deleted function (even a non-viable one).
    // libstdc++'s bits/iterator_concepts.h uses a non-deleted
    // `void iter_move();` pill for exactly this reason; do the same here and
    // fall back to the <iterator> CPO when there is no ADL candidate.
    namespace iter_move_adl {

        void iter_move(); // poison pill (NOT deleted — see comment above)

        // libc++ __iterator/iter_move.h: __unqualified_iter_move
        template<class T>
        concept unqualified_iter_move =
            (is_class_v<remove_cvref_t<T>> || is_enum_v<remove_cvref_t<T>> ||
             is_union_v<remove_cvref_t<T>>) &&
            requires(T&& t) { iter_move(static_cast<T&&>(t)); };

        struct fn {
            template<class I>
            constexpr decltype(auto) operator()(I&& i) const {
                if constexpr (unqualified_iter_move<I>)
                    return iter_move(static_cast<I&&>(i));
                else
                    return ranges::iter_move(static_cast<I&&>(i)); // <iterator> CPO
            }
        };

    } // namespace iter_move_adl

    inline constexpr iter_move_adl::fn iter_move{};

    // __memory/uninitialized_algorithms.h: std::__uninitialized_move
    template<class V, class I, class S1, class O, class EndPredicate, class IterMove>
    in_out_result<I, O> uninitialized_move_impl(I ifirst, S1 ilast, O ofirst, EndPredicate stop_moving,
                                                IterMove iter_move) {
        auto idx = ofirst;
        for (; ifirst != ilast && !stop_moving(idx); ++idx, (void)++ifirst)
            ::new (static_cast<void*>(std::addressof(*idx))) V(iter_move(ifirst));
        return {std::move(ifirst), std::move(idx)};
    }

    // __memory/uninitialized_algorithms.h: std::__uninitialized_move_n
    template<class V, class I, class Size, class O, class EndPredicate, class IterMove>
    in_out_result<I, O> uninitialized_move_n_impl(I ifirst, Size n, O ofirst, EndPredicate stop_moving,
                                                  IterMove iter_move) {
        auto idx = ofirst;
        for (; n > 0 && !stop_moving(idx); ++idx, (void)++ifirst, --n)
            ::new (static_cast<void*>(std::addressof(*idx))) V(iter_move(ifirst));
        return {std::move(ifirst), std::move(idx)};
    }

} // namespace detail

//===----------------------------------------------------------------------===//
// ranges::construct_at / ranges::destroy_at
// (from libc++ __memory/ranges_construct_at.h)
//===----------------------------------------------------------------------===//

struct construct_at_fn {
    // gcc-10 only blesses constexpr placement new inside the function
    // literally named std::construct_at, so this MUST delegate to it
    // (see the std::construct_at comment in <memory>).
    template<class T, class... Args,
             class = decltype(::new (declval<void*>()) T(declval<Args>()...))>
    constexpr T* operator()(T* location, Args&&... args) const {
        return std::construct_at(location, std::forward<Args>(args)...);
    }
};

inline constexpr construct_at_fn construct_at{};

struct destroy_at_fn {
    template<destructible T>
    constexpr void operator()(T* location) const noexcept {
        std::destroy_at(location);
    }
};

inline constexpr destroy_at_fn destroy_at{};

//===----------------------------------------------------------------------===//
// ranges::destroy / ranges::destroy_n
// (from libc++ __memory/ranges_destroy.h)
//===----------------------------------------------------------------------===//

struct destroy_fn {
    template<detail::nothrow_input_iterator I, detail::nothrow_sentinel_for<I> S>
        requires destructible<iter_value_t<I>>
    constexpr I operator()(I first, S last) const noexcept {
        return detail::destroy_impl(std::move(first), std::move(last));
    }

    template<detail::nothrow_input_range R>
        requires destructible<range_value_t<R>>
    constexpr borrowed_iterator_t<R> operator()(R&& r) const noexcept {
        return (*this)(ranges::begin(r), ranges::end(r));
    }
};

inline constexpr destroy_fn destroy{};

struct destroy_n_fn {
    template<detail::nothrow_input_iterator I>
        requires destructible<iter_value_t<I>>
    constexpr I operator()(I first, iter_difference_t<I> n) const noexcept {
        return std::destroy_n(std::move(first), n);
    }
};

inline constexpr destroy_n_fn destroy_n{};

//===----------------------------------------------------------------------===//
// ranges::uninitialized_default_construct(_n)
// (from libc++ __memory/ranges_uninitialized_algorithms.h)
//===----------------------------------------------------------------------===//

struct uninitialized_default_construct_fn {
    template<detail::nothrow_forward_iterator I, detail::nothrow_sentinel_for<I> S>
        requires default_initializable<iter_value_t<I>>
    I operator()(I first, S last) const {
        using V = remove_reference_t<iter_reference_t<I>>;
        return detail::uninitialized_default_construct_impl<V>(std::move(first), std::move(last));
    }

    template<detail::nothrow_forward_range R>
        requires default_initializable<range_value_t<R>>
    borrowed_iterator_t<R> operator()(R&& r) const {
        return (*this)(ranges::begin(r), ranges::end(r));
    }
};

inline constexpr uninitialized_default_construct_fn uninitialized_default_construct{};

struct uninitialized_default_construct_n_fn {
    template<detail::nothrow_forward_iterator I>
        requires default_initializable<iter_value_t<I>>
    I operator()(I first, iter_difference_t<I> n) const {
        using V = remove_reference_t<iter_reference_t<I>>;
        return detail::uninitialized_default_construct_n_impl<V>(std::move(first), n);
    }
};

inline constexpr uninitialized_default_construct_n_fn uninitialized_default_construct_n{};

//===----------------------------------------------------------------------===//
// ranges::uninitialized_value_construct(_n)
// (from libc++ __memory/ranges_uninitialized_algorithms.h)
//===----------------------------------------------------------------------===//

struct uninitialized_value_construct_fn {
    template<detail::nothrow_forward_iterator I, detail::nothrow_sentinel_for<I> S>
        requires default_initializable<iter_value_t<I>>
    I operator()(I first, S last) const {
        using V = remove_reference_t<iter_reference_t<I>>;
        return detail::uninitialized_value_construct_impl<V>(std::move(first), std::move(last));
    }

    template<detail::nothrow_forward_range R>
        requires default_initializable<range_value_t<R>>
    borrowed_iterator_t<R> operator()(R&& r) const {
        return (*this)(ranges::begin(r), ranges::end(r));
    }
};

inline constexpr uninitialized_value_construct_fn uninitialized_value_construct{};

struct uninitialized_value_construct_n_fn {
    template<detail::nothrow_forward_iterator I>
        requires default_initializable<iter_value_t<I>>
    I operator()(I first, iter_difference_t<I> n) const {
        using V = remove_reference_t<iter_reference_t<I>>;
        return detail::uninitialized_value_construct_n_impl<V>(std::move(first), n);
    }
};

inline constexpr uninitialized_value_construct_n_fn uninitialized_value_construct_n{};

//===----------------------------------------------------------------------===//
// ranges::uninitialized_fill(_n)
// (from libc++ __memory/ranges_uninitialized_algorithms.h)
//===----------------------------------------------------------------------===//

struct uninitialized_fill_fn {
    template<detail::nothrow_forward_iterator I, detail::nothrow_sentinel_for<I> S, class T>
        requires constructible_from<iter_value_t<I>, const T&>
    I operator()(I first, S last, const T& x) const {
        using V = remove_reference_t<iter_reference_t<I>>;
        return detail::uninitialized_fill_impl<V>(std::move(first), std::move(last), x);
    }

    template<detail::nothrow_forward_range R, class T>
        requires constructible_from<range_value_t<R>, const T&>
    borrowed_iterator_t<R> operator()(R&& r, const T& x) const {
        return (*this)(ranges::begin(r), ranges::end(r), x);
    }
};

inline constexpr uninitialized_fill_fn uninitialized_fill{};

struct uninitialized_fill_n_fn {
    template<detail::nothrow_forward_iterator I, class T>
        requires constructible_from<iter_value_t<I>, const T&>
    I operator()(I first, iter_difference_t<I> n, const T& x) const {
        using V = remove_reference_t<iter_reference_t<I>>;
        return detail::uninitialized_fill_n_impl<V>(std::move(first), n, x);
    }
};

inline constexpr uninitialized_fill_n_fn uninitialized_fill_n{};

//===----------------------------------------------------------------------===//
// ranges::uninitialized_copy(_n)
// (from libc++ __memory/ranges_uninitialized_algorithms.h)
//===----------------------------------------------------------------------===//

template<class I, class O>
using uninitialized_copy_result = in_out_result<I, O>;

struct uninitialized_copy_fn {
    template<input_iterator I, sentinel_for<I> S1,
             detail::nothrow_forward_iterator O, detail::nothrow_sentinel_for<O> S2>
        requires constructible_from<iter_value_t<O>, iter_reference_t<I>>
    uninitialized_copy_result<I, O> operator()(I ifirst, S1 ilast, O ofirst, S2 olast) const {
        using V = remove_reference_t<iter_reference_t<O>>;
        auto stop_copying = [&olast](auto&& out_iter) -> bool { return out_iter == olast; };
        return detail::uninitialized_copy_impl<V>(
            std::move(ifirst), std::move(ilast), std::move(ofirst), stop_copying);
    }

    template<input_range IR, detail::nothrow_forward_range OR>
        requires constructible_from<range_value_t<OR>, range_reference_t<IR>>
    uninitialized_copy_result<borrowed_iterator_t<IR>, borrowed_iterator_t<OR>>
    operator()(IR&& in_range, OR&& out_range) const {
        return (*this)(ranges::begin(in_range), ranges::end(in_range),
                       ranges::begin(out_range), ranges::end(out_range));
    }
};

inline constexpr uninitialized_copy_fn uninitialized_copy{};

template<class I, class O>
using uninitialized_copy_n_result = in_out_result<I, O>;

struct uninitialized_copy_n_fn {
    template<input_iterator I,
             detail::nothrow_forward_iterator O, detail::nothrow_sentinel_for<O> S>
        requires constructible_from<iter_value_t<O>, iter_reference_t<I>>
    uninitialized_copy_n_result<I, O>
    operator()(I ifirst, iter_difference_t<I> n, O ofirst, S olast) const {
        using V = remove_reference_t<iter_reference_t<O>>;
        auto stop_copying = [&olast](auto&& out_iter) -> bool { return out_iter == olast; };
        return detail::uninitialized_copy_n_impl<V>(std::move(ifirst), n, std::move(ofirst), stop_copying);
    }
};

inline constexpr uninitialized_copy_n_fn uninitialized_copy_n{};

//===----------------------------------------------------------------------===//
// ranges::uninitialized_move(_n)
// (from libc++ __memory/ranges_uninitialized_algorithms.h)
//===----------------------------------------------------------------------===//

template<class I, class O>
using uninitialized_move_result = in_out_result<I, O>;

struct uninitialized_move_fn {
    template<input_iterator I, sentinel_for<I> S1,
             detail::nothrow_forward_iterator O, detail::nothrow_sentinel_for<O> S2>
        requires constructible_from<iter_value_t<O>, iter_rvalue_reference_t<I>>
    uninitialized_move_result<I, O> operator()(I ifirst, S1 ilast, O ofirst, S2 olast) const {
        using V = remove_reference_t<iter_reference_t<O>>;
        auto iter_move   = [](auto&& iter) -> decltype(auto) { return detail::iter_move(iter); };
        auto stop_moving = [&olast](auto&& out_iter) -> bool { return out_iter == olast; };
        return detail::uninitialized_move_impl<V>(
            std::move(ifirst), std::move(ilast), std::move(ofirst), stop_moving, iter_move);
    }

    template<input_range IR, detail::nothrow_forward_range OR>
        requires constructible_from<range_value_t<OR>, range_rvalue_reference_t<IR>>
    uninitialized_move_result<borrowed_iterator_t<IR>, borrowed_iterator_t<OR>>
    operator()(IR&& in_range, OR&& out_range) const {
        return (*this)(ranges::begin(in_range), ranges::end(in_range),
                       ranges::begin(out_range), ranges::end(out_range));
    }
};

inline constexpr uninitialized_move_fn uninitialized_move{};

template<class I, class O>
using uninitialized_move_n_result = in_out_result<I, O>;

struct uninitialized_move_n_fn {
    template<input_iterator I,
             detail::nothrow_forward_iterator O, detail::nothrow_sentinel_for<O> S>
        requires constructible_from<iter_value_t<O>, iter_rvalue_reference_t<I>>
    uninitialized_move_n_result<I, O>
    operator()(I ifirst, iter_difference_t<I> n, O ofirst, S olast) const {
        using V = remove_reference_t<iter_reference_t<O>>;
        auto iter_move   = [](auto&& iter) -> decltype(auto) { return detail::iter_move(iter); };
        auto stop_moving = [&olast](auto&& out_iter) -> bool { return out_iter == olast; };
        return detail::uninitialized_move_n_impl<V>(std::move(ifirst), n, std::move(ofirst), stop_moving, iter_move);
    }
};

inline constexpr uninitialized_move_n_fn uninitialized_move_n{};

} // namespace ranges
} // namespace std
