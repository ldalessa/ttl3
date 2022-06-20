#include "ttl/ttl.hpp"
#include "dynamic_tensor.hpp"
#include "Tensor.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>

static_assert(ttl::concepts::scalar<int>);
static_assert(ttl::concepts::tensor<int>);
static_assert(ttl::concepts::expression<int>);
static_assert(ttl::concepts::scalar<double>);
static_assert(ttl::concepts::tensor<double>);
static_assert(ttl::concepts::expression<double>);
static_assert(ttl::concepts::scalar<ttl::expressions::bind<int>>);
static_assert(ttl::concepts::tensor<ttl::expressions::bind<int>>);
static_assert(ttl::concepts::expression<ttl::expressions::bind<int>>);
static_assert(ttl::concepts::has_static_extents<ttl::expressions::bind<int>>);
static_assert(ttl::concepts::has_static_outer<ttl::expressions::bind<int>>);

using scalar = ttl::tests::Tensor<double, std::array<int, 0>{}>;
static_assert(ttl::concepts::scalar<scalar>);
static_assert(ttl::concepts::tensor<scalar>);
static_assert(ttl::concepts::expression<scalar>);
static_assert(ttl::concepts::has_static_extents<scalar>);
static_assert(ttl::concepts::has_static_outer<scalar>);

using vector3d = ttl::tests::Tensor<double, std::array<int, 1>{3}>;
static_assert(not ttl::concepts::scalar<vector3d>);
static_assert(ttl::concepts::tensor<vector3d>);
static_assert(not ttl::concepts::expression<vector3d>);
static_assert(ttl::concepts::has_static_extents<vector3d>);
static_assert(not ttl::concepts::has_static_outer<vector3d>);

using matrixNd = ttl::tests::dynamic_tensor<double, 2>;
static_assert(not ttl::concepts::scalar<matrixNd>);
static_assert(ttl::concepts::tensor<matrixNd>);
static_assert(not ttl::concepts::expression<matrixNd>);
static_assert(not ttl::concepts::has_static_extents<matrixNd>);
static_assert(not ttl::concepts::has_static_outer<matrixNd>);

int main()
{
    vector3d x;
    x(0) = 0.0;
    x(1) = 0.1;
    x(2) = 0.2;

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
}
