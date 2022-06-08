#pragma once

#include "md_array.hpp"
#include "ttl/ttl.hpp"
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
        : md_array<double, extents...>
        , ttl::Bindable<Tensor<T, extents...>>
    {
        static constexpr int _order = sizeof...(extents);

        using ttl::Bindable<Tensor<T, extents...>>::operator();

        template <int N>
        static constexpr auto extent() -> int
        {
            return md_array<double, extents...>::_extents[N];
        }

        constexpr auto evaluate(ttl::ScalarIndex<_order> const& i) const -> double const&
        {
            return this->_at(*this, std::move(i));
        }

        constexpr auto evaluate(ttl::ScalarIndex<_order> const& i) -> double&
        {
            return this->_at(*this, std::move(i));
        }
    };
}

