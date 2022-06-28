#pragma once

#include "ttl/bind.hpp"
#include "ttl/expression_traits.hpp"
#include "ttl/scalar_index.hpp"
#include "ttl/tensor_traits.hpp"
#include "ttl/typed_index.hpp"
#include "ttl/utils.hpp"

namespace ttl
{
    template <class T>
    struct bindable
    {
        static consteval auto order() -> int
        {
            static_assert(is_tensor<T>); // deferred because CRTP is annoying
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
            else if constexpr (is_expression<T>) {
                return ttl::evaluate(FWD(self), typed_index<outer<T>>{is...});
            }
            else {
                return ttl::evaluate(FWD(self), scalar_index{is...});
            }
        }
    };
}
