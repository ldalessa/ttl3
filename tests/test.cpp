#include "common.hpp"
#include "md_array.hpp"
#include "Tensor.hpp"
#include "ttl/ttl.hpp"
#include <concepts>
#include <cstdio>

using namespace ttl::tests;

// static_assert(ttl::traits::order_v<int> == 0);
// static_assert(ttl::traits::order_v<double> == 0);
// static_assert(ttl::traits::order_v<md_array<double>> == 0);
// static_assert(ttl::traits::order_v<md_array<double, 1>> == 1);
// static_assert(ttl::traits::order_v<md_array<double, 2, 2>> == 2);
// static_assert(std::same_as<ttl::traits::scalar_type_t<int>, int>);
// static_assert(std::same_as<ttl::traits::scalar_type_t<double>, double>);
// static_assert(std::same_as<ttl::traits::scalar_type_t<md_array<double>>, double>);
// static_assert(ttl::concepts::tensor<int>);
// static_assert(ttl::concepts::tensor<double>);
// static_assert(ttl::concepts::tensor<md_array<double>>);

int main()
{
    Tensor<double, 2, 2> A, B;

    auto i = ttl::index<'i'>;
    auto j = ttl::index<'j'>;
    auto k = ttl::index<'k'>;
    auto l = ttl::index<'l'>;

    auto C = A(i,~k) * B(k, j);
    auto D = C(~l,l);
    double d = D;
}
