#include "Tensor.hpp"
#include "common.hpp"
#include "ttl/ttl.hpp"
#include "ttl/utils/type_args.hpp"

using ttl::tests::Tensor;
using ttl::utils::type_args;
using ttl::utils::types;

template <class T>
constexpr bool concepts(type_args<T> = {})
{
    static_assert(ttl::concepts::tensor<Tensor<T>>);
    static_assert(ttl::concepts::scalar<Tensor<T>>);
    static_assert(ttl::concepts::expression<Tensor<T>>);
    static_assert(ttl::concepts::tensor<Tensor<T, 1>>);
    static_assert(ttl::concepts::tensor<Tensor<T, 3>>);
    static_assert(ttl::concepts::tensor<Tensor<T, 1, 1>>);
    static_assert(ttl::concepts::tensor<Tensor<T, 3, 3>>);
    static_assert(ttl::concepts::tensor<Tensor<T, 3, 3, 3>>);
    static_assert(ttl::concepts::tensor<Tensor<T, 8, 8, 8, 8, 8, 8, 8, 8>>);
    return true;
}

template <class T>
constexpr bool default_ctor(type_args<T> = {})
{
  Tensor<T> a;
  Tensor<T, 1> b;
  Tensor<T, 3> c;
  Tensor<T, 1, 1> d;
  Tensor<T, 4, 4> e;
  Tensor<T, 8, 8, 8, 8, 8, 8, 8, 8> f;
  return true;
}

template <class T>
constexpr bool linear_access(type_args<T> = {})
{
  bool passed = true;

  Tensor<T> a;
  a[0] = 1;
  passed &= TTL_CHECK(a[0] == 1);

  Tensor<T, 1> b;
  b[0] = 1;
  TTL_CHECK(b[0] == 1);

  Tensor<T, 3> c;
  c[0] = 1;
  c[1] = 2;
  c[2] = 3;
  passed &= TTL_CHECK(c[0] == 1);
  passed &= TTL_CHECK(c[1] == 2);
  passed &= TTL_CHECK(c[2] == 3);

  Tensor<T, 1, 1> d;
  d[0] = 1;
  passed &= TTL_CHECK(d[0] == 1);

  Tensor<T, 2, 2> e;
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

  Tensor<T> a{ 1 };
  passed &= TTL_CHECK(a[0] == 1);

  Tensor<T, 1> b{ T(1) };
  passed &= TTL_CHECK(b[0] == 1);

  Tensor<T, 3> c{ T(1), T(2) };
  passed &= TTL_CHECK(c[0] == 1);
  passed &= TTL_CHECK(c[1] == 2);

  Tensor<T, 3> d{ T(1), T(2), T(3) };
  passed &= TTL_CHECK(d[0] == 1);
  passed &= TTL_CHECK(d[1] == 2);
  passed &= TTL_CHECK(d[2] == 3);

  Tensor<T, 1, 1> e{ T(1) };
  passed &= TTL_CHECK(e[0] == 1);

  Tensor<T, 2, 2> f{ T(1), T(2), T(3) };
  passed &= TTL_CHECK(f[0] == 1);
  passed &= TTL_CHECK(f[1] == 2);
  passed &= TTL_CHECK(f[2] == 3);

  Tensor<T, 2, 2> g{ T(1), T(2), T(3), T(4) };
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

  Tensor<T> aa{ T(1) };
  Tensor a = aa;
  passed &= TTL_CHECK(a[0] == 1);

  Tensor<T, 1> cc{ T(1) };
  Tensor c = cc;
  passed &= TTL_CHECK(c[0] == 1);

  Tensor<T, 3> dd{ T(1), T(2) };
  Tensor d = dd;
  passed &= TTL_CHECK(d[0] == 1);
  passed &= TTL_CHECK(d[1] == 2);

  Tensor<T, 3> ee{ T(1), T(2), T(3) };
  Tensor e = ee;
  passed &= TTL_CHECK(e[0] == 1);
  passed &= TTL_CHECK(e[1] == 2);
  passed &= TTL_CHECK(e[2] == 3);

  Tensor<T, 1, 1> ff{ T(1) };
  Tensor f = ff;
  passed &= TTL_CHECK(f[0] == 1);

  Tensor<T, 2, 2> gg{ T(1), T(2), T(3) };
  Tensor g = gg;
  passed &= TTL_CHECK(g[0] == 1);
  passed &= TTL_CHECK(g[1] == 2);
  passed &= TTL_CHECK(g[2] == 3);

  Tensor<T, 2, 2> hh{ T(1), T(2), T(3), T(4) };
  Tensor h = hh;
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

  Tensor<T> aa{ T(1) };
  Tensor a = std::move(aa);
  passed &= TTL_CHECK(a[0] == 1);

  Tensor<T, 1> cc{ T(1) };
  Tensor c = std::move(cc);
  passed &= TTL_CHECK(c[0] == 1);

  Tensor<T, 3> dd{ T(1), T(2) };
  Tensor d = std::move(dd);
  passed &= TTL_CHECK(d[0] == 1);
  passed &= TTL_CHECK(d[1] == 2);

  Tensor<T, 3> ee{ T(1), T(2), T(3) };
  Tensor e = std::move(ee);
  passed &= TTL_CHECK(e[0] == 1);
  passed &= TTL_CHECK(e[1] == 2);
  passed &= TTL_CHECK(e[2] == 3);

  Tensor<T, 1, 1> ff{ T(1) };
  Tensor f = std::move(ff);
  passed &= TTL_CHECK(f[0] == 1);

  Tensor<T, 2, 2> gg{ T(1), T(2), T(3) };
  Tensor g = std::move(gg);
  passed &= TTL_CHECK(g[0] == 1);
  passed &= TTL_CHECK(g[1] == 2);
  passed &= TTL_CHECK(g[2] == 3);

  Tensor<T, 2, 2> hh{ T(1), T(2), T(3), T(4) };
  Tensor h = std::move(hh);
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

  Tensor<T> aa{ T(1) };
  Tensor<T> a;
  a = aa;
  passed &= TTL_CHECK(a[0] == 1);

  Tensor<T, 1> cc{ T(1) };
  Tensor<T, 1> c;
  c = cc;
  passed &= TTL_CHECK(c[0] == 1);

  Tensor<T, 3> dd{ T(1), T(2) };
  Tensor<T, 3> d;
  d = dd;
  passed &= TTL_CHECK(d[0] == 1);
  passed &= TTL_CHECK(d[1] == 2);

  Tensor<T, 3> ee{ T(1), T(2), T(3) };
  Tensor<T, 3> e;
  e = ee;
  passed &= TTL_CHECK(e[0] == 1);
  passed &= TTL_CHECK(e[1] == 2);
  passed &= TTL_CHECK(e[2] == 3);

  Tensor<T, 1, 1> ff{ T(1) };
  Tensor<T, 1, 1> f;
  f = ff;
  passed &= TTL_CHECK(f[0] == 1);

  Tensor<T, 2, 2> gg{ T(1), T(2), T(3) };
  Tensor<T, 2, 2> g;
  g = gg;
  passed &= TTL_CHECK(g[0] == 1);
  passed &= TTL_CHECK(g[1] == 2);
  passed &= TTL_CHECK(g[2] == 3);

  Tensor<T, 2, 2> hh{ T(1), T(2), T(3), T(4) };
  Tensor<T, 2, 2> h;
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

  Tensor<T> a, aa{ T(1) };
  a = std::move(aa);
  passed &= TTL_CHECK(a[0] == 1);

  Tensor<T, 1> c, cc{ T(1) };
  c = std::move(cc);
  passed &= TTL_CHECK(c[0] == 1);

  Tensor<T, 3> d, dd{ T(1), T(2) };
  d = std::move(dd);
  passed &= TTL_CHECK(d[0] == 1);
  passed &= TTL_CHECK(d[1] == 2);

  Tensor<T, 3> e, ee{ T(1), T(2), T(3) };
  e = std::move(ee);
  passed &= TTL_CHECK(e[0] == 1);
  passed &= TTL_CHECK(e[1] == 2);
  passed &= TTL_CHECK(e[2] == 3);

  Tensor<T, 1, 1> f, ff{ T(1) };
  f = std::move(ff);
  passed &= TTL_CHECK(f[0] == 1);

  Tensor<T, 2, 2> g, gg{ T(1), T(2), T(3) };
  g = std::move(gg);
  passed &= TTL_CHECK(g[0] == 1);
  passed &= TTL_CHECK(g[1] == 2);
  passed &= TTL_CHECK(g[2] == 3);

  Tensor<T, 2, 2> h, hh{ T(1), T(2), T(3), T(4) };
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

  Tensor<T> a;
  a() = 1;
  passed &= TTL_CHECK(a[0] == 1);

  Tensor<T, 1> c;
  c(0) = 1;
  TTL_CHECK(c[0] == 1);

  Tensor<T, 3> d;
  d(0) = 1;
  d(1) = 2;
  d(2) = 3;
  passed &= TTL_CHECK(d[0] == 1);
  passed &= TTL_CHECK(d[1] == 2);
  passed &= TTL_CHECK(d[2] == 3);

  Tensor<T, 1, 1> e;
  e(0,0) = 1;
  passed &= TTL_CHECK(e[0] == 1);

  Tensor<T, 2, 2> f;
  f(0,0) = 1;
  f(0,1) = 2;
  f(1,0) = 3;
  f(1,1) = 4;
  passed &= TTL_CHECK(f[0] == 1);
  passed &= TTL_CHECK(f[1] == 2);
  passed &= TTL_CHECK(f[2] == 3);
  passed &= TTL_CHECK(f[3] == 4);

  Tensor<T, 2, 2, 2> g;
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
  constexpr bool i = tests<int>();
  constexpr bool f = tests<float>();
  constexpr bool d = tests<double>();
}

