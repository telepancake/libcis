// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_dynamic_new_delete_new_delete_single_new_size_nothrow_a80ea2f6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// void* operator new(std::size_t, std::nothrow_t const&);

// asan and msan will not call the new handler.
// UNSUPPORTED: sanitizer-new-delete

#include <new>
#include <cstddef>
#include <cassert>
#include <limits>

#include "test_macros.h"
#include "../types.h"

namespace libcis_ns_language_support_support_dynamic_new_delete_new_delete_single_new_size_nothrow_a80ea2f6 { // libcis
int new_handler_called = 0;

void my_new_handler() {
    ++new_handler_called;
    std::set_new_handler(nullptr);
}

int main(int, char**) {
    // Test that we can call the function directly
    {
        void* x = operator new(10, std::nothrow);
        assert(x != nullptr);
        operator delete(x, std::nothrow);
    }

    // Test that the new handler is called and we return nullptr if allocation fails
    {
        std::set_new_handler(my_new_handler);
        void* x = operator new(std::numeric_limits<std::size_t>::max(), std::nothrow);
        assert(new_handler_called == 1);
        assert(x == nullptr);
    }

    // Test that a new expression constructs the right object
    // and a delete expression deletes it
    {
        LifetimeInformation info;
        TrackLifetime* x = new (std::nothrow) TrackLifetime(info);
        assert(x != nullptr);
        assert(info.address_constructed == x);

        const auto old_x = x;
        delete x;
        assert(info.address_destroyed == old_x);
    }

    return 0;

    return 0;
}
} // libcis_ns_language_support_support_dynamic_new_delete_new_delete_single_new_size_nothrow_a80ea2f6 (libcis)

