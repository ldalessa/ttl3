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
static_assert(ttl::concepts::bindable<int>);
static_assert(ttl::concepts::bindable<double>);
static_assert(ttl::concepts::bindable<md_array<double>>);
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
void print_t(auto&&) {}

#include <cstdio>

int main()
{
    // Tensor<2, 2, 2> A;
    // A(0,0,0) = 1;
    // A(1,0,0) = 2;
    // A(0,1,0) = 3;
    // A(1,1,0) = 4;
    // A(0,0,1) = 5;
    // A(1,0,1) = 6;
    // A(0,1,1) = 7;
    // A(1,1,1) = 8;

    Tensor<1, 2, 1> A;
    A(0,0,0) = 1;
    A(0,1,0) = 3;

    auto i = ttl::index<'i'>;
    auto j = ttl::index<'j'>;
    auto k = ttl::index<'k'>;

    double d = A(~i,j,i)(0);
    std::printf("%f\n", d);

    // Tensor<1> B = { 1 };
    // // double e = B();
    // auto Bʹ = B(0);
    // double e = Bʹ;

    // Tensor<> C = {1};

    // auto D = C + C;
    // // print_t(D);
    // double d = D();
    // std::printf("%f\n", d);

    // auto Aʹ = A(i,j,k);

    // for (int i = 0; i < 2; ++i) {
    //     for (int j = 0; j < 2; ++j) {
    //         for (int k = 0; k < 2; ++k) {
    //             double x = Aʹ.evaluate(ttl::ScalarIndex(i, j, k));
    //             Aʹ.evaluate(ttl::ScalarIndex(i, j, k)) = 0;
    //             double y = Aʹ.evaluate(ttl::ScalarIndex(i, j, k));
    //             std::printf("%f, %f\n", x, y);
    //         }
    //     }
    // }

    // ttl::Index<'i'> i;
    // albert::Index<'j'> j;
    // albert::Tensor<double, 1, 3> a = { 1, 2, 3 };
    // a(0) = 1;
    // albert::Tensor<double, 1, 3> b;
    // albert::Tensor<double, 0, 0> c = { 2 };
    // albert::Tensor<double, 2, 3> A = {
    //   1, 2, 3,
    //   4, 5, 6,
    //   7, 8, 9
    // };

    // a(j) += A(j,i)*a(i);
}
