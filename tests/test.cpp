#include "md_array.hpp"
#include "ttl/ttl.hpp"
#include <concepts>

namespace ttl::traits
{
    template <class T, int... extents>
    struct order<md_array<T, extents...>>
    {
        static constexpr int value = sizeof...(extents);
    };

    template <class T, int... extents>
    struct scalar_type<md_array<T, extents...>>
    {
        using type = T;
    };
}

static_assert(ttl::traits::order_v<int> == 0);
static_assert(ttl::traits::order_v<double> == 0);
static_assert(ttl::traits::order_v<md_array<double>> == 0);
static_assert(ttl::traits::order_v<md_array<double, 1>> == 1);
static_assert(ttl::traits::order_v<md_array<double, 2, 2>> == 2);
static_assert(std::same_as<ttl::traits::scalar_type_t<int>, int>);
static_assert(std::same_as<ttl::traits::scalar_type_t<double>, double>);
static_assert(std::same_as<ttl::traits::scalar_type_t<md_array<double>>, double>);
static_assert(ttl::concepts::tensor<int>);
static_assert(ttl::concepts::tensor<double>);
static_assert(ttl::concepts::tensor<md_array<double>>);
// using namespace ttl::grammar;

template <int...>
struct Tensor;

namespace ttl::traits
{
    template <int... extents>
    struct order<Tensor<extents...>>
    {
        static constexpr int value = sizeof...(extents);
    };

    template <int... extents>
    struct scalar_type<Tensor<extents...>>
    {
        using type = double;
    };

    // template <int M, int... extents>
    // struct extent<M, Tensor<extents...>>
    // {
    //     constexpr static std::array<int, sizeof...(extents)> _extents = { extents... };
    //     constexpr static int value = _extents[M];
    // };
}

template <int... extents>
struct Tensor
    : md_array<double, extents...>
    , ttl::Bindable<Tensor<extents...>>
{
    static constexpr int _order = sizeof...(extents);

    using ttl::Bindable<Tensor<extents...>>::operator();

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

template <auto>
[[gnu::deprecated]]
void print() {}

[[gnu::deprecated]]
void print_t(auto&& u) {}

#include <cstdio>

int main()
{
    Tensor<2, 2> A, B;

    auto i = ttl::index<'i'>;
    auto j = ttl::index<'j'>;
    auto k = ttl::index<'k'>;
    auto l = ttl::index<'l'>;

    auto C = A(i,~k) * B(k, j);
    // print_t(C);
    // print_t(C.outer());
    auto D = C(~l,l);
    // print_t(D);
    // print_t(D.outer());
    double d = D;
}
