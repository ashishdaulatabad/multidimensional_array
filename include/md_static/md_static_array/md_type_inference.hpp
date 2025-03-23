#pragma once
#ifndef _MD_TYPE_INTER_HPP_
#define _MD_TYPE_INTER_HPP_
#include <functional>

#include "../../md_complex/md_complex.hpp"
#include "../../utility/math.hpp"

template <typename T> using fn = std::function<T>;

/**
 * @brief Type inference namespace to infer implicit return types
 */
namespace MdTypeInfer {
/**
 * @brief Template to check if type is from a variadic list
 * @tparam T1 value to check
 * @tparam ... variadic list
 */
template <typename T1, typename...> struct is_any_one : std::false_type {};

/**
 * @brief Template to check if type is from a variadic list
 * @tparam T1 value to check
 * @tparam T2 value with which
 * @tparam ... variadic list
 */
template <typename T1, typename T2, typename... args>
struct is_any_one<T1, T2, args...> {
    static constexpr bool value =
        std::is_same<T1, T2>::value || is_any_one<T1, args...>::value;
};

/**
 * @brief Template to check if type is one of the complex numbers
 * @tparam T value to check
 */
template <typename T> struct is_complex {
    static constexpr bool value =
        is_any_one<T, c64, c128, c32, cu8, cu16, cu32, cu64, ci8, ci16, ci32,
                   ci64>::value;
};

/**
 * @brief Template to check if type is one of the native types
 * @tparam T value to check
 */
template <typename T> struct is_native {
    static constexpr bool value = is_any_one<T, i32, i64, i16, i8, u8, u16, u32,
                                             u64, f32, f64, f128, usize>::value;
};

/**
 * @brief Template to check if type is one of the native types
 * @tparam T value to check
 */
template <typename T> struct is_int {
    static constexpr bool value =
        is_any_one<T, i32, i64, i16, i8, u8, u16, u32, u64, usize>::value;
};

/**
 * @brief Template to check if type is one of the native types
 * @tparam T value to check
 */
template <typename T> struct is_arith {
    static constexpr bool value = is_native<T>::value || is_complex<T>::value;
};

/**
 * @brief Template to check if type is one of the floating complex numbers
 * @tparam T value to check
 */
template <typename T> struct is_floating_complex {
    static constexpr bool value = is_any_one<T, c64, c128, c32>::value;
};

/**
 * @brief Template to check if type is one of the signed complex integers
 * @tparam T value to check
 */
template <typename T> struct is_signed_complex {
    static constexpr bool value = is_any_one<T, ci8, ci16, ci32, ci64>::value;
};

/**
 * @brief Template to check if type is one of the unsigned complex integers
 * @tparam T value to check
 */
template <typename T> struct is_unsigned_complex {
    static constexpr bool value = is_any_one<T, cu8, cu16, cu32, cu64>::value;
};

/**
 * @brief Variadic template to check if type is one of the signed complex
 * integers
 */
template <typename...> struct is_any_one_complex : std::false_type {};

/**
 * @brief Variadic template to check if any one type is one of complex
 * numbers
 * @tparam T1 value to check
 * @tparam args variadic arguments
 */
template <typename T1, typename... args>
struct is_any_one_complex<T1, args...> {
    static constexpr bool value =
        is_complex<T1>::value || is_any_one_complex<args...>::value;
};

/**
 * @brief Variadic template to check if any one type is one of complex
 * real numbers
 */
template <typename...> struct is_any_one_floating_complex : std::false_type {};

/**
 * @brief Variadic template to check if any one type is one of complex
 * real numbers
 * @tparam T1 value to check
 * @tparam args variadic arguments
 */
template <typename T1, typename... args>
struct is_any_one_floating_complex<T1, args...> {
    static constexpr bool value = is_floating_complex<T1>::value ||
                                  is_any_one_floating_complex<args...>::value;
};

/**
 * @brief Variadic template to check if any one type is one of complex
 * signed integers
 */
template <typename...> struct is_any_one_signed_complex : std::false_type {};

/**
 * @brief Variadic template to check if any one type is one of complex
 * signed integers
 * @tparam T1 value to check
 * @tparam args variadic arguments
 */
template <typename T1, typename... args>
struct is_any_one_signed_complex<T1, args...> {
    static constexpr bool value = is_signed_complex<T1>::value ||
                                  is_any_one_signed_complex<args...>::value;
};

/**
 * @brief Variadic template to check if any one type is one of complex
 * unsigned integers
 * @tparam T1 value to check
 * @tparam args variadic arguments
 */
template <typename...> struct is_any_one_unsigned_complex : std::false_type {};

/**
 * @brief Variadic template to check if any one type is one of complex
 * unsigned integers
 * @tparam T1 value to check
 * @tparam args variadic arguments
 */
template <typename T1, typename... args>
struct is_any_one_unsigned_complex<T1, args...> {
    static constexpr bool value = is_unsigned_complex<T1>::value ||
                                  is_any_one_unsigned_complex<args...>::value;
};

/**
 * @brief Variadic template to check if any one type is one of
 * real numbers
 */
template <typename...> struct is_any_one_floating : std::false_type {};

/**
 * @brief Variadic template to check if any one type is one of
 * real numbers
 * @tparam T1 value to check
 * @tparam args variadic arguments
 */
template <typename T1, class... arg> struct is_any_one_floating<T1, arg...> {
    static constexpr bool value =
        std::is_floating_point<T1>::value || is_any_one_floating<arg...>::value;
};

/**
 * @brief Variadic template to check if any one type is one of
 * signed integers
 */
template <class...> struct is_any_one_signed {
    static constexpr bool value = std::false_type{};
};

/**
 * @brief Variadic template to check if any one type is one of
 * signed integers
 * @tparam T1 value to check
 * @tparam args variadic arguments
 */
template <class T1, class... arg> struct is_any_one_signed<T1, arg...> {
    static constexpr bool value =
        std::is_signed<T1>::value || is_any_one_signed<arg...>::value;
};

/**
 * @brief Returns type that has max byte size per element
 * @tparam T1 first type
 * @tparam T2 second type
 */
template <typename T1, typename T2, class = void> struct max_usize {};

/**
 * @brief Returns type that has max byte size per element
 * @tparam T1 first type
 * @tparam T2 second type
 */
template <typename T1, typename T2>
struct max_usize<T1, T2,
                 typename std::enable_if<(sizeof(T1) > sizeof(T2))>::type> {
    static constexpr auto value = static_cast<T1>(0);
};

/**
 * @brief Returns type that has max byte size per element
 * @tparam T1 first type
 * @tparam T2 second type
 */
template <typename T1, typename T2>
struct max_usize<T1, T2,
                 typename std::enable_if<(sizeof(T2) >= sizeof(T1))>::type> {
    static constexpr auto value = static_cast<T2>(0);
};

/**
 * @brief Wrapper for returning appropriate floating number
 * @tparam t generic type
 */
template <typename t, class = void> struct floating_t;

/**
 * @brief Wrapper for returning appropriate floating number
 * @tparam Ttypeval generic type
 */
template <typename Ttypeval>
struct floating_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) ==
                                                     sizeof(f32))>::type> {
    using type = f32;
};

template <typename Ttypeval>
struct floating_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) ==
                                                     sizeof(f64))>::type> {
    using type = f64;
};

template <typename Ttypeval>
struct floating_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) >=
                                                     sizeof(f128))>::type> {
    using type = f128;
};

/**
 * @brief Wrapper for returning appropriate complex floating number
 * @tparam Ttypeval generic type
 */
template <typename t, class = void> struct complex_floating_t;

template <typename Ttypeval>
struct complex_floating_t<
    Ttypeval,
    typename std::enable_if<(sizeof(Ttypeval) == sizeof(c32))>::type> {
    using type = c32;
};

template <typename Ttypeval>
struct complex_floating_t<
    Ttypeval,
    typename std::enable_if<(sizeof(Ttypeval) == sizeof(c64))>::type> {
    using type = c64;
};

template <typename Ttypeval>
struct complex_floating_t<
    Ttypeval,
    typename std::enable_if<(sizeof(Ttypeval) >= sizeof(c128))>::type> {
    using type = c128;
};

/**
 * @brief Wrapper for returning appropriate complex signed integer
 * @tparam Ttypeval generic type
 */
template <typename Ttypeval, typename...> struct complex_signed_t {
    using type = Ttypeval;
};

template <typename Ttypeval>
struct complex_signed_t<Ttypeval, typename std::enable_if<(
                                      sizeof(Ttypeval) == sizeof(ci8))>::type> {
    using type = ci8;
};

template <typename Ttypeval>
struct complex_signed_t<
    Ttypeval,
    typename std::enable_if<(sizeof(Ttypeval) == sizeof(ci16))>::type> {
    using type = ci16;
};

template <typename Ttypeval>
struct complex_signed_t<
    Ttypeval,
    typename std::enable_if<(sizeof(Ttypeval) == sizeof(ci32))>::type> {
    using type = ci32;
};

template <typename Ttypeval>
struct complex_signed_t<
    Ttypeval,
    typename std::enable_if<(sizeof(Ttypeval) >= sizeof(ci64))>::type> {
    using type = ci64;
};

/**
 * @brief Wrapper for returning appropriate signed integer
 * @tparam Ttypeval generic type
 */
template <typename Ttypeval, class = void> struct signed_t;

template <typename Ttypeval>
struct signed_t<
    Ttypeval, typename std::enable_if<(sizeof(Ttypeval) == sizeof(i8))>::type> {
    using type = i8;
};

template <typename Ttypeval>
struct signed_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) ==
                                                   sizeof(i16))>::type> {
    using type = i16;
};

template <typename Ttypeval>
struct signed_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) ==
                                                   sizeof(i32))>::type> {
    using type = i32;
};

template <typename Ttypeval>
struct signed_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) >=
                                                   sizeof(i64))>::type> {
    using type = i64;
};

/**
 * @brief Wrapper for returning appropriate unsigned integer
 * @tparam Ttypeval generic type
 */
template <typename Ttypeval, class = void> struct unsigned_t;

template <typename Ttypeval>
struct unsigned_t<
    Ttypeval, typename std::enable_if<(sizeof(Ttypeval) == sizeof(u8))>::type> {
    using type = u8;
};

template <typename Ttypeval>
struct unsigned_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) ==
                                                     sizeof(u16))>::type> {
    using type = u16;
};

template <typename Ttypeval>
struct unsigned_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) ==
                                                     sizeof(u32))>::type> {
    using type = u32;
};

template <typename Ttypeval>
struct unsigned_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) >=
                                                     sizeof(u64))>::type> {
    using type = u64;
};

/**
 * @brief Wrapper for returning appropriate complex unsigned integer
 * @tparam Ttypeval generic type
 */
template <typename Ttypeval, class...> struct cunsigned_t {
    using type = Ttypeval;
};

template <typename Ttypeval>
struct cunsigned_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) ==
                                                      sizeof(cu8))>::type> {
    using type = cu8;
};

template <typename Ttypeval>
struct cunsigned_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) ==
                                                      sizeof(cu16))>::type> {
    using type = cu16;
};

template <typename Ttypeval>
struct cunsigned_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) ==
                                                      sizeof(cu32))>::type> {
    using type = cu32;
};

template <typename Ttypeval>
struct cunsigned_t<Ttypeval, typename std::enable_if<(sizeof(Ttypeval) >=
                                                      sizeof(cu64))>::type> {
    using type = cu64;
};

template <typename> struct is_vector : std::false_type {};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

template <typename T> struct is_mallocable {
    static constexpr bool value =
        std::is_fundamental<T>::value || is_any_one_complex<T>::value;
};

/**
 * @brief Wrapper for evaluating appropriate complex type
 * @tparam Ttypeval generic type
 */
template <typename T1, typename T2, class = void> struct eval_complex_t {
#define MX_SZ decltype(max_usize<T1, T2>::value)
    static constexpr auto value = []() {
        if constexpr (is_any_one_floating_complex<T1, T2>::value) {
            return static_cast<typename complex_floating_t<MX_SZ>::type>(0);
        } else if constexpr (is_any_one_unsigned_complex<T1, T2>::value) {
            if constexpr (is_any_one_floating<T1, T2>::value) {
                return static_cast<typename complex_floating_t<MX_SZ>::type>(0);
            } else {
                return static_cast<typename cunsigned_t<MX_SZ>::type>(0);
            }
        } else {
            return static_cast<typename complex_signed_t<MX_SZ>::type>(0);
        }
    }();
#undef MX_SZ
};

/**
 * @brief Evaluate resultant type of element, given two types
 * @tparam T1 first type
 * @tparam T2 second type
 * @todo improve complex logic
 */
template <typename T1, typename T2, class = void> struct eval_resultant_t {
#define MX_SZ decltype(max_usize<T1, T2>::value)
    static constexpr auto value = []() {
        if constexpr (is_any_one_complex<T1, T2>::value) {
            return eval_complex_t<T1, T2>::value;
        } else if constexpr (is_any_one_floating<T1, T2>::value) {
            return static_cast<typename floating_t<MX_SZ>::type>(0);
        } else if constexpr (is_any_one_signed<T1, T2>::value) {
            return static_cast<typename signed_t<MX_SZ>::type>(0);
        } else {
            return static_cast<typename unsigned_t<MX_SZ>::type>(0);
        }
    }();

#undef MX_SZ
};

} // namespace MdTypeInfer

#endif
