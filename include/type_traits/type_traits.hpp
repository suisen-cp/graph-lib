#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

namespace type_traits {
    template <typename T>
    struct has_member_size {
    private:
        template <typename T_>
        static auto test(T_) -> decltype(
            static_cast<int>(std::declval<T_>().size()),
            std::true_type{}
        );
        static std::false_type test(...);
    public:
        static constexpr bool value = decltype(test(std::declval<T>()))::value;
    };
    template <typename T>
    constexpr bool has_member_size_v = has_member_size<T>::value;

    template <typename T, std::size_t K>
    struct has_kth_element {
    private:
        template <typename T_>
        static auto test(T_) -> decltype(
            std::get<K>(std::declval<T_>()),
            std::true_type{}
        );
        static std::false_type test(...);
    public:
        static constexpr bool value = decltype(test(std::declval<T>()))::value;
    };
    template <typename T, std::size_t K>
    constexpr bool has_kth_element_v = has_kth_element<T, K>::value;

    template <typename T, typename Index>
    struct has_subscript_operator {
    private:
        template <typename T_>
        static auto test(T_) -> decltype(
            std::declval<T_>().operator[](std::declval<Index>()),
            std::true_type{}
        );
        static std::false_type test(...);
    public:
        static constexpr bool value = decltype(test(std::declval<T>()))::value;
    };
    template <typename T, typename Index>
    constexpr bool has_subscript_operator_v = has_subscript_operator<T, Index>::value;
}
