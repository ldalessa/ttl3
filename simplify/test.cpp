#include "array.hpp"
#include "bindable.hpp"
#include "operators.hpp"
#include "scalar_index.hpp"
#include "tensor_index.hpp"
#include <vector>

using namespace ttl::operators;

static_assert(ttl::is_array<ttl::array<int, 10>>);
static_assert(ttl::is_array<ttl::array<int, 0>>);
static_assert(ttl::is_int_array<ttl::array<int, 10>>);
static_assert(ttl::is_int_array<ttl::array<int, 0>>);
static_assert(ttl::is_array<ttl::scalar_index<10>>);
static_assert(ttl::is_array<ttl::scalar_index<0>>);
static_assert(ttl::is_int_array<ttl::scalar_index<10>>);
static_assert(ttl::is_int_array<ttl::scalar_index<0>>);

static_assert(ttl::is_index_or_int<ttl::index<'i'>>);
static_assert(ttl::is_index_or_int<int>);

ttl::index<'i'> i;
ttl::index<'j'> j;
ttl::index<'k'> k;
ttl::index<L'μ'> μ;
namespace index_constructors
{
    constexpr ttl::tensor_index x(i);
    constexpr ttl::tensor_index y(i, 1);
    constexpr ttl::tensor_index z(i, 1, j);
    constexpr ttl::tensor_index w(i, 1, j, 0);
    constexpr ttl::tensor_index u(i, 1, j, ~i);
    constexpr ttl::tensor_index v(i, 1, j, ~i, ~j);
}

struct scalar : ttl::bindable<scalar>
{
    using scalar_type = double;

    double v = 0;

    static consteval auto get_order() -> int {
        return 0;
    }

    constexpr auto get_extents() const -> ttl::array<int, 0> {
        return {};
    }

   constexpr auto evaluate(ttl::scalar_index<0>) const -> double const& {
        return v;
    }

    constexpr auto evaluate(ttl::scalar_index<0>) -> double& {
        return v;
    }
};

struct matrix : ttl:: bindable<matrix>
{
    using scalar_type = double;

    ttl::array<int, 2> _extents;
    ttl::array<int, 2> _strides;
    std::vector<double> _data;

    constexpr matrix(int n, int m)
            : _extents{n, m}
            , _strides{m, 1}
            , _data(n * m)
    {
    }

    static consteval auto get_order() -> int {
        return 2;
    }

    constexpr auto get_extents() const -> ttl::array<int, 2> const& {
        return _extents;
    }

    constexpr auto evaluate(ttl::scalar_index<2> const& i) const -> double const& {
        return _data[_row_major(i)];
    }

    constexpr auto evaluate(ttl::scalar_index<2> const& i) -> double& {
        return _data[_row_major(i)];
    }

    constexpr auto _row_major(ttl::scalar_index<2> const& i) const -> int {
        return i[0] * _strides[0] + i[1] * _strides[1];
    }
};

static_assert(ttl::has_member_tensor_traits<scalar>);

scalar s;

matrix A(128,64);
auto c = A(i,j);
static_assert(ttl::has_member_tensor_traits<decltype(c)>);
static_assert(ttl::has_member_expression_traits<decltype(c)>);

auto d = A(j,i);
static_assert(ttl::has_member_tensor_traits<decltype(d)>);
static_assert(ttl::has_member_expression_traits<decltype(d)>);

auto f = c + d;
auto g = f - c;
// static_assert(ttl::has_member_tensor_traits<decltype(c)>);
// static_assert(ttl::has_member_tensor_traits<decltype(d)>);

auto e = A(i,~j) * A(j,k);
static_assert(ttl::has_member_tensor_traits<decltype(e)>);
static_assert(ttl::has_member_expression_traits<decltype(e)>);
static_assert(ttl::order<decltype(e)> == 2);

auto h = A(i,~j) * A(k,μ);
static_assert(ttl::has_member_tensor_traits<decltype(h)>);
// static_assert(ttl::has_member_expression_traits<decltype(h)>);
// static_assert(ttl::order<decltype(h)> == 4);

// static_assert(std::remove_cvref_t<decltype(c)>::get_order() == 2);
// static_assert(ttl::order<decltype(c)> == 2);

int main()
{
    s() = 1;
}
