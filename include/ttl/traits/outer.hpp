#pragma once

#include "ttl/FWD.hpp"
#include "ttl/index.hpp"
#include "ttl/tag_invoke.hpp"
#include "ttl/concepts/index.hpp"
#include "ttl/concepts/scalar.hpp"
#include "ttl/concepts/tensor.hpp"
#include "ttl/traits/order.hpp"

namespace ttl::traits
{
    template <concepts::tensor>
    struct outer;

    template <concepts::scalar T>
    struct outer<T>
    {
        static constexpr index<0> value{};
    };
}

namespace ttl::concepts
{
    template <class T>
    concept _has_outer_trait = requires {
        { traits::outer<std::remove_cvref_t<T>>::value } -> concepts::index;
    };
}

namespace ttl::cpos
{
    struct outer
    {
        template <concepts::_has_outer_trait T>
        constexpr friend auto tag_invoke(outer, T&&) {
            return traits::outer<std::remove_cvref_t<T>>::value;
        }

        constexpr auto operator()(auto&& obj) const -> concepts::index auto
            requires requires { tag_invoke(*this, FWD(obj)); }
        {
            return tag_invoke(*this, FWD(obj));
        }

        consteval outer(cpo) {}
    };
}

namespace ttl
{
    constexpr cpos::outer outer{cpo_tag};
}

namespace ttl::concepts
{
    template <class T>
    concept has_outer = requires (T t) {
        { ttl::outer(t) } -> concepts::index;
    };

    template <class T>
    concept has_static_outer = has_outer<T> and _has_outer_trait<T>;
}

namespace ttl
{
    template <concepts::has_static_outer T>
    constexpr concepts::index auto outer_v = traits::outer<std::remove_cvref_t<T>>::value;
}
