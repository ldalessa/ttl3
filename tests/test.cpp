#include "ttl/ttl.hpp"
#include <array>
#include <concepts>
#include <numeric>

consteval auto non_zero_extents(std::same_as<int> auto... is) -> bool {
    return (is && ... && 1);
}

template <class T, int... _extents>
struct md_array;

template <class T, int... _extents>
requires (non_zero_extents(_extents...))
struct md_array<T, _extents...>
{
    using md_array_tag = void;

    static constexpr int M = (_extents * ... * 1);
    static constexpr auto _strides = [] {
        std::array<int, sizeof...(_extents)> extents = { _extents... }, strides;
        std::exclusive_scan(extents.begin(), extents.end(), strides.begin(), 0);
        return strides;
    }();

    T _data[M];                            // at least one element for 0th order

    static consteval auto order() -> int {
        return sizeof...(_extents);
    }

    constexpr auto operator[](std::integral auto... is) const -> T const& {
        return _at(*this, is...);
    }

    constexpr auto operator[](std::integral auto... is) const -> T& {
        return _at(*this, is...);
    }

    static constexpr auto _at(auto&& self, std::integral auto... is) -> decltype(auto)
        requires (sizeof...(is) == sizeof...(_extents))
    {
        int i = 0;
        int n = ((is * self._strides[i++]) + ...);
        return self._data[n];
    }
};

namespace ttl::traits
{
    template <class T, int... extents>
    struct order<md_array<T, extents...>>
    {
        static constexpr int value = sizeof...(extents);
    };

    template <class T, int... extents>
    struct scalar_type<md_array<T, extents...>>
    {
        using type = T;
    };
}

static_assert(ttl::traits::order_v<int> == 0);
static_assert(ttl::traits::order_v<double> == 0);
static_assert(ttl::traits::order_v<md_array<double>> == 0);
static_assert(ttl::traits::order_v<md_array<double, 1>> == 1);
static_assert(ttl::traits::order_v<md_array<double, 2, 2>> == 2);
static_assert(std::same_as<ttl::traits::scalar_type_t<int>, int>);
static_assert(std::same_as<ttl::traits::scalar_type_t<double>, double>);
static_assert(std::same_as<ttl::traits::scalar_type_t<md_array<double>>, double>);
static_assert(ttl::concepts::bindable<int>);
static_assert(ttl::concepts::bindable<double>);
static_assert(ttl::concepts::bindable<md_array<double>>);
// using namespace ttl::grammar;

template <int...>
struct Tensor;

namespace ttl::traits
{
    template <int... extents>
    struct order<Tensor<extents...>>
    {
        static constexpr int value = sizeof...(extents);
    };

    template <int... extents>
    struct scalar_type<Tensor<extents...>>
    {
        using type = double;
    };
}

template <int... extents>
struct Tensor : md_array<double, extents...>,
    ttl::Bindable<Tensor<extents...>>
{
    using md_array_t = md_array<double, extents...>;

    constexpr auto evaluate(ttl::ScalarIndex<sizeof...(extents)>) const {
        return 0;
    }
};

template <auto>
[[gnu::deprecated]]
void print() {}

int main()
{
    Tensor<1, 1, 1> A;
    static_assert(ttl::concepts::bindable<decltype(A)>);

    auto i = ttl::index<'i'>;
    auto μ = ttl::index<U'μ'>;
    int n = 1;
    auto B = A(~i, i, μ);
    // auto C = B(n);
    auto D = B + B + 1;
    // auto E = D(1);

    // ttl::Index<'i'> i;
    // albert::Index<'j'> j;
    // albert::Tensor<double, 1, 3> a = { 1, 2, 3 };
    // a(0) = 1;
    // albert::Tensor<double, 1, 3> b;
    // albert::Tensor<double, 0, 0> c = { 2 };
    // albert::Tensor<double, 2, 3> A = {
    //   1, 2, 3,
    //   4, 5, 6,
    //   7, 8, 9
    // };

    // a(j) += A(j,i)*a(i);
}
