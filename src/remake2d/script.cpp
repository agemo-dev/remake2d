#include <remake2d/error.hpp>
#include <remake2d/script.hpp>
#include <remake2d/config/config.hpp>

#include <iostream>

namespace rmk {

SolState::SolState(void) {
    m_state.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
    m_table = m_state["rmk"].get_or_create<sol::table>();
}

bool SolState::isLoadedType(std::string_view name) const noexcept {
    return m_loaded_types.find(std::string(name)) != m_loaded_types.end();
}

std::string SolState::loadedTypes(void) const noexcept {
    std::string result;
    for (const auto& type : m_loaded_types) {
        if (!result.empty()) result += "\n";
        result += type;
    }
    return result;
}

SolState& SolState::getInstance(void) noexcept {
    static SolState instance;
    return instance;
}

Script::Script(std::string_view id) : m_file(id) {
    if (!script.m_is_init) config::solstat::initLua();
    if (!std::filesystem::exists(m_file)) {
        rmk_dynamicAssert(rmk::ScriptError, std::string(error::script::file_unexist) + " : " + std::string(m_file));
    }
    m_env = sol::environment(script.m_state, sol::create, script.m_state.globals());

    onFileChanged.bindRising([this]() {
        static auto last = std::filesystem::last_write_time(m_file);
        auto current = std::filesystem::last_write_time(m_file);
        if (current != last) {
            last = current;
            return true;
        }
        return false;
    });
}

void Script::update(void) {
    try {
        script.m_state.script_file(m_file, m_env);
    } catch (const sol::error& e) {
        rmk_dynamicAssert(rmk::ScriptError, e.what());
    }
}

} // namespace rmk