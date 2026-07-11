#ifndef REMAKE2D_NUMERIC_
#define REMAKE2D_NUMERIC_

#include <cstdint>
#include <cstddef>

namespace rmk {

using byte = std::uint8_t;
using rune = std::uint16_t;

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using ptrdiff = std::ptrdiff_t;
using addr    = std::uintptr_t;
using usize   = std::size_t;

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using imax = std::intmax_t;
using umax = std::uintmax_t;

using ileast8  = std::int_least8_t;
using ileast16 = std::int_least16_t;
using ileast32 = std::int_least32_t;
using ileast64 = std::int_least64_t;

using ifast8  = std::int_fast8_t;
using ifast16 = std::int_fast16_t;
using ifast32 = std::int_fast32_t;
using ifast64 = std::int_fast64_t;

using uleast8  = std::uint_least8_t;
using uleast16 = std::uint_least16_t;
using uleast32 = std::uint_least32_t;
using uleast64 = std::uint_least64_t;

using ufast8  = std::uint_fast8_t;
using ufast16 = std::uint_fast16_t;
using ufast32 = std::uint_fast32_t;
using ufast64 = std::uint_fast64_t;

using f32  = float;
using f64  = double;
using fmax = long double;

}//namespace rmk
#endif