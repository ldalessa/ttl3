#pragma once

#include "ttl/TensorIndex.hpp"
#include "ttl/concepts/index.hpp"
#include "ttl/cpos/evaluate.hpp"
#include "ttl/expressions/Bind.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/scalar_type.hpp"
#include "ttl/utils/FWD.hpp"

namespace ttl
{
    template <class T>
    struct Bindable;

    namespace traits
    {
        template <class T>
        struct order<Bindable<T>>
        {
            static constexpr int value = order_v<T>;
        };

        template <class T>
        struct scalar_type<Bindable<T>>
        {
            using type = scalar_type_t<T>;
        };
    }

    template <class T>
    struct Bindable
    {
        constexpr auto operator()(concepts::index auto... is) const& -> decltype(auto)
        {
            return _bind(_derived(), is...);
        }

        constexpr auto operator()(concepts::index auto... is) && -> decltype(auto)
        {
            return _bind(_derived(), is...);
        }

        constexpr auto operator()(concepts::index auto... is) & -> decltype(auto)
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

        template <concepts::index... Is>
        static constexpr auto _bind(concepts::tensor auto&& self, Is... is) -> decltype(auto)
        {
            static_assert(traits::order_v<Bindable> == sizeof...(is), "Incorrect number of indices");

            if constexpr ((std::integral<Is> && ...)) {
                return evaluate(FWD(self), ScalarIndex{is...});
            }
            else {
                return expressions::Bind { FWD(self), is... };
            }
        }
    };
}
