#pragma once

#include <concepts>
#include <type_traits>
#include <ostream>

template <typename T>
concept ContainerBase = requires(T& t)
{
    typename T::value_type;
    typename T::size_type;
    typename T::iterator;
    typename T::const_iterator;

    { t.size() } noexcept -> std::same_as<typename T::size_type>;
    { t.begin() } noexcept -> std::same_as<typename T::iterator>;
    { t.end() } noexcept -> std::same_as<typename T::iterator>;
};

template <typename T>
concept CopyConstructible = requires(const T& t)
{
    T{t};
};

template <typename T>
concept Container = ContainerBase<T> && std::move_constructible<T> &&
                    std::is_copy_assignable_v<T> && std::is_move_assignable_v<T> &&
                    CopyConstructible<T> &&
                    requires(const T& t)
{
    typename T::reference;
    typename T::const_reference;
    typename T::difference_type;

    { t.cbegin() } noexcept -> std::same_as<typename T::const_iterator>;
    { t.cend() } noexcept -> std::same_as<typename T::const_iterator>;
};

template <typename T>
concept MatrixElement = std::regular<T>;

template <typename T, typename U>
concept AddAssignable = requires(T& t, const U& u)
{
    { t += u } -> std::convertible_to<T>;
};

template <typename T>
concept SelfAddAssignable = AddAssignable<T, T>;

template <typename T, typename U>
concept MulAssignable = requires(T& t, const U& u)
{
    { t *= u } -> std::convertible_to<T>;
};

template <typename T, typename U>
concept DivAssignable = requires(T& t, const U& u)
{
    { t /= u } -> std::convertible_to<T>;
};

template <typename T, typename U>
concept SubAssignable = requires(T& t, const U& u)
{
    { t -= u } -> std::convertible_to<T>;
};

template <typename T, typename U>
concept Addable = requires(const T& t, const U& u)
{
    { t + u } -> std::convertible_to<std::common_type_t<T, U>>;
};

template <typename T, typename U>
concept Subtractable = requires(const T& t, const U& u)
{
    { t - u } -> std::convertible_to<std::common_type_t<T, U>>;
};

template <typename T, typename U>
concept Multipliable = requires(const T& t, const U& u)
{
    { t * u } -> std::convertible_to<std::common_type_t<T, U>>;
};

template <typename T, typename U>
concept Divisible = requires(const T& t, const U& u)
{
    { t / u } -> std::convertible_to<std::common_type_t<T, U>>;
};

template <typename T>
concept Signed = requires(const T& t)
{
    { -t } -> std::convertible_to<T>;
};

template <typename T>
concept SignedNumber = std::is_arithmetic_v<T> && std::is_signed_v<T>;

template <typename T>
concept Printable = requires(std::ostream& os, const T& t)
{
    { os << t } -> std::same_as<std::ostream>;
};

template <typename T>
concept ThreeWayComparable = requires(const T& t, const T& u)
{
    t <=> u;
};

template <typename Container, typename T>
concept ContainerOf = ContainerBase<Container> &&
                      requires(const Container& cont)
{
    { *cont.begin() } -> std::convertible_to<T>;
};

