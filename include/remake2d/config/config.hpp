#ifndef REMAKE2D_CONFIG_
#define REMAKE2D_CONFIG_

#include <remake2d/sol2/sol.hpp>

namespace rmk {
namespace config {

namespace system {
void initSDL(void);
} // namespace system

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

namespace sound {
void initQueue(void) noexcept;
} // namespace sound

} // namespace config
} // namespace rmk

#endif