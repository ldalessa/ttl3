#include "common.hpp"
#include "dynamic_tensor.hpp"
#include "row_major.hpp"
#include "static_tensor.hpp"
#include "ttl/ttl.hpp"

using namespace ttl::tests;

constexpr bool scalar_tests(ttl::is_scalar auto x, ttl::is_scalar auto y)
{
    x() = 1;
    y() = 2;
    bool passed = true;

    {
        ttl::is_scalar auto z = x + y;
        passed &= TTL_CHECK( z == 3);
    }

    {
        ttl::is_scalar auto z = x - y;
        passed &= TTL_CHECK( z == -1 );
    }

    {
        ttl::is_scalar auto z = x + 1;
        passed &= TTL_CHECK( z == 2 );
    }

    {
        ttl::is_scalar auto z = x - 1;
        passed &= TTL_CHECK( z == 0 );
    }

    {
        ttl::is_scalar auto z = x + y - 3;
        passed &= TTL_CHECK( z == 0 );
    }

    return passed;
}

constexpr bool vector_tests(ttl::is_tensor_of_order<1> auto x, ttl::is_tensor_of_order<1> auto y)
{
    bool passed = true;

    auto ex = ttl::extents(x);
    auto ey = ttl::extents(y);
    passed &= TTL_CHECK( std::equal(ex.begin(), ex.end(), ey.begin(), ey.end()) );

    int M = ex[0];

    for (int i = 0; i < M; ++i) {
        x(i) = 1;
        y(i) = 2;
    }

    ttl::index<'i'> i;

    {
        ttl::is_tensor_of_order<1> auto z = x(i) + y(i);
        for (int i = 0; i < M; ++i) {
            passed &= TTL_CHECK( z(i) == 3);
        }
    }

    {
        ttl::is_tensor_of_order<1> auto z = x(i) - y(i);
        for (int i = 0; i < M; ++i) {
            passed &= TTL_CHECK( z(i) == -1 );
        }
    }

    {
        ttl::is_tensor_of_order<1> auto z = x(i) + y(i) - x(i);
        for (int i = 0; i < M; ++i) {
            passed &= TTL_CHECK( z(i) == y(i) );
        }
    }

    return passed;
}

constexpr bool matrix_tests(ttl::is_tensor_of_order<2> auto x, ttl::is_tensor_of_order<2> auto y)
{
    bool passed = true;

    auto ex = ttl::extents(x);
    auto ey = ttl::extents(y);
    passed &= TTL_CHECK( std::equal(ex.begin(), ex.end(), ey.begin(), ey.end()) );

    int M = ex[0];
    int N = ex[1];

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            x(i,j) = (i * N + j);
            y(i,j) = (i * N + j) + 1;
        }
    }

    ttl::index<'i'> i;
    ttl::index<'j'> j;

    {
        ttl::is_tensor_of_order<2> auto z = x(i,j) + y(i,j);
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                passed &= TTL_CHECK( z(i,j) == 2 * (i * N + j) + 1 );
            }
        }
    }

    {
        ttl::is_tensor_of_order<2> auto z = x(i,j) - y(i,j);
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                passed &= TTL_CHECK( z(i,j) == -1 );
            }
        }
    }

    {
        ttl::is_tensor_of_order<2> auto z = x(i,j) + y(i,j) - x(i,j);
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                passed &= TTL_CHECK( z(i,j) == y(i,j) );
            }
        }
    }

    if (M == N) {
        ttl::is_tensor_of_order<2> auto z = x(i,j) + y(j,i);
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                passed &= TTL_CHECK( z(i,j) == (j * M + i) +  (i * N + j) + 1 );
            }
        }
    }


    return passed;
}

template <class T>
constexpr bool static_tensor_tests()
{
    bool passed = true;

    {
        using scalar = static_tensor<T, row_major{}>;
        scalar x, y;
        passed &= TTL_CHECK( scalar_tests(x, y) );
    }

    {
        using vector = static_tensor<T, row_major{3}>;
        vector x, y;
        passed &= TTL_CHECK( vector_tests(x, y) );
    }

    {
        using matrix = static_tensor<T, row_major{3,3}>;
        matrix x, y;
        passed &= TTL_CHECK( matrix_tests(x, y) );
    }

    {
        using matrix = static_tensor<T, row_major{3,4}>;
        matrix x, y;
        passed &= TTL_CHECK( matrix_tests(x, y) );
    }

    {
        using matrix = static_tensor<T, row_major{4,3}>;
        matrix x, y;
        passed &= TTL_CHECK( matrix_tests(x, y) );
    }

    return passed;
}

template <class T>
constexpr bool dynamic_tensor_tests()
{
    bool passed = true;

    {
        using scalar = dynamic_tensor<T, 0>;
        scalar x, y;
        passed &= TTL_CHECK( scalar_tests(x, y) );
    }

    {
        using vector = dynamic_tensor<T, 1>;
        vector x(8), y(8);
        passed &= TTL_CHECK( vector_tests(x, y) );
    }

    {
        using matrix = dynamic_tensor<T, 2>;
        matrix x(8,8), y(8,8);
        passed &= TTL_CHECK( matrix_tests(x, y) );
    }

    {
        using matrix = dynamic_tensor<T, 2>;
        matrix x(5,8), y(5,8);
        passed &= TTL_CHECK( matrix_tests(x, y) );
    }

    {
        using matrix = dynamic_tensor<T, 2>;
        matrix x(8,5), y(8,5);
        passed &= TTL_CHECK( matrix_tests(x, y) );
    }

    return passed;
}

template <class T>
constexpr bool mixed_tensor_tests()
{
    bool passed = true;

    {
        static_tensor<T, row_major{}> x;
        dynamic_tensor<T, 0> y;
        passed &= scalar_tests(x, y);
        passed &= scalar_tests(y, x);
    }

    {
        static_tensor<T, row_major{8}> x;
        dynamic_tensor<T, 1> y(8);
        passed &= TTL_CHECK( vector_tests(x, y) );
        passed &= TTL_CHECK( vector_tests(y, x) );
    }

    {
        static_tensor<T, row_major{8,8}> x;
        dynamic_tensor<T, 2> y(8,8);
        passed &= TTL_CHECK( matrix_tests(x, y) );
        passed &= TTL_CHECK( matrix_tests(y, x) );
    }

    {
        static_tensor<T, row_major{4,8}> x;
        dynamic_tensor<T, 2> y(4,8);
        passed &= TTL_CHECK( matrix_tests(x, y) );
        passed &= TTL_CHECK( matrix_tests(y, x) );
    }

    {
        static_tensor<T, row_major{8,7}> x;
        dynamic_tensor<T, 2> y(8,7);
        passed &= TTL_CHECK( matrix_tests(x, y) );
        passed &= TTL_CHECK( matrix_tests(y, x) );
    }

    return passed;
}

int main()
{
    constexpr bool a = static_tensor_tests<int>(); if (!a) throw;
    constexpr bool b = static_tensor_tests<double>(); if (!a) throw;
#ifdef __clang__
    #define constexpr
#endif
    constexpr bool c = dynamic_tensor_tests<int>(); if (!a) throw;
    constexpr bool d = dynamic_tensor_tests<double>(); if (!a) throw;

    constexpr bool e = mixed_tensor_tests<int>(); if (!a) throw;
    constexpr bool f = mixed_tensor_tests<double>(); if (!a) throw;
}
