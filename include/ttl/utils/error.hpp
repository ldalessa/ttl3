#pragma once

#include <cstdio>

namespace ttl::utils
{
    auto error(char const *s) -> bool {
        std::puts(s);
        __builtin_unreachable();
        return false;
    }
}
