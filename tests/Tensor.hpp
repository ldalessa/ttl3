#pragma once

#include "md_array.hpp"
#include "ttl/ttl.hpp"
#include <algorithm>
#include <array>
#include <utility>

namespace ttl::tests
{
    template <class, ttl::concepts::shape auto>
    struct Tensor;
}

// Specialize the traits that we need for md_array and Tensor to be considered
// ttl::tensors.
namespace ttl::traits
{
    template <class T, ttl::concepts::shape auto _shape>
    struct tensor<ttl::tests::Tensor<T, _shape>>
    {
        using value_type = T;
        static constexpr int order = _shape.size();
        static constexpr auto extents = _shape;
    };
}

namespace ttl::tests
{
    ///
    template <class T, ttl::concepts::shape auto _shape>
    struct Tensor
        : md_array<T, _shape>
        // , ttl::Bindable<Tensor<T, extents...>>
    {
        using md_array<T, _shape>::_data;;
        using md_array<T, _shape>::order;
        using md_array<T, _shape>::size;

        constexpr Tensor() = default;

        /// Variadic constructor for a set of scalars.
        constexpr Tensor(ttl::concepts::scalar auto&&... vs)
            requires (0 < sizeof...(vs) and sizeof...(vs) <= size())
            : md_array<T, _shape>{ static_cast<T>(vs)... }
        {
        }

        // /// Allow implicit conversion from expressions.
        // template <class Expression>
        // requires (ttl::concepts::non_scalar<Expression> and
        //           ttl::concepts::expression<Expression> and
        //           ttl::traits::order_v<Expression> == _order)
        // constexpr Tensor(Expression&& v)
        // {
        //     ttl::assign(*this, FWD(v), [](T& a, ttl::traits::scalar_type_t<Expression> const& b) {
        //         return a = b;
        //     });
        // }

        // constexpr operator T() const requires (_size == 1)
        // {
        //     return _data[0];
        // }

        // using ttl::Bindable<Tensor<T, extents...>>::operator();

        // template <int N>
        // static constexpr auto extent() -> int
        // {
        //     return md_array<T, extents...>::_extents[N];
        // }

        // constexpr auto evaluate(ttl::ScalarIndex<_order> const& i) const -> T const&
        // {
        //     return this->_at(*this, std::move(i));
        // }

        // constexpr auto evaluate(ttl::ScalarIndex<_order> const& i) -> T&
        // {
        //     return this->_at(*this, std::move(i));
        // }
    };

    // namespace concepts {
    //     template <int Order, class Expression>
    //     concept expression_of_order = (
    //             ttl::concepts::expression<Expression> and
    //             ttl::concepts::static_extents<Expression> and
    //             ttl::traits::order_v<Expression> == Order);
    // }

    // template <class Expression> requires (concepts::expression_of_order<0, Expression>)
    // Tensor(Expression&&) -> Tensor<ttl::traits::scalar_type_t<Expression>>;

    // template <class Expression> requires (concepts::expression_of_order<1, Expression>)
    // Tensor(Expression&&) -> Tensor<ttl::traits::scalar_type_t<Expression>,
    //                                ttl::traits::extent_v<0, Expression>>;

    // template <class Expression> requires (concepts::expression_of_order<2, Expression>)
    // Tensor(Expression&&) -> Tensor<ttl::traits::scalar_type_t<Expression>,
    //                                ttl::traits::extent_v<0, Expression>,
    //                                ttl::traits::extent_v<1, Expression>>;

    // template <class Expression> requires (concepts::expression_of_order<3, Expression>)
    // Tensor(Expression&&) -> Tensor<ttl::traits::scalar_type_t<Expression>,
    //                                ttl::traits::extent_v<0, Expression>,
    //                                ttl::traits::extent_v<1, Expression>,
    //                                ttl::traits::extent_v<2, Expression>>;

    // template <class Expression> requires (concepts::expression_of_order<4, Expression>)
    // Tensor(Expression&&) -> Tensor<ttl::traits::scalar_type_t<Expression>,
    //                                ttl::traits::extent_v<0, Expression>,
    //                                ttl::traits::extent_v<1, Expression>,
    //                                ttl::traits::extent_v<2, Expression>,
    //                                ttl::traits::extent_v<3, Expression>>;
}

