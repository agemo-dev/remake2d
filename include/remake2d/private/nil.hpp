#ifndef REMAKE2D_NIL_
#define REMAKE2D_NIL_

namespace rmk {

class Nil {
public:
    template<typename T> constexpr operator T(void) const noexcept;

public:
    template<typename T> constexpr bool operator==(const T&) const noexcept;
    template<typename T> constexpr bool operator!=(const T&) const noexcept;

private:
    template<typename T> friend constexpr bool operator==(const T&, const Nil&) noexcept;
    template<typename T> friend constexpr bool operator!=(const T&, const Nil&) noexcept;
};

inline constexpr Nil nil;

} // namespace rmk

#include <remake2d/template/private/nil.tpp>
#endif