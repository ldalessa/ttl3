#include "ttl/ttl.hpp"
#include "common.hpp"
#include "static_tensor.hpp"
#include "dynamic_tensor.hpp"

ttl::index<'i'> i;
ttl::index<'j'> j;
ttl::index<'k'> k;


template <ttl::concepts::scalar T>
constexpr bool scalar_tests(T t)
{
    bool passed = true;
    return passed;
}

template <ttl::concepts::tensor T>
constexpr bool vector_tests(T t)
{
    static_assert(ttl::order_v<T> == 1);
    bool passed = true;

    ttl::concepts::shape auto e0 = ttl::extents(t);
    for (int i = 0; i < e0[0]; ++i) {
        t(i) = i;
    }

    ttl::concepts::bind_expression auto bind = t(i);
    ttl::concepts::shape auto e1 = ttl::extents(bind);

    passed &= TTL_CHECK( e0 == e1 );

    for (int i = 0; i < e0[0]; ++i) {
        passed &= TTL_CHECK( bind(i) == i);
    }

    return passed;
}

template <ttl::concepts::tensor T>
constexpr bool matrix_tests(T t)
{
    static_assert(ttl::order_v<T> == 2);
    bool passed = true;

    ttl::concepts::shape auto e0 = ttl::extents(t);
    for (int k = 0, i = 0; i < e0[0]; ++i) {
        for (int j = 0; j < e0[1]; ++j) {
            t(i,j) = k++;
        }
    }

    ttl::concepts::bind_expression auto bind = t(i,j);
    ttl::concepts::shape auto e1 = ttl::extents(bind);

    passed &= TTL_CHECK( e0 == e1 );

    for (int i = 0; i < e0[0]; ++i) {
        for (int j = 0; j < e0[1]; ++j) {
            passed &= TTL_CHECK( bind(i,j) == t(i,j));
        }
    }

    // project i
    for (int i = 0; i < e0[0]; ++i) {
        ttl::concepts::bind_expression auto bind = t(i,j);
        ttl::concepts::shape auto e1 = ttl::extents(bind);
        passed &= TTL_CHECK( e0[1] == e1[0] );
        static_assert(ttl::order_v<decltype(bind)> == 1);
        for (int j = 0; j < e0[1]; ++j) {
            passed &= TTL_CHECK( bind(j) == t(i,j));
        }
    }

    // project j
    for (int j = 0; j < e0[1]; ++j) {
        ttl::concepts::bind_expression auto bind = t(i,j);
        ttl::concepts::shape auto e1 = ttl::extents(bind);
        passed &= TTL_CHECK( e0[0] == e1[0] );
        static_assert(ttl::order_v<decltype(bind)> == 1);
        for (int i = 0; i < e0[0]; ++i) {
            passed &= TTL_CHECK( bind(i) == t(i,j));
        }
    }

    // transpose
    {
        ttl::concepts::bind_expression auto u = t(i,j);
        static_assert(ttl::concepts::tensor<decltype(u)>);
        ttl::concepts::bind_expression auto x = u(j,i);
        static_assert(ttl::concepts::tensor<decltype(x)>);
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
    if constexpr (ttl::concepts::has_static_extents<T>) {
        constexpr ttl::concepts::shape auto e = ttl::extents(t);
        if constexpr (e[0] == e[1]) {
            ttl::value_type_t<T> a{0};
            for (int i = 0; i < e[0]; ++i) {
                a += t(i,i);
            }
            ttl::value_type_t<T> b = t(i,~i);
            passed &= TTL_CHECK( a == b );
            ttl::value_type_t<T> c = t(~i,i);
            passed &= TTL_CHECK( a == c );
        }
    }
    else {
        if (e0[0] == e0[1]) {
            ttl::value_type_t<T> a{0};
            for (int i = 0; i < e0[0]; ++i) {
                a += t(i,i);
            }
            ttl::value_type_t<T> b = t(i,~i);
            passed &= TTL_CHECK( a == b );
            ttl::value_type_t<T> c = t(~i,i);
            passed &= TTL_CHECK( a == c );
        }
    }

    return passed;
}

template <class T>
constexpr bool static_tests()
{
    bool passed = true;
    ttl::tests::static_tensor<T, std::array<int, 0>{}> scalar;
    passed &= scalar_tests(scalar);

    ttl::tests::static_tensor<T, std::array{4}> vector;
    passed &= vector_tests(vector);

    ttl::tests::static_tensor<T, std::array{4,3}> matrix43;
    passed &= matrix_tests(matrix43);

    ttl::tests::static_tensor<T, std::array{3,4}> matrix34;
    passed &= matrix_tests(matrix34);

    ttl::tests::static_tensor<T, std::array{4,4}> matrix44;
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

    ttl::tests::dynamic_tensor<T, 2> matrix_128_16(128, 16);
    passed &= matrix_tests(matrix_128_16);

    ttl::tests::dynamic_tensor<T, 2> matrix_16_128(16, 128);
    passed &= matrix_tests(matrix_16_128);

    ttl::tests::dynamic_tensor<T, 2> matrix_128_128(128, 128);
    passed &= matrix_tests(matrix_128_128);

    return passed;
}

int main()
{
    //constexpr
    bool a = static_tests<double>(); if (!a) throw;
    // constexpr bool b = dynamic_tests<double>(); if (!b) throw;
}

