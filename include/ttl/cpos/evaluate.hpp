#pragma once

#include "ttl/ScalarIndex.hpp"
#include "ttl/concepts/scalar.hpp"
#include "ttl/traits/scalar_type.hpp"
#include "ttl/utils/FWD.hpp"

namespace ttl
{
    namespace cpos
    {
        struct evaluate
        {
            template <class T, int order>
            constexpr auto operator()(T&& obj, ScalarIndex<order> const& i) const
                -> decltype(auto) requires requires { FWD(obj).evaluate(i); }
            {
                return FWD(obj).evaluate(i);
            }

            // template <concepts::scalar A>
            // constexpr auto operator()(A&& a, ScalarIndex<0> const& i) const
            //     -> decltype(auto)
            // {
            //     return static_cast<traits::scalar_type<A>>(a);
            // }

            consteval evaluate(int) {}
        };
    }

    inline constexpr cpos::evaluate evaluate{0};
}
