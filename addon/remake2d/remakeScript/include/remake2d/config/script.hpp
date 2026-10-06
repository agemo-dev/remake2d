#ifndef REMAKE2D_CONFIG_SCRIPT_
#define REMAKE2D_CONFIG_SCRIPT_

#define SOL_DEFAULT_AUTOMAGICAL_USERTYPES 0

#include <remake2d/sol2/sol.hpp>
#include <remake2d/config/export.hpp>

namespace rmk {
namespace config {
namespace solstat {

RMK_SCRIPT_HIDDEN void initLua(void)       noexcept;
RMK_SCRIPT_HIDDEN void initLuaType(void)   noexcept;
RMK_SCRIPT_HIDDEN void initLuaTrait(void)  noexcept;
RMK_SCRIPT_HIDDEN void initLuaClass(void)  noexcept;
RMK_SCRIPT_HIDDEN void initLuaEntity(void) noexcept;
RMK_SCRIPT_HIDDEN void initLuaSignal(void) noexcept;
RMK_SCRIPT_HIDDEN void initLuaTracker(void) noexcept;

RMK_SCRIPT_HIDDEN void initLuaEvent(sol::table&)     noexcept;
RMK_SCRIPT_HIDDEN void initLuaGlobal(sol::table&)    noexcept;
RMK_SCRIPT_HIDDEN void initLuaUtility(sol::table&)   noexcept;
RMK_SCRIPT_HIDDEN void initLuaSingleton(sol::table&) noexcept;

} // namespace solstat

} // namespace config
} // namespace rmk

#endif