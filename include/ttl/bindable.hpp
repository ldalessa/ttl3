#pragma once

#include "ttl/bind.hpp"
#include "ttl/tensor_traits.hpp"
#include "ttl/utils.hpp"

namespace ttl
{
    template <class T>
    struct bindable
    {
        static consteval auto order() -> int
        {
            static_assert(has_tensor_traits<T>); // deferred because CRTP is annoying
            return ttl::order<T>;
        }

        constexpr auto _derived() const -> T const& {
            return *static_cast<T const*>(this);
        }

        constexpr auto _derived() -> T& {
            return *static_cast<T*>(this);
        }

        constexpr auto operator()(is_index_or_int auto... is) const -> decltype(auto) {
            return _bind(_derived(), is...);
        }

        constexpr auto operator()(is_index_or_int auto... is) -> decltype(auto) {
            return _bind(_derived(), is...);
        }

        template <is_index_or_int... Is>
        static constexpr auto _bind(auto&& self, Is... is) -> decltype(auto)
            requires (order() == sizeof...(is))
        {
            if constexpr ((is_index<Is> || ...)) {
                return bind(FWD(self), is...);
            }
            else {
                return ttl::evaluate(FWD(self), scalar_index{is...});
            }
        }
    };
}
