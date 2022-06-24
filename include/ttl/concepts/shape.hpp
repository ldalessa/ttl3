#pragma once

#include <concepts>
#include <ranges>

namespace ttl::concepts
{
    template <class T>
    concept shape =
        std::default_initializable<std::remove_cvref_t<T>>
        and std::ranges::range<std::remove_cvref_t<T>>
        and requires (T t, int i) {
            { t.size() } -> std::convertible_to<int>;
            { t[i] } -> std::convertible_to<int>;
        };
}
