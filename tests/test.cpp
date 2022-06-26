#include "ttl/ttl.hpp"
#include "dynamic_tensor.hpp"
#include "static_tensor.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>

static_assert(ttl::is_scalar<int>);
static_assert(ttl::is_tensor<int>);
static_assert(ttl::is_expression<int>);
static_assert(ttl::is_scalar<double>);
static_assert(ttl::is_tensor<double>);
static_assert(ttl::is_expression<double>);
// static_assert(ttl::concepts::scalar<ttl::expressions::bind<int, ttl::tensor_index<0>{}>>);
// static_assert(ttl::concepts::tensor<ttl::expressions::bind<int, ttl::tensor_index<0>{}>>);
// static_assert(ttl::concepts::expression<ttl::expressions::bind<int, ttl::tensor_index<0>{}>>);
// static_assert(ttl::concepts::has_static_extents<ttl::expressions::bind<int, ttl::tensor_index<0>{}>>);
// static_assert(ttl::concepts::has_static_outer<ttl::expressions::bind<int, ttl::tensor_index<0>{}>>);

using scalar = ttl::tests::static_tensor<double, std::array<int, 0>{}>;
static_assert(ttl::is_tensor<scalar>);
static_assert(ttl::is_scalar<scalar>);
static_assert(ttl::is_expression<scalar>);

using vector3d = ttl::tests::static_tensor<double, std::array<int, 1>{3}>;
static_assert(ttl::is_tensor<vector3d>);
static_assert(not ttl::is_scalar<vector3d>);
static_assert(not ttl::is_expression<vector3d>);

using matrixNd = ttl::tests::dynamic_tensor<double, 2>;
static_assert(ttl::is_tensor<matrixNd>);
static_assert(not ttl::is_scalar<matrixNd>);
static_assert(not ttl::is_expression<matrixNd>);

static_assert(ttl::is_int_array<std::array<int, 0>>);
static_assert(ttl::is_int_array<std::array<int, 1>>);

[[deprecated]]
void print(auto) {}

int main()
{
    vector3d x;
    x(0) = 0.0;
    x(1) = 0.1;
    x(2) = 0.2;
    fmt::print("{}\n", x._data);

    ttl::index<'i'> i;
    auto b = x(i);
    // constexpr auto c = ttl::extents(b);

    matrixNd A(3, 3);
    A(0,0) = 0.0;
    A(0,1) = 0.1;
    A(0,2) = 0.2;
    A(1,0) = 1.0;
    A(1,1) = 1.1;
    A(1,2) = 1.2;
    A(2,0) = 2.0;
    A(2,1) = 2.1;
    A(2,2) = 2.2;
    fmt::print("{}\n", A._data);

    ttl::index<'j'> j;
    auto d = A(i,j);
    auto e = ttl::extents(d);
}
