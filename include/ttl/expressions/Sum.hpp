#pragma once

#include "ttl/ScalarIndex.hpp"
#include "ttl/TensorIndex.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/concepts/static_extents.hpp"
#include "ttl/cpos/extent.hpp"
#include "ttl/expressions/Binary.hpp"
#include "ttl/traits/extent.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/outer.hpp"
#include "ttl/traits/scalar_type.hpp"
#include "ttl/utils/FWD.hpp"
#include "ttl/utils/sequence.hpp"
#include <utility>

namespace ttl::expressions
{
    template <concepts::expression A, concepts::expression B, auto op>
    struct Sum : Binary<A, B>
    {
        static_assert(is_permutation(traits::outer_v<A>, traits::outer_v<B>),
                      "Sum expression indices must be permutations");

        using Binary<A, B>::Binary;

        using scalar_type = std::common_type_t<traits::scalar_type_t<A>, traits::scalar_type_t<B>>;

        static constexpr TensorIndex _ai = traits::outer_v<A>;
        static constexpr TensorIndex _bi = traits::outer_v<A>;
        static constexpr int _order = traits::order_v<A>;

        static consteval auto order() -> int {
            return _order;
        }

        static consteval auto outer() -> decltype(_ai) {
            return _ai;
        }

        constexpr auto evaluate(ScalarIndex<_order> const& i) const -> scalar_type
        {
            expect(_validate_shapes());
            auto&& a = ttl::evaluate(this->_a, i);
            auto&& b = ttl::evaluate(this->_b, ttl::select<_ai, _bi>(i));
            return op(FWD(a), FWD(b));
        }

        static constexpr auto _validate_shapes() -> bool
            requires (concepts::static_extents<A> and concepts::static_extents<B>)
        {
            return []<int... i>(utils::sequence<i...>) {
                constexpr map_extents<_ai, _ai, A> a{};
                constexpr map_extents<_ai, _bi, B> b{};
                return ((ttl::extent<i>(a) == ttl::extent<i>(b)) && ...);
            }(utils::sequence_v<_order>);
        }

        constexpr auto _validate_shapes() const -> bool
        {
            return [&]<int... i>(utils::sequence<i...>) {
                map_extents<_ai, _ai, A> a{this->_a};
                map_extents<_ai, _bi, B> b{this->_b};
                return ((ttl::extent<i>(a) == ttl::extent<i>(b)) && ...);
            }(utils::sequence_v<_order>);
        }

        static_assert(not concepts::static_extents<A> or
                      not concepts::static_extents<B> or
                      _validate_shapes());
    };
}
