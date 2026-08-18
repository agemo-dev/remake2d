#include <remake2d/all/everything.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaSignal(void) noexcept {

    script._registerEngineType<_EventSignal<>>                         ("_EventSignal");
    script._registerEngineType<_EventSignal<Vec2d>>                    ("_EventSignal::Vec2d");
    script._registerEngineType<_EventSignal<i32>>                      ("_EventSignal::i32");
    script._registerEngineType<_EventSignal<i32, i16>>                 ("_EventSignal::i32_i16");
    script._registerEngineType<_EventSignal<std::string>>              ("_EventSignal::string");

    script._registerEngineType<_EventSignal<u32>>                      ("_EventSignal::u32");
    script._registerEngineType<_EventSignal<u32, Dim2d>>               ("_EventSignal::u32_Dim2d");
    script._registerEngineType<_EventSignal<u32, Vec2d>>               ("_EventSignal::u32_Vec2d");

    script._registerEngineType<_PhysicSignal<>>                        ("_PhysicSignal::");
    script._registerEngineType<_PhysicSignal<DynamicBody*>>            ("_PhysicSignal::DynamicBody");
    script._registerEngineType<_PhysicSignal<PhysicBody*, PhysicBody*>>("_PhysicSignal::PhysicBody_PhysicBody");

    script._registerEngineType<_TimerSignal<>>                         ("_TimerSignal");
}

} // namespace solstat
} // namespace config
} // namespace rmk