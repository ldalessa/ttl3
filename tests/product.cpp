#include "common.hpp"
#include "row_major.hpp"
#include "static_tensor.hpp"
#include "ttl/ttl.hpp"

using namespace ttl::tests;

// scalar-scalar tests
constexpr auto test(ttl::is_scalar auto x, ttl::is_scalar auto y) -> bool
{
    bool passed = true;
    x() = 2;
    y() = 3;

    {
        ttl::is_scalar auto z = x * y;
        passed &= TTL_CHECK( z == 6 );
    }

    {
        ttl::is_scalar auto z = x * 4;
        passed &= TTL_CHECK( z == 8 );
    }

    {
        ttl::is_scalar auto z = x * y * 10;
        passed &= TTL_CHECK( z == 60 );
    }

    return passed;
}

// scalar-vector tests
constexpr auto test(ttl::is_tensor_of_order<1> auto x, ttl::is_scalar auto y) -> bool
{
    bool passed = true;
    ttl::is_extents auto extents = ttl::extents(x);
    int M = extents[0];
    for (int i = 0; i < M; ++i) {
        x(i) = i;
    }
    y() = 2;

    ttl::index<'i'> i;

    {
        ttl::is_tensor_of_order<1> auto z = x(i) * y;
        for (int i = 0; i < M; ++i) {
            passed &= TTL_CHECK( z(i) == 2 * i );
        }
    }

    {
        ttl::is_tensor_of_order<1> auto z = y * x(i);
        for (int i = 0; i < M; ++i) {
            passed &= TTL_CHECK( z(i) == 2 * i );
        }
    }

    {
        ttl::is_tensor_of_order<1> auto z = x(i) * 2;
        for (int i = 0; i < M; ++i) {
            passed &= TTL_CHECK( z(i) == 2 * i );
        }
    }

    {
        ttl::is_tensor_of_order<1> auto z = 2 * x(i);
        for (int i = 0; i < M; ++i) {
            passed &= TTL_CHECK( z(i) == 2 * i );
        }
    }

    {
        ttl::is_tensor_of_order<1> auto z = 2 * x(i) * y;
        for (int i = 0; i < M; ++i) {
            passed &= TTL_CHECK( z(i) == 4 * i );
        }
    }

    return passed;
}

constexpr auto test(ttl::is_tensor_of_order<1> auto x) -> bool
{
    bool passed = true;
    ttl::is_extents_of_order<1> auto x_extents = ttl::extents(x);

    int M = x_extents[0];
    for (int i = 0; i < M; ++i) {
        x(i) = i;
    }


    // inner product
    {
        ttl::index<'i'> i;
        ttl::is_scalar auto z = x(i) * x(~i);
        ttl::value_type_t<decltype(x)> inner{0};
        for (int i = 0; i < M; ++i) {
            inner += x(i) * x(i);
        }
        passed &= TTL_CHECK( inner == z );
    }

    // outer product
    {
        ttl::index<'i'> i;
        ttl::index<'j'> j;
        ttl::is_tensor_of_order<2> auto z = x(i) * x(j);
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                passed &= TTL_CHECK( z(i,j) == x(i) * x(j) );
            }
        }
    }

    return passed;
}

constexpr auto test(ttl::is_tensor_of_order<1> auto x, ttl::is_tensor_of_order<1> auto y) -> bool
{
    bool passed = true;
    ttl::is_extents_of_order<1> auto x_extents = ttl::extents(x);
    ttl::is_extents_of_order<1> auto y_extents = ttl::extents(y);

    int M = x_extents[0];
    for (int i = 0; i < M; ++i) {
        x(i) = i;
    }

    int N = y_extents[0];
    for (int i = 0; i < N; ++i) {
        y(i) = i;
    }

    ttl::index<'i'> i;
    ttl::index<'j'> j;

    ttl::is_tensor_of_order<2> auto z = x(i) * y(j);
    ttl::is_extents_of_order<2> auto z_extents = ttl::extents(z);
    passed &= TTL_CHECK( z_extents[0] == M );
    passed &= TTL_CHECK( z_extents[1] == N );

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            passed &= TTL_CHECK( z(i,j) == x(i) * y(j) );
        }
    }

    return passed;
}

constexpr auto test(ttl::is_tensor_of_order<2> auto x) -> bool {
    bool passed = true;
    ttl::is_extents_of_order<2> auto extents = ttl::extents(x);

    int M = extents[0];
    int N = extents[1];

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            x(i,j) = i * N + j;
        }
    }

    // inner product
    {
        ttl::index<'i'> i;
        ttl::index<'j'> j;
        ttl::is_scalar auto y = x(i,j) * x(~i,~j);
        ttl::value_type_t<decltype(y)> inner{0};
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                inner += x(i,j) * x(i,j);
            }
        }
        passed &= TTL_CHECK( inner == y );
    }

    // outer product
    {
        ttl::index<'i'> i;
        ttl::index<'j'> j;
        ttl::index<'k'> k;
        ttl::index<'l'> l;
        ttl::is_tensor_of_order<4> auto y = x(i,j) * x(k,l);
        ttl::is_extents_of_order<4> auto e = ttl::extents(y);
        passed &= TTL_CHECK( e[0] == M );
        passed &= TTL_CHECK( e[1] == N );
        passed &= TTL_CHECK( e[2] == M );
        passed &= TTL_CHECK( e[3] == N );
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < M; ++k) {
                    for (int l = 0; l < N; ++l) {
                        passed &= TTL_CHECK( y(i,j,k,l) ==  x(i,j) * x(k,l) );
                    }
                }
            }
        }
    }

    // contract
    {
        ttl::index<'i'> i;
        ttl::index<'j'> j;
        ttl::index<'k'> k;
        ttl::is_tensor_of_order<2> auto y = x(i,k) * x(j,~k);
        ttl::is_extents_of_order<2> auto e = ttl::extents(y);
        passed &= TTL_CHECK( e[0] == M );
        passed &= TTL_CHECK( e[1] == M );

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                ttl::value_type_t<decltype(y)> a{0};
                for (int k = 0; k < N; ++k) {
                    a += x(i,k) * x(j,k);
                }
                passed &= TTL_CHECK( y(i,j) == a );
            }
        }
    }

    // contract
    {
        ttl::index<'i'> i;
        ttl::index<'j'> j;
        ttl::index<'k'> k;
        ttl::is_tensor_of_order<2> auto y = x(k,i) * x(~k,j);
        ttl::is_extents_of_order<2> auto e = ttl::extents(y);
        passed &= TTL_CHECK( e[0] == N );
        passed &= TTL_CHECK( e[1] == N );

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                ttl::value_type_t<decltype(y)> a{0};
                for (int k = 0; k < M; ++k) {
                    a += x(k,i) * x(k,j);
                }
                passed &= TTL_CHECK( y(i,j) == a );
            }
        }
    }

    return passed;
}

template <class T>
constexpr bool static_tensor_tests()
{
    bool passed = true;

    static_tensor<T, row_major{}> scalar;
    static_tensor<T, row_major{3}> vector3;
    static_tensor<T, row_major{4}> vector4;
    static_tensor<T, row_major{3,3}> matrix33;
    static_tensor<T, row_major{3,4}> matrix34;

    passed &= TTL_CHECK( test(scalar, scalar) );
    passed &= TTL_CHECK( test(vector3, scalar) );
    passed &= TTL_CHECK( test(vector3) );
    passed &= TTL_CHECK( test(vector3, vector4) );
    passed &= TTL_CHECK( test(vector4, vector3) );
    passed &= TTL_CHECK( test(matrix33) );
    passed &= TTL_CHECK( test(matrix34) );

    return passed;
}

int main()
{
    //constexpr
    bool a = static_tensor_tests<int>(); if (!a) throw;
}
