#pragma once

#include "md_array.hpp"
#include "ttl/ttl.hpp"
#include <algorithm>
#include <array>
#include <concepts>
#include <utility>

namespace ttl::tests
{
    template <class T, ttl::concepts::shape auto _shape>
    struct Tensor : md_array<T, _shape>
                  , ttl::bindable<Tensor<T, _shape>>
    {
        using md_array<T, _shape>::_data;;
        using md_array<T, _shape>::order;
        using md_array<T, _shape>::size;
        using ttl::bindable<Tensor<T, _shape>>::operator();

        constexpr Tensor() = default;

        /// Variadic constructor for a set of scalars.
        constexpr Tensor(std::convertible_to<T> auto&&... vs)
            requires (0 < sizeof...(vs) and sizeof...(vs) <= size())
            : md_array<T, _shape>{ static_cast<T>(vs)... }
        {
        }

        constexpr static auto _evaluate(auto&& self, ttl::scalar_index<order()> const& index)
            -> decltype(auto)
        {
            return [&]<std::size_t... is>(std::index_sequence<is...>) {
                return std::forward<md_array<T, _shape>>(self)(index[is]...);
            }(std::make_index_sequence<order()>{});
        }

        constexpr friend auto tag_invoke(
                ttl::cpos::evaluate,
                Tensor const& t,
                ttl::scalar_index<order()> const& index)
            -> decltype(auto)
        {
            return _evaluate(t, index);
        }

        constexpr friend auto tag_invoke(
                ttl::cpos::evaluate,
                Tensor& t,
                ttl::scalar_index<order()> const& index)
            -> decltype(auto)
        {
            return _evaluate(t, index);
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
