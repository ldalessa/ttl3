#pragma once

#include "ttl/tensor_index.hpp"
#include "ttl/tensor_traits.hpp"

namespace ttl
{
    template <class> class expression_traits;

    template <class T>
    using expression_traits_t = expression_traits<std::remove_cvref_t<T>>;

    namespace _detail
    {
        template <class T>
        concept _has_expression_traits_specialization = requires {
            typename expression_traits_t<T>;
        };

        template <class T>
        concept _has_expression_traits_get_outer = is_tensor<T>
            and _has_expression_traits_specialization<T>
            and requires {
                { expression_traits_t<T>::get_outer() } -> is_tensor_index_of_size<order<T>>;
            };
    }

    template <class T>
    concept is_expression = is_tensor<T>
        and _detail::_has_expression_traits_get_outer<T>;

    template <class T, int order>
    concept is_expression_of_order = is_expression<T>
        and is_tensor_of_order<T, order>;

    template <_detail::_has_expression_traits_get_outer T>
    constexpr tensor_index outer = expression_traits_t<T>::get_outer();

    namespace _detail
    {
        template <class T>
        concept _has_member_get_outer = is_tensor<T> and requires {
            { std::remove_cvref_t<T>::get_outer() } -> is_tensor_index_of_size<order<T>>;
        };

        template <class T>
        concept _has_member_expression_traits = _has_member_get_outer<T>;
    }

    /// Specialize expression traits for scalar tensors
    template <is_scalar T>
    struct expression_traits<T>
    {
        static constexpr auto get_outer() -> tensor_index<0> {
            return tensor_index<0>{};
        }
    };

    /// Specialize expression traits for tensors that have appropriate members.
    template <class T>
    requires _detail::_has_member_expression_traits<T> and (not is_scalar<T>)
    struct expression_traits<T>
    {
        static constexpr auto get_outer() -> decltype(auto) {
            return std::remove_cvref_t<T>::get_outer();
        }
    };
}
