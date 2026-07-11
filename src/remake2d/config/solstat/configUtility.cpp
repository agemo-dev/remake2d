#include <remake2d/config/config.hpp>
#include <remake2d/utility.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaUtility(sol::table& rmk) noexcept {

    rmk["degToRad"] = &angle::degToRad;
    rmk["radToDeg"] = &angle::radToDeg;

    rmk["RGBToHSL"] = &color::RGBToHSL;
    rmk["HSLToRGB"] = &color::HSLToRGB;

    rmk["pixelToMeter"] = sol::overload(
        [](f32 pixels) { return physic::pixelToMeter(pixels); },
        [](const Vec2d& pixels) { return physic::pixelToMeter(pixels); },
        [](const Dim2d& pixels) { return physic::pixelToMeter(pixels); }
    );

    rmk["meterToPixel"] = sol::overload(
        [](f32 meters) { return physic::meterToPixel(meters); },
        [](const Vec2d& meters) { return physic::meterToPixel(meters); },
        [](const Dim2d& meters) { return physic::meterToPixel(meters); }
    );

    rmk["fail"]     = fail;
    rmk["success"]  = success;
    rmk["on"]       = on;
    rmk["off"]      = off;
    rmk["isTrue"]   = isTrue;
    rmk["isFalse"]  = isFalse;

    rmk["file"] = rmk.create();
    rmk["file"]["jump"] = [](sol::variadic_args va) -> std::string {
        std::vector<std::string> strs;
        std::vector<std::string_view> views;
        for (auto v : va) strs.push_back(v.get<std::string>());
        for (auto& s : strs) views.push_back(s);
        return file::jump(views);
    };
}

} // namespace solstat
} // namespace config
} // namespace rmk