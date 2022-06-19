#pragma once

#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept shape = requires (T t, int i) {
        { t.size() } -> std::convertible_to<int>;
        { t[i] } -> std::convertible_to<int>;
    };
}
