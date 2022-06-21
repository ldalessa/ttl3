#pragma once

#include "ttl/index.hpp"
#include "ttl/concepts/bind_expression.hpp"
#include "ttl/traits/order.hpp"
#include <concepts>
#include <utility>

namespace ttl::concepts
{
    template <has_order T>
    struct _bindable_helper
    {
        template <std::size_t... is>
        static void test(T t, std::index_sequence<is...>)
            requires requires { { t(ttl::index<wchar_t{is}>{}...) } -> bind_expression; };

        static void test(T t)
            requires requires { test(t, std::make_index_sequence<order_v<T>>{}); };
    };

    template <has_order_zero T>
    struct _bindable_helper<T>
    {
        static void test(T t);
    };

    template <class T>
    concept bindable = has_order<T> and requires (T t) {
        { _bindable_helper<std::remove_cvref_t<T>>::test(t) };
    };
}
