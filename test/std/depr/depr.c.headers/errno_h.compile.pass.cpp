// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_c_headers_errno_h_ff832e15).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test <errno.h>

#include <errno.h>

#ifndef EDOM
#error EDOM not defined
#endif

#ifndef EILSEQ
#error EILSEQ not defined
#endif

#ifndef ERANGE
#error ERANGE not defined
#endif

#ifndef errno
#error errno not defined
#endif
