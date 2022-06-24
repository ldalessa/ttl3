#include "dynamic_tensor.hpp"
#include "common.hpp"
#include "ttl/ttl.hpp"
#include <array>

using ttl::tests::dynamic_tensor;
using ttl::tests::type_args;
using ttl::tests::types;

template <class T>
constexpr bool concepts(type_args<T> = {})
{
    using scalar = dynamic_tensor<T, 0>;
    static_assert(ttl::concepts::scalar<scalar>);
    static_assert(ttl::concepts::tensor<scalar>);
    static_assert(ttl::concepts::expression<scalar>);
    static_assert(ttl::concepts::has_static_extents<scalar>);
    static_assert(ttl::concepts::has_static_outer<scalar>);

    using vector = dynamic_tensor<T, 1>;
    static_assert(not ttl::concepts::scalar<vector>);
    static_assert(ttl::concepts::tensor<vector>);
    static_assert(not ttl::concepts::expression<vector>);
    static_assert(not ttl::concepts::has_static_extents<vector>);
    static_assert(not ttl::concepts::has_static_outer<vector>);

    using matrix = dynamic_tensor<T, 2>;
    static_assert(not ttl::concepts::scalar<matrix>);
    static_assert(ttl::concepts::tensor<matrix>);
    static_assert(not ttl::concepts::expression<matrix>);
    static_assert(not ttl::concepts::has_static_extents<matrix>);
    static_assert(not ttl::concepts::has_static_outer<matrix>);

    using tensor = dynamic_tensor<T, 4>;
    static_assert(not ttl::concepts::scalar<tensor>);
    static_assert(ttl::concepts::tensor<tensor>);
    static_assert(not ttl::concepts::expression<tensor>);
    static_assert(not ttl::concepts::has_static_extents<tensor>);
    static_assert(not ttl::concepts::has_static_outer<tensor>);

    return true;
}

template <class T>
constexpr bool default_ctor(type_args<T> = {})
{
    dynamic_tensor<T, 0> a;
    return true;
}

template <class T>
constexpr bool linear_access(type_args<T> = {})
{
    bool passed = true;

    dynamic_tensor<T, 0> a;
    a[0] = 1;
    passed &= TTL_CHECK(a[0] == 1);

    dynamic_tensor<T, 1> c(3);
    c[0] = 1;
    c[1] = 2;
    c[2] = 3;
    passed &= TTL_CHECK(c[0] == 1);
    passed &= TTL_CHECK(c[1] == 2);
    passed &= TTL_CHECK(c[2] == 3);

    dynamic_tensor<T, 2> d(1, 1);
    d[0] = 1;
    passed &= TTL_CHECK(d[0] == 1);

    dynamic_tensor<T, 2> e(2, 2);
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
constexpr bool md_access(type_args<T> = {})
{
    bool passed = true;

    dynamic_tensor<T, 0> a;
    a() = 1;
    passed &= TTL_CHECK(a[0] == 1);

    dynamic_tensor<T, 1> c(1);
    c(0) = 1;
    TTL_CHECK(c[0] == 1);

    dynamic_tensor<T, 1> d(3);
    d(0) = 1;
    d(1) = 2;
    d(2) = 3;
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);
    passed &= TTL_CHECK(d[2] == 3);

    dynamic_tensor<T, 2> e(1,1);
    e(0,0) = 1;
    passed &= TTL_CHECK(e[0] == 1);

    dynamic_tensor<T, 2> f(2,2);
    f(0,0) = 1;
    f(0,1) = 2;
    f(1,0) = 3;
    f(1,1) = 4;
    passed &= TTL_CHECK(f[0] == 1);
    passed &= TTL_CHECK(f[1] == 2);
    passed &= TTL_CHECK(f[2] == 3);
    passed &= TTL_CHECK(f[3] == 4);

    dynamic_tensor<T, 3> g(2,2,2);
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
constexpr bool copy_ctor(type_args<T> = {})
{
    bool passed = true;

    dynamic_tensor<T, 0> aa;
    aa() = T(1);
    dynamic_tensor<T, 0> a = aa;
    passed &= TTL_CHECK(a[0] == 1);

    dynamic_tensor<T, 1> cc(1);
    cc(0) = T(1);
    dynamic_tensor c = cc;
    passed &= TTL_CHECK(c[0] == 1);

    dynamic_tensor<T, 1> dd(2);
    dd(0) = T(1);
    dd(1) = T(2);
    dynamic_tensor d = dd;
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);

    dynamic_tensor<T, 1> ee(3);
    ee(0) = T(1);
    ee(1) = T(2);
    ee(2) = T(3);
    dynamic_tensor e = ee;
    passed &= TTL_CHECK(e[0] == 1);
    passed &= TTL_CHECK(e[1] == 2);
    passed &= TTL_CHECK(e[2] == 3);

    dynamic_tensor<T, 2> ff(1,1);
    ff(0,0) = T(1);
    dynamic_tensor f = ff;
    passed &= TTL_CHECK(f[0] == 1);

    dynamic_tensor<T, 2> gg(2,2);
    gg(0,0) = T(1);
    gg(0,1) = T(2);
    gg(1,0) = T(3);
    gg(1,1) = T(4);
    dynamic_tensor g = gg;
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);
    passed &= TTL_CHECK(g[2] == 3);
    passed &= TTL_CHECK(g[3] == 4);

    return passed;
}

template <class T>
constexpr bool move_ctor(type_args<T> = {})
{
    bool passed = true;

    dynamic_tensor<T, 0> aa;
    aa() = T(1);
    dynamic_tensor a = std::move(aa);
    passed &= TTL_CHECK(a[0] == 1);

    dynamic_tensor<T, 1> cc(1);
    cc(0) = T(1);
    dynamic_tensor c = std::move(cc);
    passed &= TTL_CHECK(c[0] == 1);

    dynamic_tensor<T, 1> dd(2);
    dd(0) = T(1);
    dd(1) = T(2);
    dynamic_tensor d = std::move(dd);
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);

    dynamic_tensor<T, 1> ee(3);
    ee(0) = T(1);
    ee(1) = T(2);
    ee(2) = T(3);
    dynamic_tensor e = std::move(ee);
    passed &= TTL_CHECK(e[0] == 1);
    passed &= TTL_CHECK(e[1] == 2);
    passed &= TTL_CHECK(e[2] == 3);

    dynamic_tensor<T, 2> ff(1,1);
    ff(0,0) = T(1);
    dynamic_tensor f = std::move(ff);
    passed &= TTL_CHECK(f[0] == 1);

    dynamic_tensor<T, 2> gg(2,2);
    gg(0,0) = T(1);
    gg(0,1) = T(2);
    gg(1,0) = T(3);
    gg(1,1) = T(4);
    dynamic_tensor g = std::move(gg);
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);
    passed &= TTL_CHECK(g[2] == 3);
    passed &= TTL_CHECK(g[3] == 4);

    return passed;
}

template <class T>
constexpr bool copy(type_args<T> = {})
{
    bool passed = true;

    dynamic_tensor<T, 0> aa;
    aa() = T(1);
    dynamic_tensor<T, 0> a;
    a = aa;
    passed &= TTL_CHECK(a[0] == 1);

    dynamic_tensor<T, 1> cc(1);
    cc(0) = T(1);
    dynamic_tensor<T, 1> c;
    c = cc;
    passed &= TTL_CHECK(c[0] == 1);

    dynamic_tensor<T, 1> dd(2);
    dd(0) = T(1);
    dd(1) = T(2);
    dynamic_tensor<T, 1> d;
    d = dd;
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);

    dynamic_tensor<T, 1> ee(3);
    ee(0) = T(1);
    ee(1) = T(2);
    ee(2) = T(3);
    dynamic_tensor<T, 1> e;
    e = ee;
    passed &= TTL_CHECK(e[0] == 1);
    passed &= TTL_CHECK(e[1] == 2);
    passed &= TTL_CHECK(e[2] == 3);

    dynamic_tensor<T, 2> ff(1,1);
    ff(0,0) = T(1);
    dynamic_tensor<T, 2> f;
    f = ff;
    passed &= TTL_CHECK(f[0] == 1);

    dynamic_tensor<T, 2> gg(2,2);
    gg(0,0) = T(1);
    gg(0,1) = T(2);
    gg(1,0) = T(3);
    gg(1,1) = T(4);
    dynamic_tensor<T, 2> g;
    g = gg;
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);
    passed &= TTL_CHECK(g[2] == 3);
    passed &= TTL_CHECK(g[3] == 4);

    dynamic_tensor<T, 2> hh(2,2);
    hh(0,0) = T(1);
    hh(0,1) = T(2);
    hh(1,0) = T(3);
    hh(1,1) = T(4);
    dynamic_tensor<T, 2> h(1,1);
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

    dynamic_tensor<T, 0> a, aa;
    aa() = T(1);
    a = std::move(aa);
    passed &= TTL_CHECK(a[0] == 1);

    dynamic_tensor<T, 1> c(1), cc(1);
    cc(0) = T(1);
    c = std::move(cc);
    passed &= TTL_CHECK(c[0] == 1);

    dynamic_tensor<T, 1> d(2), dd(2);
    dd(0) = T(1);
    dd(1) = T(2);
    d = std::move(dd);
    passed &= TTL_CHECK(d[0] == 1);
    passed &= TTL_CHECK(d[1] == 2);

    dynamic_tensor<T, 1> e(3), ee(3);
    ee(0) = T(1);
    ee(1) = T(2);
    ee(2) = T(3);
    e = std::move(ee);
    passed &= TTL_CHECK(e[0] == 1);
    passed &= TTL_CHECK(e[1] == 2);
    passed &= TTL_CHECK(e[2] == 3);

    dynamic_tensor<T, 2> f(1,1), ff(1,1);
    ff(0,0) = T(1);
    f = std::move(ff);
    passed &= TTL_CHECK(f[0] == 1);

    dynamic_tensor<T, 2> g(1,2), gg(1,2);
    gg(0,0) = T(1);
    gg(0,1) = T(2);
    g = std::move(gg);
    passed &= TTL_CHECK(g[0] == 1);
    passed &= TTL_CHECK(g[1] == 2);

    dynamic_tensor<T, 2> h(2,2), hh(2,2);
    hh(0,0) = T(1);
    hh(0,1) = T(2);
    hh(1,0) = T(3);
    hh(1,1) = T(4);
    h = std::move(hh);
    passed &= TTL_CHECK(h[0] == 1);
    passed &= TTL_CHECK(h[1] == 2);
    passed &= TTL_CHECK(h[2] == 3);
    passed &= TTL_CHECK(h[3] == 4);

    return passed;
}

template <class T>
constexpr bool tests()
{
    bool passed = true;

    passed &= TTL_CHECK( concepts(types<T>)   );
    passed &= TTL_CHECK( default_ctor(types<T>)   );
    passed &= TTL_CHECK( linear_access(types<T>)   );
    passed &= TTL_CHECK( md_access(types<T>) );
    passed &= TTL_CHECK( copy_ctor(types<T>) );
    passed &= TTL_CHECK( move_ctor(types<T>) );
    passed &= TTL_CHECK( copy(types<T>) );
    passed &= TTL_CHECK( move(types<T>) );

    return passed;
}

int main()
{
#ifdef __clang__
#define constexpr
#endif
    constexpr bool i = tests<int>(); if (!i) throw;
    constexpr bool f = tests<float>(); if (!f) throw;
    constexpr bool d = tests<double>(); if (!d) throw;
}

