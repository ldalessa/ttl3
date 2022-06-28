#include "ttl/ttl.hpp"
#include "common.hpp"
#include "dynamic_tensor.hpp"
#include "static_tensor.hpp"
#include "row_major.hpp"
#include <algorithm>
#include <array>

ttl::index<'i'> i;
ttl::index<'j'> j;
ttl::index<'k'> k;

template <ttl::is_scalar T>
constexpr bool scalar_tests(T&& t)
{
    bool passed = true;
    t() = 1;
    passed &= TTL_CHECK( t[0] == 1 );
    passed &= TTL_CHECK( t[0] == t() );
    return passed;
}

template <ttl::is_tensor T>
constexpr bool vector_tests(T&& t)
{
    static_assert(ttl::order<T> == 1);
    bool passed = true;

    auto e0 = ttl::extents(t);
    for (int i = 0; i < e0[0]; ++i) {
        t(i) = i;
    }

    auto bind = t(i);
    auto e1 = ttl::extents(bind);

    passed &= TTL_CHECK( std::equal(e0.begin(), e0.end(), e1.begin(), e1.end()) );

    for (int i = 0; i < e0[0]; ++i) {
        passed &= TTL_CHECK( bind(i) == i);
    }

    for (int i = 0; i < e0[0]; ++i) {
        bind(i)++;
    }

    for (int i = 0; i < e0[0]; ++i) {
        passed &= TTL_CHECK( bind(i) == i + 1);
    }

    return passed;
}

template <ttl::is_tensor T>
constexpr bool matrix_tests(T&& t)
{
    static_assert(ttl::order<T> == 2);
    bool passed = true;

    auto e0 = ttl::extents(t);
    for (int k = 0, i = 0; i < e0[0]; ++i) {
        for (int j = 0; j < e0[1]; ++j) {
            t(i,j) = k++;
        }
    }

    auto bind = t(i,j);
    auto e1 = ttl::extents(bind);

    passed &= TTL_CHECK( std::equal(e0.begin(), e0.end(), e1.begin(), e1.end()) );

    for (int i = 0; i < e0[0]; ++i) {
        for (int j = 0; j < e0[1]; ++j) {
            passed &= TTL_CHECK( bind(i,j) == t(i,j));
        }
    }

    // project i
    for (int i = 0; i < e0[0]; ++i) {
        auto bind = t(i,j);
        auto e1 = ttl::extents(bind);
        passed &= TTL_CHECK( e0[1] == e1[0] );
        static_assert(ttl::order<decltype(bind)> == 1);
        for (int j = 0; j < e0[1]; ++j) {
            passed &= TTL_CHECK( bind(j) == t(i,j));
        }
    }

    // project j
    for (int j = 0; j < e0[1]; ++j) {
        auto bind = t(i,j);
        auto e1 = ttl::extents(bind);
        passed &= TTL_CHECK( e0[0] == e1[0] );
        static_assert(ttl::order<decltype(bind)> == 1);
        for (int i = 0; i < e0[0]; ++i) {
            passed &= TTL_CHECK( bind(i) == t(i,j));
        }
    }

    // transpose
    {
        auto u = t(i,j);
        static_assert(ttl::is_tensor<decltype(u)>);
        auto x = u(j,i);
        static_assert(ttl::is_tensor<decltype(x)>);
        for (int i = 0; i < e0[0]; ++i) {
            for (int j = 0; j < e0[1]; ++j) {
                auto a = t(i,j);
                auto b = u(i,j);
                auto c = x(j,i);
                passed &= TTL_CHECK( a == b );
                passed &= TTL_CHECK( a == c );
            }
        }
    }

    // trace
    if (e0[0] == e0[1]) {
        ttl::scalar_type_t<T> a{0};
        for (int i = 0; i < e0[0]; ++i) {
            a += t(i,i);
        }
        ttl::scalar_type_t<T> b = t(i,~i);
        passed &= TTL_CHECK( a == b );
        ttl::scalar_type_t<T> c = t(~i,i);
        passed &= TTL_CHECK( a == c );
    }

    return passed;
}

template <class T>
constexpr bool static_tests()
{
    bool passed = true;
    ttl::tests::static_tensor<T, ttl::tests::row_major{}> scalar;
    passed &= scalar_tests(scalar);

    ttl::tests::static_tensor<T, ttl::tests::row_major{4}> vector;
    passed &= vector_tests(vector);

    ttl::tests::static_tensor<T, ttl::tests::row_major{4,3}> matrix43;
    passed &= matrix_tests(matrix43);

    ttl::tests::static_tensor<T, ttl::tests::row_major{3,4}> matrix34;
    passed &= matrix_tests(matrix34);

    ttl::tests::static_tensor<T, ttl::tests::row_major{4,4}> matrix44;
    passed &= matrix_tests(matrix44);

    return passed;
}

template <class T>
constexpr bool dynamic_tests()
{
    bool passed = true;
    ttl::tests::dynamic_tensor<T, 0> scalar;
    passed &= scalar_tests(scalar);

    ttl::tests::dynamic_tensor<T, 1> vector(128);
    passed &= vector_tests(vector);

    ttl::tests::dynamic_tensor<T, 2> matrix_16_16(16, 16);
    passed &= matrix_tests(matrix_16_16);

    ttl::tests::dynamic_tensor<T, 2> matrix_16_11(16, 11);
    passed &= matrix_tests(matrix_16_11);

    ttl::tests::dynamic_tensor<T, 2> matrix_11_16(11, 16);
    passed &= matrix_tests(matrix_11_16);

    return passed;
}

int main()
{
    constexpr bool a = static_tests<double>(); if (!a) throw;
#ifdef __clang__
    bool b = dynamic_tests<double>(); if (!b) throw;
#else
    constexpr bool b = dynamic_tests<double>(); if (!b) throw;
#endif
}

