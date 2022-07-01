#pragma once

#include "ttl/expression_traits.hpp"
#include "ttl/utils.hpp"
#include <type_traits>

namespace ttl
{
    template <class> struct bindable;

    template <is_expression A, is_expression B, auto _op>
    struct sum : bindable<sum<A, B, _op>>
    {
        using scalar_type = std::common_type_t<scalar_type_t<A>, scalar_type_t<B>>;

        static constexpr tensor_index _outer_a = outer<A>;
        static constexpr tensor_index _outer_b = outer<B>;
        static constexpr int _order = _outer_a.size();

        static_assert(is_permutation_of<_outer_a, _outer_b>);

        A _a;
        B _b;

        constexpr sum(auto&& a, auto&& b, nttp_args<_op>)
                : _a(std::forward<A>(a))
                , _b(std::forward<B>(b))
        {
        }

        constexpr operator scalar_type() const requires(_order == 0) {
            return evaluate(typed_index<_outer_a>{});
        }

        static consteval auto get_outer() -> tensor_index<_order> {
            tensor_index<_order> outer;
            int i = 0;
            for (auto&& [c, t] : _outer_a) {
                for (auto&& [cʹ, tʹ] : _outer_b) {
                    if (c == cʹ) {
                        int synthetic = (t & SYNTHETIC) ^ (tʹ & SYNTHETIC);
                        outer[i++] = _index(c, t &~ SYNTHETIC + synthetic);
                        break;
                    }
                }
            }
            if (i != _order) throw "failed to properly initialize outer index";
            return outer;
        }

        constexpr auto get_extents() const -> array<int, _order>
        {
            // The extents map tells us if the extent should come from the left
            // or right child, based on if one is synthetic and one isn't
            constexpr auto map = _get_extents_map();
            is_extents_of_order<_order> auto a = ttl::extents(_a);
            is_extents_of_order<_order> auto b = ttl::extents(_b);
            array<int, _order> extents;
            for (int i = 0; i < _order; ++i) {
                extents[i] = (map.as[i]) ? b[map.is[i]] : a[map.is[i]];
            }
            return extents;
        }

        constexpr auto evaluate(typed_index<_outer_a> const& outer) const -> decltype(auto) {
            using a_index = typed_index<_outer_a>;
            using b_index = typed_index<_outer_b>;
            return _op(ttl::evaluate(_a, a_index(outer)), ttl::evaluate(_b, b_index(outer)));
        }

        static constexpr /* consteval */ auto _get_extents_map()
        {
            struct extent_map {
                array<int, _order> as;
                array<int, _order> is;
            };

            extent_map map;
            int i = 0;
            for (auto&& [c, t] : _outer_a) {
                if (t & SYNTHETIC) {
                    for (int j = 0; auto&& [cʹ, tʹ] : _outer_b) {
                        if (c == cʹ) {
                            map.as[i] = 1;
                            map.is[i] = j;
                            break;
                        }
                        ++j;
                    }
                }
                else {
                    map.as[i] = 0;
                    map.is[i] = i;
                }
            }
            return map;
        }
    };

    template <is_expression A, is_expression B, auto op>
    sum(A&&, B&&, nttp_args<op>) -> sum<A, B, op>;
}
