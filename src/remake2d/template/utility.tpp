#ifndef REMAKE2D_UTILITY_TPP_
#define REMAKE2D_UTILITY_TPP_

namespace rmk {

template<typename T>
constexpr Nil::operator T(void) const noexcept {
    return T{};
}

template<typename T>
constexpr bool Nil::operator==(const T& other) const noexcept {
    return other == T{};
}

template<typename T>
constexpr bool Nil::operator!=(const T& other) const noexcept {
    return other != T{};
}

template<typename T>
constexpr bool operator==(const T& lhs, const Nil& rhs) noexcept {
    return rhs == lhs;
}

template<typename T>
constexpr bool operator!=(const T& lhs, const Nil& rhs) noexcept {
    return rhs != lhs;
}


} //namespace rmk

#endif