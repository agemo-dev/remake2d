#ifndef REMAKE2D_SCRIPT_TPP_
#define REMAKE2D_SCRIPT_TPP_

namespace rmk {

namespace type {
template<typename T>
concept _HasAddition = requires(const T& a, const T& b) { { a + b } -> std::same_as<T>; };

template<typename T>
concept _HasSubtraction = requires(const T& a, const T& b) { { a - b } -> std::same_as<T>; };

template<typename T>
concept _HasMultiplication = requires(const T& a, const T& b) { { a * b } -> std::same_as<T>; };

template<typename T>
concept _HasDivision = requires(const T& a, const T& b) { { a / b } -> std::same_as<T>; };

template<typename T>
concept _HasModulus = requires(const T& a, const T& b) { { a % b } -> std::same_as<T>; };
} // namespace type

template<typename T, typename... Ctors, typename B, typename... Fields>
void SolState::registerType(std::string_view name, std::function<void(SolState::Type&)> init, B base_tuple, Fields... fields) {
    if (m_loaded_types.find(std::string(name)) != m_loaded_types.end()) {
        rmk_dynamicAssert(rmk::ScriptError, (std::string(error::script::type_already_registered) + " : " + name.data()));
    }

    if constexpr (sizeof...(Ctors) == 0) {
        std::apply([this, &name, &fields...](auto&&... unpacked_args) {
            m_state.new_usertype<T>(name.data(), sol::no_constructor,
                std::forward<decltype(unpacked_args)>(unpacked_args)...,
                std::forward<Fields>(fields)...
            );
        }, std::forward<B>(base_tuple));
    } else {
        std::apply([this, &name, &fields...](auto&&... unpacked_args) {
            m_state.new_usertype<T>(name.data(),
                sol::constructors<Ctors...>(),
                sol::call_constructor, sol::constructors<Ctors...>(),
                std::forward<decltype(unpacked_args)>(unpacked_args)...,
                std::forward<Fields>(fields)...
            );
        }, std::forward<B>(base_tuple));
    }

	SolState::Type ut = m_state[name.data()];
	_placeInTable(name, ut);

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
    } else if constexpr (IsTracker<T>) {
		ut["locate"] = [](T& self) { return self.locate(); };
	}

    if constexpr (type::_HasAddition<T>) {
        ut[sol::meta_function::addition] = [](const T& a, const T& b) -> T { return a + b; };
    }
	if constexpr (type::_HasSubtraction<T>) {
        ut[sol::meta_function::subtraction] = [](const T& a, const T& b) -> T { return a - b; };
    }
    if constexpr (type::_HasMultiplication<T>) {
        ut[sol::meta_function::multiplication] = [](const T& a, const T& b) -> T { return a * b; };
    }
    if constexpr (type::_HasDivision<T>) {
        ut[sol::meta_function::division] = [](const T& a, const T& b) -> T { return a / b; };
    }
    if constexpr (type::_HasModulus<T>) {
        ut[sol::meta_function::modulus] = [](const T& a, const T& b) -> T { return a % b; };
    }

    if (init) init(ut);

    m_loaded_types.insert(std::string(name));
}

template<typename T, typename... Ctors, typename B, typename... Fields>
void SolState::_registerEngineType(std::string_view name, std::function<void(SolState::Type&)> init, B base_tuple, Fields... fields) {
	registerType<T, Ctors...>(std::string("rmk::") + std::string(name), init, base_tuple, fields...);
}

template<typename T>
void SolState::loadVar(std::string_view id, T& data) noexcept {
    m_state[id.data()] = &data;
}

template<typename T>
T Script::get(std::string_view id) {
    return m_env[id.data()].get<T>();
}

} // namespace rmk
#endif