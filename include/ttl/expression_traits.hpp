#pragma once

#include "ttl/tensor_index.hpp"
#include "ttl/tensor_traits.hpp"

namespace ttl
{
    template <class> class expression_traits;

    template <class T>
    using expression_traits_t = expression_traits<std::remove_cvref_t<T>>;

    template <class T>
    concept has_expression_traits = has_tensor_traits<T> and requires {
        { expression_traits_t<T>::get_outer() } -> is_tensor_index_of_size<order<T>>;
    };

    template <has_expression_traits T>
    constexpr tensor_index outer = expression_traits_t<T>::get_outer();

    template <class T>
    concept has_member_expression_traits = has_tensor_traits<T> and requires {
        { T::get_outer() } -> is_tensor_index_of_size<order<T>>;
    };

    template <is_scalar T>
    struct expression_traits<T>
    {
        static consteval auto get_outer() -> tensor_index<0> {
            return tensor_index<0>{};
        }
    };

    template <class T>
    requires has_member_expression_traits<T> and (not is_scalar<T>)
    struct expression_traits<T>
    {
        static constexpr int test = true;

        static consteval auto get_outer() -> decltype(auto) {
            return T::get_outer();
        }
    };

    template <class T>
    concept is_expression = has_expression_traits<T>;
}
