#pragma once

#include "md_array.hpp"
#include "ttl/ttl.hpp"
#include <algorithm>
#include <array>
#include <utility>

namespace ttl::tests
{
    template <class, int...>
    struct Tensor;
}

// Specialize the traits that we need for md_array and Tensor to be considered
// ttl::tensors.
namespace ttl::traits
{
    template <class T, int... extents>
    struct order<ttl::tests::md_array<T, extents...>>
    {
        static constexpr int value = sizeof...(extents);
    };

    template <class T, int... extents>
    struct scalar_type<ttl::tests::md_array<T, extents...>>
    {
        using type = T;
    };

    template <class T, int... extents>
    struct order<ttl::tests::Tensor<T, extents...>>
    {
        static constexpr int value = sizeof...(extents);
    };

    template <class T, int... extents>
    struct scalar_type<ttl::tests::Tensor<T, extents...>>
    {
        using type = double;
    };

    template <int M, class T, int... extents>
    requires (M > 1)
    struct extent<M, ttl::tests::Tensor<T, extents...>>
    {
        constexpr static std::array<int, sizeof...(extents)> _extents = { extents... };
        constexpr static int value = _extents[M];
    };
}

namespace ttl::tests
{
    ///
    template <class T, int... extents>
    struct Tensor
        : md_array<T, extents...>
        , ttl::Bindable<Tensor<T, extents...>>
    {
        static constexpr int _order = sizeof...(extents);
        static constexpr int _size = (extents * ... * 1);

        using md_array<T, extents...>::_data;

        constexpr Tensor() = default;

        /// Variadic constructor for a set of scalars.
        constexpr Tensor(ttl::concepts::scalar auto&&... vs)
            requires (0 < sizeof...(vs) and sizeof...(vs) <= _size)
                : md_array<T, extents...>{ static_cast<T>(vs)... }
        {
        }

        template <class Expression>
        requires (ttl::concepts::non_scalar<Expression> and
                  ttl::concepts::expression<Expression> and
                  ttl::traits::order_v<Expression> == _order)
        constexpr Tensor(Expression&& v)
        {
            ttl::assign(*this, FWD(v), [](T& a, ttl::traits::scalar_type_t<Expression> const& b) {
                return a = b;
            });
        }

        constexpr operator T() const requires (_size == 1)
        {
            return _data[0];
        }

        using ttl::Bindable<Tensor<T, extents...>>::operator();

        template <int N>
        static constexpr auto extent() -> int
        {
            return md_array<T, extents...>::_extents[N];
        }

        constexpr auto evaluate(ttl::ScalarIndex<_order> const& i) const -> T const&
        {
            return this->_at(*this, std::move(i));
        }

        constexpr auto evaluate(ttl::ScalarIndex<_order> const& i) -> T&
        {
            return this->_at(*this, std::move(i));
        }
    };
}

