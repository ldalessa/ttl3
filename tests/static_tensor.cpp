#include "static_tensor.hpp"
#include "common.hpp"
#include "ttl/ttl.hpp"
#include <array>

using ttl::tests::to_int_array;
using ttl::tests::type_args;
using ttl::tests::types;

template <class T, std::integral auto... extents>
requires ((extents >= 0) && ...)
using tensor = ttl::tests::static_tensor<T, to_int_array(extents...)>;

template <class T>
constexpr bool concepts(type_args<T> = {})
{
    using scalar = tensor<T>;
    static_assert(ttl::concepts::scalar<scalar>);
    static_assert(ttl::concepts::scalar<scalar>);
    static_assert(ttl::concepts::tensor<scalar>);
    static_assert(ttl::concepts::expression<scalar>);
    static_assert(ttl::concepts::has_static_extents<scalar>);
    static_assert(ttl::concepts::has_static_outer<scalar>);

    using vector_1 = tensor<T, 1>;
    static_assert(not ttl::concepts::scalar<vector_1>);
    static_assert(ttl::concepts::tensor<vector_1>);
    static_assert(not ttl::concepts::expression<vector_1>);
    static_assert(ttl::concepts::has_static_extents<vector_1>);
    static_assert(not ttl::concepts::has_static_outer<vector_1>);

    using vector_3 = tensor<T, 3>;
    static_assert(not ttl::concepts::scalar<vector_3>);
    static_assert(ttl::concepts::tensor<vector_3>);
    static_assert(not ttl::concepts::expression<vector_3>);
    static_assert(ttl::concepts::has_static_extents<vector_3>);
    static_assert(not ttl::concepts::has_static_outer<vector_3>);

    using matrix_3_3 = tensor<T, 3, 3>;
    static_assert(not ttl::concepts::scalar<matrix_3_3>);
    static_assert(ttl::concepts::tensor<matrix_3_3>);
    static_assert(not ttl::concepts::expression<matrix_3_3>);
    static_assert(ttl::concepts::has_static_extents<matrix_3_3>);
    static_assert(not ttl::concepts::has_static_outer<matrix_3_3>);

    using tensor_4_4_4_4 = tensor<T, 4, 4, 4, 4>;
    static_assert(not ttl::concepts::scalar<tensor_4_4_4_4>);
    static_assert(ttl::concepts::tensor<tensor_4_4_4_4>);
    static_assert(not ttl::concepts::expression<tensor_4_4_4_4>);
    static_assert(ttl::concepts::has_static_extents<tensor_4_4_4_4>);
    static_assert(not ttl::concepts::has_static_outer<tensor_4_4_4_4>);

    return true;
}

template <class T>
constexpr bool default_ctor(type_args<T> = {})
{
    tensor<T> a;
    tensor<T, 1> b;
    tensor<T, 3> c;
    tensor<T, 3, 3> d;
    tensor<T, 4, 4, 4, 4> e;
    return true;
}

template <class T>
constexpr bool linear_access(type_args<T> = {})
{
    bool passed = true;

    tensor<T> a;
    a[0] = 1;
    passed &= TTL_CHECK(a[0] == 1);

    tensor<T, 1> b;
    b[0] = 1;
    TTL_CHECK(b[0] == 1);

    tensor<T, 3> c;
    c[0] = 1;
    c[1] = 2;
    c[2] = 3;
    passed &= TTL_CHECK(c[0] == 1);
    passed &= TTL_CHECK(c[1] == 2);
    passed &= TTL_CHECK(c[2] == 3);

    tensor<T, 1, 1> d;
    d[0] = 1;
    passed &= TTL_CHECK(d[0] == 1);

    tensor<T, 2, 2> e;
    e[0] = 1;
    e[1] = 2;
    e[2] = 3;
    e[3] = 4;
    passed &= TTL_CHECK(e[0] == 1);
    passed &= TTL_CHECK(e[1] == 2);
    passed &= TTL_CHECK(e[2] == 3);
    passed &= TTL_CHECK(e[3] == 4);

    return passed;
}

template <class T>
constexpr bool aggregate_ctor(type_args<T>)
{
    bool passed = true;

    tensor<T> a{ 1 };
    passed &= TTL_CHECK(a[0] == 1);

    tensor<T, 1> b{ T(1) };
    passed &= TTL_CHECK(b[0] == 1);

    tensor<T, 3> c{ T(1), T(2) };
    passed &= TTL_CHECK(c[0] == 1);
    passed &= TTL_CHECK(c[1] == 2);

    tensor<T, 3> d{ T(1), T(2), T(3) };
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);
    passed &= TTL_CHECK(d[2] == 3);

    tensor<T, 1, 1> e{ T(1) };
    passed &= TTL_CHECK(e[0] == 1);

    tensor<T, 2, 2> f{ T(1), T(2), T(3) };
    passed &= TTL_CHECK(f[0] == 1);
    passed &= TTL_CHECK(f[1] == 2);
    passed &= TTL_CHECK(f[2] == 3);

    tensor<T, 2, 2> g{ T(1), T(2), T(3), T(4) };
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);
    passed &= TTL_CHECK(g[2] == 3);
    passed &= TTL_CHECK(g[3] == 4);

    return passed;
}

template <class T>
constexpr bool copy_ctor(type_args<T> = {})
{
    bool passed = true;

    tensor<T> aa{ T(1) };
    tensor<T> a = aa;
    passed &= TTL_CHECK(a[0] == 1);

    tensor<T, 1> cc{ T(1) };
    tensor<T, 1> c = cc;
    passed &= TTL_CHECK(c[0] == 1);

    tensor<T, 3> dd{ T(1), T(2) };
    tensor<T, 3> d = dd;
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);

    tensor<T, 3> ee{ T(1), T(2), T(3) };
    tensor<T, 3> e = ee;
    passed &= TTL_CHECK(e[0] == 1);
    passed &= TTL_CHECK(e[1] == 2);
    passed &= TTL_CHECK(e[2] == 3);

    tensor<T, 1, 1> ff{ T(1) };
    tensor<T, 1, 1> f = ff;
    passed &= TTL_CHECK(f[0] == 1);

    tensor<T, 2, 2> gg{ T(1), T(2), T(3) };
    tensor<T, 2, 2> g = gg;
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);
    passed &= TTL_CHECK(g[2] == 3);

    tensor<T, 2, 2> hh{ T(1), T(2), T(3), T(4) };
    tensor<T, 2, 2> h = hh;
    passed &= TTL_CHECK(h[0] == 1);
    passed &= TTL_CHECK(h[1] == 2);
    passed &= TTL_CHECK(h[2] == 3);
    passed &= TTL_CHECK(h[3] == 4);

    return passed;
}

template <class T>
constexpr bool move_ctor(type_args<T> = {})
{
    bool passed = true;

    tensor<T> aa{ T(1) };
    tensor<T> a = std::move(aa);
    passed &= TTL_CHECK(a[0] == 1);

    tensor<T, 1> cc{ T(1) };
    tensor<T, 1> c = std::move(cc);
    passed &= TTL_CHECK(c[0] == 1);

    tensor<T, 3> dd{ T(1), T(2) };
    tensor<T, 3> d = std::move(dd);
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);

    tensor<T, 3> ee{ T(1), T(2), T(3) };
    tensor<T, 3> e = std::move(ee);
    passed &= TTL_CHECK(e[0] == 1);
    passed &= TTL_CHECK(e[1] == 2);
    passed &= TTL_CHECK(e[2] == 3);

    tensor<T, 1, 1> ff{ T(1) };
    tensor<T, 1, 1> f = std::move(ff);
    passed &= TTL_CHECK(f[0] == 1);

    tensor<T, 2, 2> gg{ T(1), T(2), T(3) };
    tensor<T, 2, 2> g = std::move(gg);
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);
    passed &= TTL_CHECK(g[2] == 3);

    tensor<T, 2, 2> hh{ T(1), T(2), T(3), T(4) };
    tensor<T, 2, 2> h = std::move(hh);
    passed &= TTL_CHECK(h[0] == 1);
    passed &= TTL_CHECK(h[1] == 2);
    passed &= TTL_CHECK(h[2] == 3);
    passed &= TTL_CHECK(h[3] == 4);

    return passed;
}

template <class T>
constexpr bool copy(type_args<T> = {})
{
    bool passed = true;

    tensor<T> aa{ T(1) };
    tensor<T> a;
    a = aa;
    passed &= TTL_CHECK(a[0] == 1);

    tensor<T, 1> cc{ T(1) };
    tensor<T, 1> c;
    c = cc;
    passed &= TTL_CHECK(c[0] == 1);

    tensor<T, 3> dd{ T(1), T(2) };
    tensor<T, 3> d;
    d = dd;
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);

    tensor<T, 3> ee{ T(1), T(2), T(3) };
    tensor<T, 3> e;
    e = ee;
    passed &= TTL_CHECK(e[0] == 1);
    passed &= TTL_CHECK(e[1] == 2);
    passed &= TTL_CHECK(e[2] == 3);

    tensor<T, 1, 1> ff{ T(1) };
    tensor<T, 1, 1> f;
    f = ff;
    passed &= TTL_CHECK(f[0] == 1);

    tensor<T, 2, 2> gg{ T(1), T(2), T(3) };
    tensor<T, 2, 2> g;
    g = gg;
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);
    passed &= TTL_CHECK(g[2] == 3);

    tensor<T, 2, 2> hh{ T(1), T(2), T(3), T(4) };
    tensor<T, 2, 2> h;
    h = hh;
    passed &= TTL_CHECK(h[0] == 1);
    passed &= TTL_CHECK(h[1] == 2);
    passed &= TTL_CHECK(h[2] == 3);
    passed &= TTL_CHECK(h[3] == 4);

    return passed;
}

template <class T>
constexpr bool move(type_args<T> = {})
{
    bool passed = true;

    tensor<T> a, aa{ T(1) };
    a = std::move(aa);
    passed &= TTL_CHECK(a[0] == 1);

    tensor<T, 1> c, cc{ T(1) };
    c = std::move(cc);
    passed &= TTL_CHECK(c[0] == 1);

    tensor<T, 3> d, dd{ T(1), T(2) };
    d = std::move(dd);
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);

    tensor<T, 3> e, ee{ T(1), T(2), T(3) };
    e = std::move(ee);
    passed &= TTL_CHECK(e[0] == 1);
    passed &= TTL_CHECK(e[1] == 2);
    passed &= TTL_CHECK(e[2] == 3);

    tensor<T, 1, 1> f, ff{ T(1) };
    f = std::move(ff);
    passed &= TTL_CHECK(f[0] == 1);

    tensor<T, 2, 2> g, gg{ T(1), T(2), T(3) };
    g = std::move(gg);
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);
    passed &= TTL_CHECK(g[2] == 3);

    tensor<T, 2, 2> h, hh{ T(1), T(2), T(3), T(4) };
    h = std::move(hh);
    passed &= TTL_CHECK(h[0] == 1);
    passed &= TTL_CHECK(h[1] == 2);
    passed &= TTL_CHECK(h[2] == 3);
    passed &= TTL_CHECK(h[3] == 4);

    return passed;
}

template <class T>
constexpr bool md_access(type_args<T> = {})
{
    bool passed = true;

    tensor<T> a;
    a() = 1;
    passed &= TTL_CHECK(a[0] == 1);

    tensor<T, 1> c;
    c(0) = 1;
    TTL_CHECK(c[0] == 1);

    tensor<T, 3> d;
    d(0) = 1;
    d(1) = 2;
    d(2) = 3;
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);
    passed &= TTL_CHECK(d[2] == 3);

    tensor<T, 1, 1> e;
    e(0,0) = 1;
    passed &= TTL_CHECK(e[0] == 1);

    tensor<T, 2, 2> f;
    f(0,0) = 1;
    f(0,1) = 2;
    f(1,0) = 3;
    f(1,1) = 4;
    passed &= TTL_CHECK(f[0] == 1);
    passed &= TTL_CHECK(f[1] == 2);
    passed &= TTL_CHECK(f[2] == 3);
    passed &= TTL_CHECK(f[3] == 4);

    tensor<T, 2, 2, 2> g;
    g(0,0,0) = 1;
    g(0,0,1) = 2;
    g(0,1,0) = 3;
    g(0,1,1) = 4;
    g(1,0,0) = 5;
    g(1,0,1) = 6;
    g(1,1,0) = 7;
    g(1,1,1) = 8;
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);
    passed &= TTL_CHECK(g[2] == 3);
    passed &= TTL_CHECK(g[3] == 4);
    passed &= TTL_CHECK(g[4] == 5);
    passed &= TTL_CHECK(g[5] == 6);
    passed &= TTL_CHECK(g[6] == 7);
    passed &= TTL_CHECK(g[7] == 8);

    return passed;
}

template <class T>
constexpr bool tests()
{
    bool passed = true;

    passed &= TTL_CHECK( concepts(types<T>)   );
    passed &= TTL_CHECK( default_ctor(types<T>)   );
    passed &= TTL_CHECK( linear_access(types<T>)   );
    passed &= TTL_CHECK( aggregate_ctor(types<T>) );
    passed &= TTL_CHECK( copy_ctor(types<T>) );
    passed &= TTL_CHECK( move_ctor(types<T>) );
    passed &= TTL_CHECK( copy(types<T>) );
    passed &= TTL_CHECK( move(types<T>) );
    passed &= TTL_CHECK( md_access(types<T>) );

    return passed;
}

int main()
{
    constexpr bool i = tests<int>(); if (!i) throw;
    constexpr bool f = tests<float>(); if (!f) throw;
    constexpr bool d = tests<double>(); if (!d) throw;
}

