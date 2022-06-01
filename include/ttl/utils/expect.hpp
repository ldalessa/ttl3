#ifndef expect
#include "ttl/utils/error.hpp"
#define expect(...) (__builtin_expect((__VA_ARGS__), 1) || utils::error(#__VA_ARGS__))
#endif
