#include <remake2d/utility.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/clock.hpp>
#include <remake2d/math.hpp>

#include <cmath>
#include <ostream>
#include <algorithm>
#include <filesystem>

namespace rmk {

namespace version {
std::string current(void) noexcept {
	std::ostringstream oss;
	oss << REMAKE2D_VERSION_MAJOR << '.' << REMAKE2D_VERSION_MINOR << '.' << REMAKE2D_VERSION_PATCH;
	return oss.str();
}
} // namespace version

namespace color {
HSL RGBToHSL(const Color& c) {
    
    f32 rf = c.r / 255.0f;
    f32 gf = c.g / 255.0f;
    f32 bf = c.b / 255.0f;

    f32 max = std::max({ rf, gf, bf });
    f32 min = std::min({ rf, gf, bf });
    f32 delta = max - min;

    HSL out;
    out.l = (max + min) / 2.0f;

    if (delta == 0.0f) {
        out.h = 0.0f;
        out.s = 0.0f;
        return out;
    }

    out.s = out.l > 0.5f ? delta / (2.0f - max - min) : delta / (max + min);

    if (max == rf)
        out.h = (gf - bf) / delta + (gf < bf ? 6.0f : 0.0f);
    else if (max == gf)
        out.h = (bf - rf) / delta + 2.0f;
    else
        out.h = (rf - gf) / delta + 4.0f;

    out.h *= 60.0f;
    return out;
}

Color HSLToRGB(const HSL& hsl) {
    auto hue2rgb = [](f32 p, f32 q, f32 t) {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f / 2.0f) return q;
        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
        return p;
    };

    if (hsl.s == 0.0f) {
        byte v = (byte)(hsl.l * 255.0f);
        return { v, v, v, 255 };
    }

    f32 q = hsl.l < 0.5f ? hsl.l * (1.0f + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
    f32 p = 2.0f * hsl.l - q;
    f32 h_norm = hsl.h / 360.0f;

    byte r = (byte)(hue2rgb(p, q, h_norm + 1.0f / 3.0f) * 255.0f);
    byte g = (byte)(hue2rgb(p, q, h_norm) * 255.0f);
    byte b = (byte)(hue2rgb(p, q, h_norm - 1.0f / 3.0f) * 255.0f);

    return { r, g, b, 255 };
}
} //namespace color

namespace physic {

f32 pixelToMeter(f32 pixels) {
    return pixels / physics.pixelsPerMeter();
}

f32 meterToPixel(f32 meters) {
    return meters * physics.pixelsPerMeter();
}

Vec2d pixelToMeter(const Vec2d& pixels) {
    f32 ptm = physics.pixelsPerMeter();
    return { pixels.x / ptm, pixels.y / ptm };
}

Vec2d meterToPixel(const Vec2d& meters) {
    f32 ptm = physics.pixelsPerMeter();
    return { meters.x * ptm, meters.y * ptm };
}

Dim2d pixelToMeter(const Dim2d& pixels) {
    f32 ptm = physics.pixelsPerMeter();
    return { pixels.w / ptm, pixels.h / ptm };
}

Dim2d meterToPixel(const Dim2d& meters) {
    f32 ptm = physics.pixelsPerMeter();
    return { meters.w * ptm, meters.h * ptm };
}

} //namespace physic


namespace layer {
i16 ground(u8 wall) noexcept {
    return wall - 256;
}
i16 world(u8 wall) noexcept {
    return wall;
}
i16 sky(u8 wall) noexcept {
    return wall + 256;
}
i16 ui(u8 wall) noexcept {
    return wall + 511;
}
i16 log(u8 wall) noexcept {
    return wall + 767;
}
} //namespace layer

namespace angle {
fmax degToRad(fmax degrees) noexcept {
    return degrees * (pi / 180.0f);
}

fmax radToDeg(fmax radians) noexcept {
    return radians * (180.0f / pi);
}

namespace literal {
fmax operator""_deg(fmax d) noexcept { 
	return degToRad(d);
}
	
fmax operator""_rad(fmax r) noexcept {
	return radToDeg(r);
}

} //namespace literal
} //namespace angle

namespace file {
std::string jump(std::span<std::string_view> files) noexcept {
    for (const auto& file : files) {
        std::string f = std::string(file);
        if (std::filesystem::is_regular_file(f)) {
            return f;
        }
    }
    return nil;
}
} //namespace file

namespace time {
namespace literal {
time::Nanosecond  operator""_ns(unsigned long long val)  noexcept { return time::Nanosecond(val);  }
time::Microsecond operator""_us(unsigned long long val)  noexcept { return time::Microsecond(val); }
time::Millisecond operator""_ms(unsigned long long val)  noexcept { return time::Millisecond(val); }
time::Second      operator""_s(unsigned long long val)   noexcept { return time::Second(val); }
time::Minute      operator""_min(unsigned long long val) noexcept { return time::Minute(val); }
time::Hour        operator""_h(unsigned long long val)   noexcept { return time::Hour(val);   }
time::Day         operator""_d(unsigned long long val)   noexcept { return time::Day(val);    }
time::Week        operator""_w(unsigned long long val)   noexcept { return time::Week(val);   }
time::Month       operator""_mo(unsigned long long val)  noexcept { return time::Month(val);  }
time::Year        operator""_y(unsigned long long val)   noexcept { return time::Year(val);   }
} //namespace literal
} //namespace time

bool operator==(const Date& a, const Date& b) noexcept { return a.dayIs(b); }
bool operator!=(const Date& a, const Date& b) noexcept { return !a.dayIs(b); }

bool operator<(const Date& a, const Date& b) noexcept {
    if (a.year()  != b.year())  return a.year()  < b.year();
    if (a.month() != b.month()) return a.month() < b.month();
    return a.day() < b.day();
}

bool operator>(const Date& a, const Date& b)  noexcept {
    if (a.year()  != b.year())  return a.year()  > b.year();
    if (a.month() != b.month()) return a.month() > b.month();
    return a.day() > b.day();
}

bool operator<=(const Date& a, const Date& b) noexcept { return a < b || a == b; }
bool operator>=(const Date& a, const Date& b) noexcept { return a > b || a == b; }


bool operator==(const Clock& a, const Clock& b) noexcept { return a.timeIs(b); }
bool operator!=(const Clock& a, const Clock& b) noexcept { return !a.timeIs(b); }

bool operator<(const Clock& a, const Clock& b) noexcept {
    if (a.hour()   != b.hour())   return a.hour()   < b.hour();
    if (a.minute() != b.minute()) return a.minute() < b.minute();
    return a.second() < b.second();
}

bool operator>(const Clock& a, const Clock& b)  noexcept {
    if (a.hour()   != b.hour())   return a.hour()   > b.hour();
    if (a.minute() != b.minute()) return a.minute() > b.minute();
    return a.second() > b.second();
}

bool operator<=(const Clock& a, const Clock& b) noexcept { return a < b || a == b; }
bool operator>=(const Clock& a, const Clock& b) noexcept { return a > b || a == b; }

} //namespace rmk