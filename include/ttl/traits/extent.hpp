#pragma once

#include <concepts>

namespace ttl::traits
{
    template <int M, class T>
    struct extent
    {
        static constexpr struct{} value{};
    };

    template <int M, class T>
    inline constexpr auto extent_v = extent<M, std::remove_cvref_t<T>>::value;
}
