#include <remake2d/config/config.hpp>
#include <remake2d/script.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLua(void) noexcept {
    auto& table = script.m_table;

    initLuaType();
    initLuaClass();
    initLuaSignal();
    initLuaEntity();
    initLuaEvent(table);
    initLuaGlobal(table);
    initLuaUtility(table);
    initLuaSingleton(table);

    script.m_is_init = true;
}

} // namespace solstat
} // namespace config
} // namespace rmk