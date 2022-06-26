#pragma once

#include "ttl/ttl.hpp"
#include "md_vector.hpp"
#include <concepts>

namespace ttl::tests
{
    template <class T, int _order>
    struct dynamic_tensor : md_vector<T, _order>, ttl::bindable<dynamic_tensor<T, _order>>
    {
        using scalar_type = T;

        using ttl::bindable<dynamic_tensor<T, _order>>::operator();

        constexpr dynamic_tensor() requires(_order != 0) = default;

        constexpr explicit dynamic_tensor(std::integral auto... extents)
            requires (sizeof...(extents) == _order)
                : md_vector<T, _order> { extents... }
        {
        }

        static consteval auto get_order() -> int {
            return _order;
        }

        constexpr auto get_extents() const -> decltype(auto) {
            return this->_shape;
        }

        /// Handle the ttl::cpos::evaluate CPO for const&,&&,& dynamic vectors.
        ///
        /// This could also be done in the raw tag_invoke calls above if we had
        /// concept for dynamic tensors. The md_vector provides a variadic
        /// operator(), so we just expand the index into a variadic form here.
        ///
        /// @param self  A forwarding reference formed from the this pointer.
        /// @param index The index to evalute.
        constexpr auto evaluate(ttl::scalar_index<_order> const& index) const -> decltype(auto)
        {
            return [&]<std::size_t... is>(md_vector<T, _order> const& self, std::index_sequence<is...>) -> decltype(auto) {
                return self(index[is]...);
            }(*this, std::make_index_sequence<_order>{});
        }

        constexpr auto evaluate(ttl::scalar_index<_order> const& index) -> decltype(auto)
        {
            return [&]<std::size_t... is>(md_vector<T, _order>& self, std::index_sequence<is...>) -> decltype(auto) {
                return self(index[is]...);
            }(*this, std::make_index_sequence<_order>{});
        }
    };
}
