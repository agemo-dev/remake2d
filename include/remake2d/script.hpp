#ifndef REMAKE2D_SCRIPT_
#define REMAKE2D_SCRIPT_

#include <remake2d/signal.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/sol2/sol.hpp>
#include <remake2d/config/script/config.hpp>

#include <set>
#include <string>
#include <filesystem>

namespace rmk {

namespace type {

template<typename... Bases> inline constexpr auto base = [] {
	if constexpr (sizeof...(Bases) == 0) {
		return std::tuple<>{};
	} else {
		return std::tuple{
			sol::base_classes, 
			sol::bases<Bases...>()
		};
	}
}();
	
template <typename... Args> inline auto overload(Args&&... args) {
	return sol::overload(std::forward<Args>(args)...);
}

} // namespace type

class SolState {
public:
    using Type = sol::table;

private:
    sol::state                  m_state;
    sol::table                  m_table;
    bool                        m_is_init{false};
    std::set<std::string>       m_loaded_types;

private:
    SolState(void);
    SolState(const SolState&)               = delete;
    SolState& operator=(const SolState&)    = delete;

public:
	template<typename T, typename... Ctors, typename B = const std::tuple<>>
	void registerType(std::string_view, std::function<void(SolState::Type&)> = nullptr, B = rmk::type::base<>);
    template<typename T> void loadVar(std::string_view id, T&) noexcept;

public:
    std::string loadedTypes(void) 		const noexcept;
    bool isLoadedType(std::string_view) const noexcept;

public:
    static SolState& getInstance(void) noexcept;

private:
	template<typename T, typename... Ctors, typename B = const std::tuple<>>
	void _registerEngineType(std::string_view, std::function<void(SolState::Type&)> = nullptr, B = rmk::type::base<>);
	
private:
    friend class Script;
    friend class System;
    friend void config::solstat::initLua(void) 				   noexcept;
    friend void config::solstat::initLuaType(void) 			   noexcept;
    friend void config::solstat::initLuaClass(void)			   noexcept;
    friend void config::solstat::initLuaEntity(void) 		   noexcept;
    friend void config::solstat::initLuaSignal(void) 		   noexcept;
    friend void config::solstat::initLuaEvent(sol::table&)     noexcept;
    friend void config::solstat::initLuaGlobal(sol::table&)    noexcept;
    friend void config::solstat::initLuaUtility(sol::table&)   noexcept;
    friend void config::solstat::initLuaSingleton(sol::table&) noexcept;
};

inline SolState& script = SolState::getInstance();


class Script {
private:
    sol::environment    m_env;
    std::string         m_file;

public:
    Signal<> onFileChanged;

public:
    Script(std::string_view);
    Script(Script&&)                    = default;
    Script(const Script&)               = delete;
    Script& operator=(Script&&)         = default;
    Script& operator=(const Script&)    = delete;

public:	
    void update(void);
    template<typename T> T get(std::string_view);
};

} // namespace rmk

#include <remake2d/template/script.tpp>

#endif