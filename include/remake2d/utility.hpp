#ifndef REMAKE2D_UTILITY_
#define REMAKE2D_UTILITY_

#include <remake2d/time.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/private/nil.hpp>
#include <remake2d/private/layer.hpp>
#include <remake2d/config/forward.hpp>
#include <remake2d/config/version.hpp>

#include <span>

namespace rmk {

std::string currentVersion(void) noexcept;

namespace color  {
HSL RGBToHSL(const Color&);
Color HSLToRGB(const HSL&);
} // namespace color

namespace angle {
fmax degToRad(fmax) noexcept;
fmax radToDeg(fmax) noexcept;

namespace literal {
fmax operator""_deg(fmax) noexcept;
fmax operator""_rad(fmax) noexcept;
} // namespace literal
} // namespace angle

namespace physic {
f32 pixelToMeter(f32);
f32 meterToPixel(f32);
Vec2d pixelToMeter(const Vec2d&);
Vec2d meterToPixel(const Vec2d&);
Dim2d pixelToMeter(const Dim2d&);
Dim2d meterToPixel(const Dim2d&);
} // namespace physic

namespace file {
    std::string jump(std::span<std::string_view>) noexcept;
} // namespace file

namespace time {
namespace literal {
time::Nanosecond  operator""_ns(unsigned long long)  noexcept;
time::Microsecond operator""_us(unsigned long long)  noexcept;
time::Millisecond operator""_ms(unsigned long long)  noexcept;
time::Second      operator""_s(unsigned long long)   noexcept;
time::Minute      operator""_min(unsigned long long) noexcept;
time::Hour        operator""_h(unsigned long long)   noexcept;
time::Day         operator""_d(unsigned long long)   noexcept;
time::Week        operator""_w(unsigned long long)   noexcept;
time::Month       operator""_mo(unsigned long long)  noexcept;
time::Year        operator""_y(unsigned long long)   noexcept;
} // namespace literal
} // namespace time


inline constexpr u8 fail      = 1;
inline constexpr u8 success   = 0;
inline constexpr bool on      = true;
inline constexpr bool off     = false;
inline constexpr bool isTrue  = true;
inline constexpr bool isFalse = false;

} // namespace rmk

#include <remake2d/template/utility.tpp>

#endif