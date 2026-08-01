#ifndef REMAKE2D_CONFIG_SCRIPT_
#define REMAKE2D_CONFIG_SCRIPT_

#include <remake2d/sol2/sol.hpp>

namespace rmk {
namespace config {

namespace solstat {
void initLua(void)       		   noexcept;
void initLuaType(void)   		   noexcept;
void initLuaClass(void)  		   noexcept;
void initLuaEntity(void)		   noexcept;
void initLuaSignal(void)		   noexcept;
void initLuaEvent(sol::table&)     noexcept;
void initLuaGlobal(sol::table&)    noexcept;
void initLuaUtility(sol::table&)   noexcept;
void initLuaSingleton(sol::table&) noexcept;
} // namespace solstat

} // namespace config
} // namespace rmk

#endif