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

        static constexpr auto _outer = traits::outer_v<A> ^ traits::outer_v<B>;
        static constexpr auto _order = _outer.order();

        using Binary<A, B>::Binary;

        static consteval auto order() -> int {
            return _order;
        }

        static consteval auto outer() {
            return _outer;
        }

        constexpr auto evaluate(ScalarIndex<order()> const& index) const -> scalar_type
        {
            constexpr TensorIndex ai = traits::outer_v<A>;
            constexpr TensorIndex bi = traits::outer_v<B>;
            constexpr TensorIndex contracted = ai & bi;
            constexpr TensorIndex all = _outer + contracted;

            scalar_type c{};
            ScalarIndex<all.size()> i = index;
            do {
                auto&& a = ttl::evaluate(this->_a, ttl::select<all, ai>(i));
                auto&& b = ttl::evaluate(this->_b, ttl::select<all, bi>(i));
                c += op(FWD(a), FWD(b));
            } while (ttl::carry_sum_inc<order()>(i, *this));
            return c;
        }

    };
}
