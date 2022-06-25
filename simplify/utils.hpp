#pragma once

#ifndef FWD
#define FWD(x) static_cast<decltype(x)&&>(x)
#endif

namespace ttl
{
    template <auto...>
    struct nttp_args{};

    template <auto... args>
    constexpr nttp_args<args...> nttp{};

    [[deprecated]]
    void debug_print_types(auto...) {}
}
