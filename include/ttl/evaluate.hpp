#pragma once

#include "ttl/FWD.hpp"
#include "ttl/scalar_index.hpp"
#include "ttl/tag_invoke.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/value_type.hpp"
#include <concepts>

namespace ttl
{
    namespace cpos
    {
        struct evaluate
        {
            template <std::integral T>
            constexpr friend auto tag_invoke(evaluate, T t, scalar_index<0>) -> T {
                return t;
            }

            template <std::floating_point T>
            constexpr friend auto tag_invoke(evaluate, T t, scalar_index<0>) -> T {
                return t;
            }

            template <concepts::has_order T>
            constexpr auto operator()(T&& obj, scalar_index<order_v<T>> const& index) const -> decltype(auto)
                requires requires { tag_invoke(*this, FWD(obj), index); }
            {
                return tag_invoke(*this, FWD(obj), index);
            }

            consteval evaluate(cpo) {}
        };
    }

    constexpr cpos::evaluate evaluate(cpo_tag);

    namespace concepts
    {
        template <class T>
        concept has_evaluate = has_order<T> and requires (T t, scalar_index<order_v<T>> index) {
            { ttl::evaluate(t, index) };
        };
    }
}
