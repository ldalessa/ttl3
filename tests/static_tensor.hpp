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
    struct static_tensor : md_array<T, _shape> , ttl::bindable<static_tensor<T, _shape>>
    {
        using md_array<T, _shape>::_data;;
        using md_array<T, _shape>::order;
        using md_array<T, _shape>::size;
        using ttl::bindable<static_tensor<T, _shape>>::operator();

        constexpr static_tensor() = default;

        /// Variadic constructor for a set of scalars.
        constexpr static_tensor(std::convertible_to<T> auto&&... vs)
            requires (0 < sizeof...(vs) and sizeof...(vs) <= size())
            : md_array<T, _shape>{ static_cast<T>(vs)... }
        {
        }

        constexpr friend auto tag_invoke(ttl::cpos::evaluate, static_tensor const& t, ttl::scalar_index<order()> const& index)
            -> decltype(auto)
        {
            return _evaluate(t, index);
        }

        constexpr friend auto tag_invoke(ttl::cpos::evaluate, static_tensor&& t, ttl::scalar_index<order()> const& index)
            -> decltype(auto)
        {
            return _evaluate(std::move(t), index);
        }

        constexpr friend auto tag_invoke(ttl::cpos::evaluate, static_tensor& t, ttl::scalar_index<order()> const& index)
            -> decltype(auto)
        {
            return _evaluate(t, index);
        }

        /// Handle the ttl::cpos::evaluate CPO for const&,&&,& dynamic vectors.
        ///
        /// This could also be done in the raw tag_invoke calls above if we had
        /// concept for dynamic tensors. The md_array provides a variadic
        /// operator(), so we just expand the index into a variadic form here.
        ///
        /// @param self  A forwarding reference formed from the this pointer.
        /// @param index The index to evalute.
        constexpr static auto _evaluate(auto&& self, ttl::scalar_index<order()> const& index)
            -> decltype(auto)
        {
            return [&]<std::size_t... is>(std::index_sequence<is...>) -> decltype(auto) {
                return std::forward<md_array<T, _shape>>(self)(index[is]...);
            }(std::make_index_sequence<order()>{});
        }

        // /// Allow implicit conversion from expressions.
        // template <class Expression>
        // requires (ttl::concepts::non_scalar<Expression> and
        //           ttl::concepts::expression<Expression> and
        //           ttl::traits::order_v<Expression> == order())
        // constexpr static_tensor(Expression&& v)
        // {
        //     ttl::assign(*this, FWD(v), [](T& a, ttl::traits::scalar_type_t<Expression> const& b) {
        //         return a = b;
        //     });
        // }

        // constexpr operator T() const requires (_size == 1)
        // {
        //     return _data[0];
        // }

        // using ttl::Bindable<static_tensor<T, extents...>>::operator();

        // template <int N>
        // static constexpr auto extent() -> int
        // {
        //     return md_array<T, extents...>::_extents[N];
        // }

        // constexpr auto evaluate(ttl::ScalarIndex<order()> const& i) const -> T const&
        // {
        //     return this->_at(*this, std::move(i));
        // }

        // constexpr auto evaluate(ttl::ScalarIndex<order()> const& i) -> T&
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
    // static_tensor(Expression&&) -> static_tensor<ttl::traits::scalar_type_t<Expression>>;

    // template <class Expression> requires (concepts::expression_of_order<1, Expression>)
    // static_tensor(Expression&&) -> static_tensor<ttl::traits::scalar_type_t<Expression>,
    //                                ttl::traits::extent_v<0, Expression>>;

    // template <class Expression> requires (concepts::expression_of_order<2, Expression>)
    // static_tensor(Expression&&) -> static_tensor<ttl::traits::scalar_type_t<Expression>,
    //                                ttl::traits::extent_v<0, Expression>,
    //                                ttl::traits::extent_v<1, Expression>>;

    // template <class Expression> requires (concepts::expression_of_order<3, Expression>)
    // static_tensor(Expression&&) -> static_tensor<ttl::traits::scalar_type_t<Expression>,
    //                                ttl::traits::extent_v<0, Expression>,
    //                                ttl::traits::extent_v<1, Expression>,
    //                                ttl::traits::extent_v<2, Expression>>;

    // template <class Expression> requires (concepts::expression_of_order<4, Expression>)
    // static_tensor(Expression&&) -> static_tensor<ttl::traits::scalar_type_t<Expression>,
    //                                ttl::traits::extent_v<0, Expression>,
    //                                ttl::traits::extent_v<1, Expression>,
    //                                ttl::traits::extent_v<2, Expression>,
    //                                ttl::traits::extent_v<3, Expression>>;
}

namespace ttl::traits
{
    template <class T, ttl::concepts::shape auto _shape>
    struct tensor<ttl::tests::static_tensor<T, _shape>>
    {
        using value_type = T;
        static constexpr int order = _shape.size();
        static constexpr auto extents = _shape;
    };
}
