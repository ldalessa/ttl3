#include "ttl/ttl.hpp"

static_assert(ttl::concepts::scalar<int>);
static_assert(ttl::concepts::tensor<int>);
static_assert(ttl::concepts::expression<int>);
static_assert(ttl::concepts::scalar<double>);
static_assert(ttl::concepts::tensor<double>);
static_assert(ttl::concepts::expression<double>);

int main()
{
}
