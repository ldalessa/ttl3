#pragma once

#include "ttl/utils/FWD.hpp"

namespace ttl::utils
{
    struct _min_fn
    {
        constexpr auto operator()(auto&& a, auto&& b) const -> decltype(auto)
        {
            return (a < b) ? a : b;
        }

        consteval _min_fn(int) {};
    };

    inline constexpr _min_fn min{0};

    struct _max_fn
    {
        constexpr auto operator()(auto&& a) const
        {
            return FWD(a);
        }

        constexpr auto operator()(auto&& a, auto&& b, auto&&... c) const
        {
            auto d = (*this)(FWD(b), FWD(c)...);
            return (a < d) ? d : a;
        }

        consteval _max_fn(int) {};
    };

    inline constexpr _max_fn max{0};

    struct _plus_fn
    {
        constexpr auto operator()(auto&& a, auto&&... b) const -> decltype(auto)
        {
            return (FWD(a) + ... + FWD(b));
        }

        consteval _plus_fn(int) {}
    };

    inline constexpr _plus_fn plus{0};

    struct _minus_fn
    {
        constexpr auto operator()(auto&& a, auto&&... b) const -> decltype(auto)
        {
            return (FWD(a) - ... - FWD(b));
        }

        consteval _minus_fn(int) {}
    };

    inline constexpr _minus_fn minus{0};

    struct _multiply_fn
    {
        constexpr auto operator()(auto&& a, auto&&... b) const -> decltype(auto)
        {
            return (FWD(a) * ... * FWD(b));
        }

        consteval _multiply_fn(int) {}
    };

    inline constexpr _multiply_fn multiply{0};

}
