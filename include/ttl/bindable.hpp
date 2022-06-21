#pragma once

#include "ttl/FWD.hpp"
#include "ttl/evaluate.hpp"
#include "ttl/scalar_index.hpp"
#include "ttl/tensor_index.hpp"
#include "ttl/concepts/index.hpp"
#include "ttl/expressions/bind.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/value_type.hpp"

namespace ttl
{
    template <concepts::has_order>
    struct bindable;

    namespace traits
    {
        template <concepts::has_order T>
        struct order<bindable<T>>
        {
            static constexpr int value = order_v<T>;
        };

        template <concepts::has_order T>
        struct value_type<bindable<T>>
        {
            using type = value_type_t<T>;
        };
    }

    template <concepts::has_order T>
    struct bindable
    {
        constexpr auto operator()(concepts::index_or_integral auto... is) const& -> decltype(auto)
        {
            return _bind(_derived(), is...);
        }

        constexpr auto operator()(concepts::index_or_integral auto... is) && -> decltype(auto)
        {
            return _bind(_derived(), is...);
        }

        constexpr auto operator()(concepts::index_or_integral auto... is) & -> decltype(auto)
        {
            return _bind(_derived(), is...);
        }

        constexpr auto _derived() const & -> T const&
        {
            return *(static_cast<T const*>(this));
        }

        constexpr auto _derived() && -> T&&
        {
            return std::move(*(static_cast<T*>(this)));
        }

        constexpr auto _derived() & -> T&
        {
            return *(static_cast<T*>(this));
        }

        static constexpr auto _bind(auto&& self, std::integral auto... is) -> decltype(auto)
        {
            static_assert(order_v<bindable> == sizeof...(is), "Incorrect number of indices");
            return ttl::evaluate(FWD(self), scalar_index{is...});
        }

        template <concepts::index_or_integral... Is>
        requires (concepts::index<Is> || ...)
        static constexpr auto _bind(auto&& self, Is... is)
        {
            static_assert(order_v<bindable> == sizeof...(Is), "Incorrect number of indices");
            constexpr tensor_index i{is...};
            return expressions::bind(FWD(self), is...);
        }
    };
}
