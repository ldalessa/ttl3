#include "common.hpp"
#include "dynamic_tensor.hpp"
#include "row_major.hpp"
#include "static_tensor.hpp"
#include "ttl/ttl.hpp"

using namespace ttl::tests;

constexpr auto Axpy(
        ttl::is_tensor_of_order<2> auto A,
        ttl::is_tensor_of_order<1> auto x,
        ttl::is_tensor_of_order<1> auto y) -> bool
{
    bool passed = true;

    using T = std::common_type_t<ttl::scalar_type_t<decltype(A)>,
                                 ttl::scalar_type_t<decltype(x)>,
                                 ttl::scalar_type_t<decltype(y)>>;

    ttl::is_extents_of_order<2> auto eA = ttl::extents(A);
    ttl::is_extents_of_order<1> auto ex = ttl::extents(x);
    ttl::is_extents_of_order<1> auto ey = ttl::extents(y);

    int M = eA[0];
    int N = eA[1];

    passed &= TTL_CHECK( eA[0] == ey[0] );
    passed &= TTL_CHECK( eA[1] == ex[0] );

    ttl::index<'i'> i;
    ttl::index<'j'> j;
    ttl::is_tensor_of_order<1> auto z = A(i,~j) * x(j) + y(i);

    for (int i = 0; i < M; ++i) {
        T a = y(i);
        for (int j = 0; j < N; ++j) {
            a += A(i,j) * x(j);
        }
        T b = z(i);
        passed &= TTL_CHECK( a == b );
    }

    return passed;
}

template <class T>
constexpr auto static_tensor_tests() -> bool
{
    bool passed = true;

    static_tensor<T, row_major{3,4}> A = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11
    };
    static_tensor<T, row_major{4}> x {
        1, 2, 3, 4
    };
    static_tensor<T, row_major{3}> y {
        1, 2, 3
    };

    passed &= Axpy(A, x, y);
    return passed;
}

template <class T>
constexpr auto dynamic_tensor_tests() -> bool
{
    bool passed = true;

    dynamic_tensor<T, 2> A(3,4);
    A(0,0) = 0;
    A(0,1) = 1;
    A(0,2) = 2;
    A(0,3) = 3;
    A(1,0) = 4;
    A(1,1) = 5;
    A(1,2) = 6;
    A(1,3) = 7;
    A(2,0) = 8;
    A(2,1) = 9;
    A(2,2) = 10;
    A(2,3) = 11;

    dynamic_tensor<T, 1> x(4);
    x(0) = 1;
    x(1) = 2;
    x(2) = 3;
    x(3) = 4;

    dynamic_tensor<T, 1> y(3);
    y(0) = 1;
    y(1) = 2;
    y(2) = 3;

    passed &= Axpy(A, x, y);
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
}
