#include <remake2d/config/config.hpp>
#include <remake2d/utility.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaUtility(sol::table& rmk) noexcept {

	rmk["version"].get_or_create<sol::table>();
	rmk["version"]["current"] = &version::current;
	
    rmk["angle"].get_or_create<sol::table>();
    rmk["angle"]["degToRad"] = &angle::degToRad;
    rmk["angle"]["radToDeg"] = &angle::radToDeg;

	rmk["color"].get_or_create<sol::table>();
    rmk["color"]["RGBToHSL"] = &color::RGBToHSL;
    rmk["color"]["HSLToRGB"] = &color::HSLToRGB;

	rmk["physic"].get_or_create<sol::table>();
    rmk["physic"]["pixelToMeter"] = sol::overload(
        [](f32 pixels) { return physic::pixelToMeter(pixels); },
        [](const Vec2d& pixels) { return physic::pixelToMeter(pixels); },
        [](const Dim2d& pixels) { return physic::pixelToMeter(pixels); }
    );

    rmk["physic"]["meterToPixel"] = sol::overload(
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

    rmk["file"].get_or_create<sol::table>();
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