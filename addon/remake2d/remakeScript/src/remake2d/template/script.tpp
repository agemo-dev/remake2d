#ifndef REMAKE2D_SCRIPT_TPP_
#define REMAKE2D_SCRIPT_TPP_

namespace rmk {

namespace type {

template<typename T>
concept _HasAddition  = requires(const T& a, const T& b) { { a + b } -> std::same_as<T>; };

template<typename T>
concept _HasSubtraction    = requires(const T& a, const T& b) { { a - b } -> std::same_as<T>; };

template<typename T>
concept _HasMultiplication = requires(const T& a, const T& b) { { a * b } -> std::same_as<T>; };

template<typename T>
concept _HasDivision  = requires(const T& a, const T& b) { { a / b } -> std::same_as<T>; };

template<typename T>
concept _HasModulus   = requires(const T& a, const T& b) { { a % b } -> std::same_as<T>; };

template<typename T>
concept _HasEqual     = requires(const T& a, const T& b) { { a == b } -> std::convertible_to<bool>; };

template<typename T>
concept _HasLess      = requires(const T& a, const T& b) { { a < b } -> std::convertible_to<bool>; };

template<typename T>
concept _HasLessEqual = requires(const T& a, const T& b) { { a <= b } -> std::convertible_to<bool>; };

template<typename T>
concept _HasSize = requires (const T& a) { { a.size() } -> std::convertible_to<usize>; };

template<typename T>
concept _HasOS = requires(std::ostream& os, const T& a) { { os << a } -> std::convertible_to<std::ostream&>; };

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

    _generateOperator<T>(ut);
    _generateSpecialType<T>(ut);

    if (init) init(ut);

    m_loaded_types.insert(std::string(name));
}

template<typename T>
void SolState::_generateSpecialType(SolState::Type& ut) noexcept {

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

}

template<typename T>
void SolState::_generateOperator(SolState::Type& ut) noexcept {

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

    if constexpr (type::_HasEqual<T>) {
        ut[sol::meta_function::equal_to] = [](const T& a, const T& b) -> bool { return a == b; };
    }
    if constexpr (type::_HasLess<T>) {
        ut[sol::meta_function::less_than] = [](const T& a, const T& b) -> bool { return a < b; };
    }
    if constexpr (type::_HasLessEqual<T>) {
        ut[sol::meta_function::less_than_or_equal_to] = [](const T& a, const T& b) -> bool { return a <= b; };
    }

    if constexpr (type::_HasSize<T>) {
        ut[sol::meta_function::length] = [](const T& a) -> usize { return a.size(); };
    }

    if constexpr (type::_HasOS<T>) {
        ut[sol::meta_function::to_string] = [](const T& a) -> std::string {
            std::ostringstream oss;
            oss << a;
            return oss.str();
        };
    }

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