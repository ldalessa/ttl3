#pragma once

#include "ttl/scalar_index.hpp"
#include "ttl/utils.hpp"
#include <concepts>

namespace ttl
{
    template <class>
    struct tensor_traits;

    template <class T>
    using tensor_traits_t = tensor_traits<std::remove_cvref_t<T>>;

    template <class T>
    concept is_extents = is_array_of<T, int> and has_static_size<T>;

    template <class T, int N>
    concept is_extents_of_order = is_extents<T> and size<T> == N;

    namespace _detail
    {
        template <class T>
        concept has_tensor_traits_specialization = requires {
            typename tensor_traits_t<T>;
        };

        template <class T>
        concept has_tensor_traits_value_type = has_tensor_traits_specialization<T> and requires {
            typename tensor_traits_t<T>::value_type;
        };

        template <class T>
        concept has_tensor_traits_get_extents = has_tensor_traits_specialization<T>
            and requires (T t) {
                { tensor_traits_t<T>::get_extents(t) } -> is_extents;
            };

        template <class T>
        concept has_tensor_traits_evaluate = has_tensor_traits_specialization<T>
            and has_tensor_traits_value_type<T>
            and has_tensor_traits_get_extents<T>
            and requires (T t, scalar_index<size<decltype(tensor_traits_t<T>::get_extents(t))>> const& i) {
                { tensor_traits_t<T>::evaluate(t, i) } -> std::convertible_to<typename tensor_traits_t<T>::value_type>;
            };
    };

    template <class T>
    concept is_tensor = _detail::has_tensor_traits_value_type<T>
        and _detail::has_tensor_traits_get_extents<T>
        and _detail::has_tensor_traits_evaluate<T>;

    template <_detail::has_tensor_traits_value_type T>
    using value_type_t = typename tensor_traits_t<T>::value_type;

    template <_detail::has_tensor_traits_get_extents T>
    constexpr int order = size<decltype(tensor_traits_t<T>::get_extents(std::declval<T>()))>;

    namespace _detail
    {
        struct _get_extents_fn
        {
            template <_detail::has_tensor_traits_get_extents T>
            constexpr auto operator()(T&& obj) const -> decltype(auto) {
                return tensor_traits_t<T>::get_extents(FWD(obj));
            }
        };
    }
    constexpr _detail::_get_extents_fn extents{};

    template <is_extents A, is_extents B>
    constexpr auto join_extents(A&& a, B&& b) -> array<int, size<A> + size<B>>
    {
        array<int, size<A> + size<B>> out;
        int i = 0;
        for (auto&& a : a) out[i++] = a;
        for (auto&& b : b) out[i++] = b;
        return out;
    }

    namespace _detail
    {
        struct _evaluate_fn
        {
            template <_detail::has_tensor_traits_evaluate T>
            constexpr auto operator()(T&& obj, is_scalar_index auto&& i) const -> decltype(auto) {
                return tensor_traits_t<T>::evaluate(FWD(obj), FWD(i));
            }
        };
    }
    constexpr _detail::_evaluate_fn evaluate{};

    template <class T, auto N>
    concept is_tensor_of_order = is_tensor<T> and (order<T> == N);

    template <class T>
    concept is_scalar = is_tensor_of_order<T, 0>;

    namespace _detail
    {
        template <class T>
        concept has_member_value_type = requires {
            typename std::remove_cvref_t<T>::value_type;
        };

        template <class T>
        concept has_member_get_extents = requires (T t) {
            { t.get_extents() } -> is_extents;
        };

        template <class T>
        concept has_member_evaluate = has_member_value_type<T>
            and has_member_get_extents<T>
            and requires (T t, scalar_index<size<decltype(t.get_extents())>> const& i) {
                { t.evaluate(i) } -> std::convertible_to<typename std::remove_cvref_t<T>::value_type>;
            };

        template <class T>
        concept has_member_tensor_traits = has_member_value_type<T>
            and has_member_get_extents<T>
            and has_member_evaluate<T>;
    }

    template <_detail::has_member_tensor_traits T>
    struct tensor_traits<T>
    {
        using value_type = typename std::remove_cvref_t<T>::value_type;

        static constexpr auto get_extents(auto&& obj) -> decltype(auto) {
            return FWD(obj).get_extents();
        }

        static constexpr auto evaluate(auto&& obj, is_scalar_index auto&& i) -> decltype(auto) {
            return FWD(obj).evaluate(FWD(i));
        }
    };
}
