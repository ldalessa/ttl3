#pragma once

#include <concepts>
#include <utility>

namespace ttl::utils
{
    template <std::integral auto...>
    struct sequence {};

    template <std::integral auto N>
    inline constexpr auto sequence_v = []<std::size_t... is>(std::index_sequence<is...>) {
        return sequence<decltype(N)(is)...>();
    }(std::make_index_sequence<N>());
}
