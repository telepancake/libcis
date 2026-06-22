// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// The [rand.req.urng] uniform_random_bit_generator concept.
//
// Factored out of <random> (libc++ __random/uniform_random_bit_generator.h) so
// that <algorithm> can constrain ranges::shuffle / ranges::sample on it without
// including <random> -- in libcis <random> includes <algorithm>, so the reverse
// edge would be a cycle. Both headers include this one, so the concept has a
// single definition (no ODR conflict when both meet in one TU).
//===----------------------------------------------------------------------===//

#ifndef _LIBCIS_BITS_UNIFORM_RANDOM_BIT_GENERATOR_H
#define _LIBCIS_BITS_UNIFORM_RANDOM_BIT_GENERATOR_H

#include <concepts>
#include <type_traits>

namespace std {

template<class Gen>
concept uniform_random_bit_generator =
    invocable<Gen&> && unsigned_integral<invoke_result_t<Gen&>> &&
    requires {
        { Gen::min() } -> same_as<invoke_result_t<Gen&>>;
        { Gen::max() } -> same_as<invoke_result_t<Gen&>>;
        requires bool_constant<(Gen::min() < Gen::max())>::value;
    };

} // namespace std

#endif // _LIBCIS_BITS_UNIFORM_RANDOM_BIT_GENERATOR_H
