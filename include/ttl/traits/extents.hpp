#pragma once

#include "ttl/FWD.hpp"
#include "ttl/tag_invoke.hpp"
#include "ttl/concepts/shape.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/tensor.hpp"
#include <array>

namespace ttl::traits
{
    template <class T>
    struct extents;

    template <concepts::_has_tensor_extents_trait T>
    struct extents<T>
    {
        static constexpr concepts::shape auto value = traits::tensor<std::remove_cvref_t<T>>::extents;
    };

    template <class T>
    requires concepts::has_order_zero<T> and (not concepts::_has_tensor_extents_trait<T>)
    struct extents<T>
    {
        static constexpr std::array<int, 0> value{};
    };
}

namespace ttl::concepts
{
    template <class T>
    concept _has_extents_trait = requires {
        { traits::extents<std::remove_cvref_t<T>>::value } -> concepts::shape;
    };
}

namespace ttl::cpos
{
    struct extents
    {
        template <concepts::_has_extents_trait T>
        constexpr friend auto tag_invoke(extents, T&&) -> concepts::shape auto {
            return traits::extents<std::remove_cvref_t<T>>::value;
        }

        template <class T>
        requires concepts::_has_tensor_extents_call<T> and (not concepts::_has_extents_trait<T>)
        constexpr friend auto tag_invoke(extents, T&& t) -> concepts::shape auto {
            return traits::tensor<std::remove_cvref_t<T>>::extents(FWD(t));
        }

        constexpr auto operator()(auto&& obj) const -> concepts::shape auto
            requires requires { tag_invoke(*this, FWD(obj)); }
        {
            return tag_invoke(*this, FWD(obj));
        }

        consteval extents(cpo) {}
    };
}

namespace ttl
{
    constexpr cpos::extents extents{cpo_tag};
}

namespace ttl::concepts
{
    template <class T>
    concept has_extents = requires (T t) {
        { ttl::extents(t) } -> concepts::shape;
    };

    template <class T>
    concept has_static_extents = has_extents<T> and _has_extents_trait<T>;
}

namespace ttl
{
    template <concepts::has_static_extents T>
    constexpr concepts::shape auto extents_v = traits::extents<std::remove_cvref_t<T>>::value;
}
