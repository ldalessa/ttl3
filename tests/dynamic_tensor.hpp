#pragma once

#include "ttl/ttl.hpp"
#include "md_vector.hpp"
#include <concepts>

namespace ttl::tests
{
    template <class T, int _order>
    struct dynamic_tensor : md_vector<T, _order>, ttl::bindable<dynamic_tensor<T, _order>>
    {
        using ttl::bindable<dynamic_tensor<T, _order>>::operator();

        constexpr explicit dynamic_tensor(std::integral auto... extents)
                : md_vector<T, _order> { extents... }
        {
        }

        constexpr friend auto tag_invoke(ttl::cpos::extents, dynamic_tensor const& t) {
            return t._shape;
        }

        constexpr friend auto tag_invoke(ttl::cpos::evaluate, dynamic_tensor const& t, ttl::scalar_index<_order> const& index)
            -> decltype(auto)
        {
            return _evaluate(t, index);
        }

        constexpr friend auto tag_invoke(ttl::cpos::evaluate, dynamic_tensor&& t, ttl::scalar_index<_order> const& index)
            -> decltype(auto)
        {
            return _evaluate(std::move(t), index);
        }

        constexpr friend auto tag_invoke(ttl::cpos::evaluate, dynamic_tensor& t, ttl::scalar_index<_order> const& index)
            -> decltype(auto)
        {
            return _evaluate(t, index);
        }

        /// Handle the ttl::cpos::evaluate CPO for const&,&&,& dynamic vectors.
        ///
        /// This could also be done in the raw tag_invoke calls above if we had
        /// concept for dynamic tensors. The md_vector provides a variadic
        /// operator(), so we just expand the index into a variadic form here.
        ///
        /// @param self  A forwarding reference formed from the this pointer.
        /// @param index The index to evalute.
        constexpr static auto _evaluate(auto&& self, ttl::scalar_index<_order> const& index)
            -> decltype(auto)
        {
            return [&]<std::size_t... is>(std::index_sequence<is...>) -> decltype(auto) {
                return std::forward<md_vector<T, _order>>(self)(index[is]...);
            }(std::make_index_sequence<_order>{});
        }
    };
}

namespace ttl::traits
{
    template <class T, int _order>
    struct tensor<ttl::tests::dynamic_tensor<T, _order>>
    {
        using value_type = T;
        static constexpr int order = _order;
    };
}
