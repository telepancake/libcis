// <__utility/private_constructor_tag.h> — libc++ compatibility shim.
//
// libc++ uses std::__private_constructor_tag as an access-restricted tag so
// that test code (and internal code) can construct certain library types whose
// public constructors are intentionally absent. libcis folds the __utility/*
// fragments into <utility>, but the conformance test support headers include
// this path directly (guarded by _LIBCPP_VERSION), so provide it here.
#pragma once

namespace std {
struct __private_constructor_tag {};
} // namespace std
