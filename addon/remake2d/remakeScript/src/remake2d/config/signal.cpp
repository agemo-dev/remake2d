#include <remake2d/all/everything.hpp>
#include <remake2d/config/otracker.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaSignal(void) noexcept {

    script._registerEngineType<_EventSignal<>>                         ("_EventSignal::void", [](SolState::Type& ut) {
        ut["isActive"] = &_EventSignal<>::isActive;
    });
    script._registerEngineType<_EventSignal<Vec2d>>                    ("_EventSignal::Vec2d", [](SolState::Type& ut) {
        ut["isActive"] = &_EventSignal<Vec2d>::isActive;
    });
    script._registerEngineType<_EventSignal<i32>>                      ("_EventSignal::i32", [](SolState::Type& ut) {
        ut["isActive"] = &_EventSignal<i32>::isActive;
    });
    script._registerEngineType<_EventSignal<i32, i16>>                 ("_EventSignal::i32_i16", [](SolState::Type& ut) {
        ut["isActive"] = &_EventSignal<i32, i16>::isActive;
    });
    script._registerEngineType<_EventSignal<std::string>>              ("_EventSignal::string", [](SolState::Type& ut) {
        ut["isActive"] = &_EventSignal<std::string>::isActive;
    });

    script._registerEngineType<_EventSignal<u32>>                      ("_EventSignal::u32", [](SolState::Type& ut) {
        ut["isActive"] = &_EventSignal<u32>::isActive;
    });
    script._registerEngineType<_EventSignal<u32, Dim2d>>               ("_EventSignal::u32_Dim2d", [](SolState::Type& ut) {
        ut["isActive"] = &_EventSignal<u32, Dim2d>::isActive;
    });
    script._registerEngineType<_EventSignal<u32, Vec2d>>               ("_EventSignal::u32_Vec2d", [](SolState::Type& ut) {
        ut["isActive"] = &_EventSignal<u32, Vec2d>::isActive;
    });

    script._registerEngineType<_PhysicSignal<>>                                        ("_PhysicSignal::void");
    script._registerEngineType<_PhysicSignal<Tracker<DynamicBody>>>                    ("_PhysicSignal::DynamicBody");
    script._registerEngineType<_PhysicSignal<Tracker<PhysicBody>, Tracker<PhysicBody>>>("_PhysicSignal::PhysicBody_PhysicBody");

    script._registerEngineType<_TimerSignal<>>                         ("_TimerSignal");
}

} // namespace solstat
} // namespace config
} // namespace rmk