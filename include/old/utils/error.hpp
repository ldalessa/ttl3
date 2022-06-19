#pragma once

#include <cstdio>

namespace ttl::utils
{
    [[gnu::noreturn]]
    auto error(char const *s) -> bool {
        std::puts(s);
        __builtin_abort();
    }
}
