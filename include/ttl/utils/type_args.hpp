#pragma once

namespace ttl::utils
{
    template <class...>
    struct type_args {};

    template <class... Ts>
    inline constexpr type_args<Ts...> types = {};
}
