#ifndef REMAKE2D_SCRIPT_TPP_
#define REMAKE2D_SCRIPT_TPP_

namespace rmk {

template<typename T, typename... Ctors, typename B>
void SolState::registerType(std::string_view name, std::function<void(SolState::Type&)> init, B base_tuple) {
    if (m_loaded_types.find(std::string(name)) != m_loaded_types.end()) {
        rmk_dynamicAssert(rmk::ScriptError, (std::string(error::script::type_already_registered) + " : " + name.data()));
    }

    if constexpr (sizeof...(Ctors) == 0) {
        std::apply([this, &name](auto&&... unpacked_args) {
            m_state.new_usertype<T>(name.data(), sol::no_constructor, std::forward<decltype(unpacked_args)>(unpacked_args)...);
        }, std::forward<B>(base_tuple));
    } else {
        std::apply([this, &name](auto&&... unpacked_args) {
            m_state.new_usertype<T>(name.data(),
                sol::constructors<Ctors...>(),
                sol::call_constructor, sol::constructors<Ctors...>(),
                std::forward<decltype(unpacked_args)>(unpacked_args)...
            );
        }, std::forward<B>(base_tuple));
    }
    
    SolState::Type ut = m_state[name.data()];

	if constexpr (IsSignal<T>) {
        ut["join"]             = &T::join;
        ut["joinOnce"]         = &T::joinOnce;
        ut["joinPriority"]     = &T::joinPriority;
        ut["emit"]             = &T::emit;
        ut["bind"]             = &T::bind;
        ut["bindRising"]       = &T::bindRising;
        ut["bindFalling"]      = &T::bindFalling;
        ut["bindChange"]       = &T::bindChange;
        ut["start"]            = &T::start;
        ut["stop"]             = &T::stop;
        ut["count"]            = &T::count;
        ut["reserve"]          = &T::reserve;
    }
	
    if (init) init(ut);
	
    m_loaded_types.insert(std::string(name));
}

template<typename T>
void SolState::loadVar(std::string_view id, T& data) noexcept {
    m_state[id.data()] = &data;
}

template<typename T>
T Script::get(std::string_view id) {
    return m_env[id.data()].get<T>();
}

template<typename T, typename... Ctors, typename B>
void SolState::_registerEngineType(std::string_view name, std::function<void(SolState::Type&)> init, B base_tuple) {
    if (m_loaded_types.find(std::string(name)) != m_loaded_types.end()) {
        rmk_dynamicAssert(rmk::ScriptError, (std::string(error::script::type_already_registered) + " : " + name.data()));
    }

    if constexpr (sizeof...(Ctors) == 0) {
        std::apply([this, &name](auto&&... unpacked_args) {
            m_state.new_usertype<T>(name.data(), sol::no_constructor, std::forward<decltype(unpacked_args)>(unpacked_args)...);
        }, std::forward<B>(base_tuple));
    } else {
        std::apply([this, &name](auto&&... unpacked_args) {
            m_state.new_usertype<T>(name.data(),
                sol::constructors<Ctors...>(),
                sol::call_constructor, sol::constructors<Ctors...>(),
                std::forward<decltype(unpacked_args)>(unpacked_args)...
            );
        }, std::forward<B>(base_tuple));
    }

	SolState::Type ut = m_state[name.data()];

	if constexpr (IsSignal<T>) {
        ut["join"]             = &T::join;
        ut["joinOnce"]         = &T::joinOnce;
        ut["joinPriority"]     = &T::joinPriority;
        ut["emit"]             = &T::emit;
        ut["bind"]             = &T::bind;
        ut["bindRising"]       = &T::bindRising;
        ut["bindFalling"]      = &T::bindFalling;
        ut["bindChange"]       = &T::bindChange;
        ut["start"]            = &T::start;
        ut["stop"]             = &T::stop;
        ut["count"]            = &T::count;
        ut["reserve"]          = &T::reserve;
    }

    if (init) init(ut);

    m_table[name.data()] = ut;
    m_loaded_types.insert(std::string(name));
}


} // namespace rmk
#endif