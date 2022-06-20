#include "ttl/ttl.hpp"
#include "md_array.hpp"
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

int main()
{
}
