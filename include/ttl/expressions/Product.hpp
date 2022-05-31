#pragma once

#include "ttl/TensorIndex.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/expressions/Binary.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/outer.hpp"
#include "ttl/traits/scalar_type.hpp"

namespace ttl::expressions
{
    template <concepts::expression A, concepts::expression B, auto op>
    struct Product : Binary<A, B>
    {
        using scalar_type = std::common_type_t<traits::scalar_type_t<A>, traits::scalar_type_t<B>>;

        using Binary<A, B>::Binary;

        static consteval auto order() -> int {
            return outer().order();
        }

        static consteval auto outer() {
            return traits::outer_v<A> ^ traits::outer_v<B>;
        }
    };
}
