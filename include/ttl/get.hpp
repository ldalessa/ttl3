#pragma once

#include "ttl/FWD.hpp"
#include "ttl/tag_invoke.hpp"
#include <kumi/tuple.hpp>

namespace ttl::cpos
{
    template <int i>
    struct get
    {
        constexpr friend auto tag_invoke(get, auto&& obj) -> decltype(auto)
            requires requires { std::get<i>(FWD(obj)); }
        {
            return std::get<i>(FWD(obj));
        }

        constexpr friend auto tag_invoke(get, auto&& obj) -> decltype(auto)
            requires requires { kumi::get<i>(FWD(obj)); }
        {
            return kumi::get<i>(FWD(obj));
        }

        constexpr auto operator()(auto&& obj) const -> decltype(auto) {
            return tag_invoke(*this, FWD(obj));
        }

        consteval get(cpo) {}
    };
}

namespace ttl
{
    template <int i>
    constexpr cpos::get<i> get{cpo_tag};
}
