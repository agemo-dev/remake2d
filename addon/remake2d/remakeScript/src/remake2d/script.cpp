#include <remake2d/script.hpp>

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

void SolState::_placeInTable(std::string_view path, SolState::Type ut) noexcept {
    usize sep = path.find("::");
    if (sep == std::string_view::npos) return;

    sol::table current = m_state.globals();
    usize      start   = 0;

    while (true) {
        sep = path.find("::", start);
        std::string segment = (sep == std::string_view::npos)
            ? std::string(path.substr(start))
            : std::string(path.substr(start, sep - start));

        if (sep == std::string_view::npos) {
            current[segment] = ut;
            break;
        }

        sol::table next = current[segment].get_or_create<sol::table>();
        current = next;
        start   = sep + 2;
    }

    m_state[std::string(path)] = sol::lua_nil;
}

Script::Script(std::string_view id) : m_file(id) {

    if (!std::filesystem::exists(m_file)) {
        rmk_dynamicAssert(rmk::ScriptError, std::string(error::script::file_nonexistent) + " : " + std::string(m_file));
    }
    m_env = sol::environment(script.m_state, sol::create, script.m_state.globals());

    m_last_write_time = std::filesystem::last_write_time(m_file);

    Tracker<Script> track = tracker();

    onFileChanged.bindRising([track]() mutable {
        if (!track) return false;

        std::error_code ec;
        auto current = std::filesystem::last_write_time(track->m_file, ec);
        if (ec || current == track->m_last_write_time) return false;   // file momentarily missing, or unchanged

        track->m_last_write_time = current;
        return true;
    });
}

void Script::update(void) {
	if (!script.m_is_init) config::solstat::initLua();

    try {
        script.m_state.script_file(m_file, m_env);
    } catch (const sol::error& e) {
        rmk_dynamicAssert(rmk::ScriptError, e.what());
    }
}

} // namespace rmk