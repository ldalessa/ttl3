#pragma once

#include "ttl/carry_sum_add.hpp"
#include "ttl/scalar_index.hpp"
#include "ttl/tensor_index.hpp"
#include "ttl/concepts/bind_expression.hpp"
#include "ttl/concepts/shape.hpp"
#include "ttl/concepts/tensor.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/traits/extents.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/outer.hpp"
#include "ttl/traits/value_type.hpp"

namespace ttl
{
    template <concepts::has_order> struct bindable;
}

namespace ttl::expressions
{
    template <concepts::tensor A, concepts::tensor_index auto _index>
    struct bind : bindable<bind<A, _index>>
    {
        using bind_expression_concept = void;
        using value_type = value_type_t<A>;

        static constexpr int _order = _index.n_unique();
        static constexpr tensor_index _outer = unique_v<_index>;
        static constexpr tensor_index _contracted = contracted_v<_index>;
        static constexpr tensor_index _inner = concat_v<_outer, _contracted>;

        using outer_index_t     = scalar_index<_order>;
        using projected_index_t = scalar_index<_index.size()>;
        using inner_index_t     = scalar_index<_inner.size()>;

        A _a;
        projected_index_t _projected;

        // NB: b should be concept::tensor, but it doesn't seem to work given my
        //     concept soup ("not work" means infinite concept recursion in
        //     tag_invoke).
        constexpr bind(auto&& b, concepts::index_or_integral auto... is)
                : _a(std::forward<A>(b))
                , _projected(is...)
        {
            if constexpr (concepts::has_static_extents<A>) {
                constexpr bool valid = _validate_contracted_shape(ttl::extents_v<A>);
                (void)valid;
            }
        }

        constexpr static auto outer()
        {
            return _outer;
        }

        /// If the bind is a scalar, it should be something that can be
        /// converted to the underlying type.
        constexpr operator value_type() const requires(_order == 0)
        {
            return _evaluate(*this, scalar_index<0>{});
        }

        /// Map extents from the bound tensor.
        ///
        /// Basically, if I have a tensor that has extents like [3][3][10], and
        /// the bind expression contracts the first two ranks, then the bind has
        /// a single extent of 10.
        ///
        /// @param a_extents The shape of the child.
        ///
        /// @returns The shape of the bind tensor.
        constexpr static auto _extents(concepts::shape auto&& a_extents)
            -> std::array<int, _order>
        {
            constexpr auto&& map = index_to_index_map_v<_outer, _index>;
            std::array<int, _order> out;
            for (int i = 0; i < _order; ++i) {
                out[i] = a_extents[map[i]];
            }
            return out;
        }

        /// Evaluate the bind for a specific scalar index.
        ///
        /// This might simply pass the index through to the child, it might need
        /// to shuffle the index, it might need to expand the index to inject
        /// the projected ranks, and it might need to perform a full contraction
        /// of one or more dimensions.
        ///
        /// @param self  A forwarding reference formed from *this.
        /// @param index The index we would like to evaluate.
        constexpr static auto _evaluate(auto&& self, outer_index_t const& index)
            -> decltype(auto)
        {
            constexpr auto inner_extents = [](int i) {
                return _map_inner_extent(i);
            };

            constexpr auto mapped = [] {
                std::array<int, _index.size()> out;
                for (int i = 0; auto&& [c, t] : _index) {
                    if (auto j = _inner.index_of(c)) {
                        out[i++] = *j;
                    }
                    else {
                        if (t != IGNORE) throw "index not found";
                        out[i++] = 0;
                    }
                }
                return out;
            }();

            constexpr auto map = [=](inner_index_t inner) -> projected_index_t
            {
                // constexpr auto&& map = index_to_index_map_v<_index, _inner>;
                projected_index_t out;
                for (int i = 0; i < out.size(); ++i) {
                    // out[i] = inner[map[i]];
                    out[i] = inner[mapped[i]];
                }
                return out;
            };

            constexpr auto projected = [] {
                std::array<int, _index.n_projected()> map;
                for (int i = 0, j = 0; auto&& [_, t] : _index) {
                    if (t == IGNORE) {
                        map[i++] = j;
                    }
                    ++j;
                }
                return map;
            }();

            // auto project = [&](projected_index_t index) -> projected_index_t {
            //     for (auto i : projected) {
            //         index[i] = self._projected[i];
            //     }
            //     return index;
            // };

            value_type_t<A> sum{};
            // inner_index_t inner(index);
            // do {
            //     sum += ttl::evaluate(self._a, project(map(inner)));
            // } while (carry_sum_add<_order>(inner, inner_extents));
            return sum;
        }

        /// Map an extent from the _inner index to the _index.
        ///
        /// This helper function supports the carry_sum_add function when the
        /// bind represents a contraction. For instance, if we have a 3rd order
        /// tensor A, and the bind formed by A(i,~i,j), then that contracts
        /// those outermost indices.
        ///
        /// When evaluating the resulting bind operation, we need to write a
        /// loop like `for (int i = 0; i < ??; ++i)`. We get the value for ??
        /// from the extents of the child tensor, but it needs to be mapped
        /// properly. See the implementation of carry_sum_add for more details.
        ///
        /// @param i The index of an extent in the _inner index space.
        /// @returns The corresponding index in the _outer index space.
        static constexpr auto _map_inner_extent(int i) -> int
            requires (concepts::has_static_extents<A>)
        {
            constexpr auto&& map = index_to_index_map_v<_inner, _index>;
            constexpr concepts::shape auto&& extents = ttl::extents_v<A>;
            return extents[map[i]];
        }

        constexpr auto _map_inner_extent(int i) const -> int
        {
            constexpr auto&& map = index_to_index_map_v<_inner, _index>;
            concepts::shape auto&& extents = ttl::extents(_a);
            return extents[map[i]];
        }

        constexpr auto _map_index(inner_index_t const& inner) const -> projected_index_t
        {
            projected_index_t projected = _projected;

        }

        /// Validate that contracted indices have the right extents.
        ///
        /// @returns True when the extents of contracted indices match (it will
        ///          throw if it fails during constexpr evaluation).
        static constexpr bool _validate_contracted_shape(concepts::shape auto&& extents)
        {
            for (auto&& [c, _] : _contracted) {
                int i = *_index.index_of(c, 1);
                int j = *_index.index_of(c, 2);
                if (extents[i] != extents[j]) {
                    if consteval {throw "contracted extents do not match in bind";}
                    return false;
                }
            }
            return true;
        }

        constexpr friend auto tag_invoke(cpos::outer, bind const&) {
            return outer();
        }

        constexpr friend auto tag_invoke(cpos::extents, bind const& b)
            -> decltype(auto) requires (not concepts::has_static_extents<A>)
        {
            return _extents(ttl::extents(b._a));
        }

        constexpr friend auto tag_invoke(cpos::evaluate, bind const& b, outer_index_t const& index)
            -> decltype(auto)
        {
            return _evaluate(b, index);
        }

        constexpr friend auto tag_invoke(cpos::evaluate, bind && b, outer_index_t const& index)
            -> decltype(auto)
        {
            return _evaluate(std::move(b), index);
        }

        constexpr friend auto tag_invoke(cpos::evaluate, bind & b, outer_index_t const& index)
            -> decltype(auto)
        {
            return _evaluate(b, index);
        }
    };

    template <concepts::tensor A, concepts::index_or_integral... Is>
    bind(A&&, Is...) -> bind<A, tensor_index{Is{}...}>;
}


namespace ttl::traits
{
    template <concepts::tensor A, concepts::tensor_index auto _index>
    struct order<expressions::bind<A, _index>>
    {
        static constexpr int value = _index.n_unique();
    };

    template <concepts::tensor A, concepts::tensor_index auto _index>
    struct value_type<expressions::bind<A, _index>>
    {
        using type = value_type_t<A>;
    };

    template <concepts::tensor A, concepts::tensor_index auto _index>
    requires concepts::has_static_extents<A>
    struct extents<expressions::bind<A, _index>>
    {
        static constexpr concepts::shape auto value = expressions::bind<A, _index>::_extents(extents_v<A>);
    };
}
