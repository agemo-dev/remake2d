#include <remake2d/all/everything.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaSignal(void) noexcept {

    script._registerEngineType<_EventSignal<>>                         ("_EventSignal");
    script._registerEngineType<_EventSignal<Vec2d>>                    ("_EventSignalVec2d");
    script._registerEngineType<_EventSignal<i32>>                      ("_EventSignalI32");
    script._registerEngineType<_EventSignal<i32, i16>>                 ("_EventSignalI32I16");
    script._registerEngineType<_EventSignal<std::string>>              ("_EventSignalString");

    script._registerEngineType<_EventSignal<u32>>                      ("_EventSignalU32");
    script._registerEngineType<_EventSignal<u32, Dim2d>>               ("_EventSignalU32Dim2d");
    script._registerEngineType<_EventSignal<u32, Vec2d>>               ("_EventSignalU32Vec2d");

    script._registerEngineType<_PhysicSignal<>>                        ("_PhysicSignal");
    script._registerEngineType<_PhysicSignal<DynamicBody*>>            ("_PhysicSignalDynamicBody");
    script._registerEngineType<_PhysicSignal<PhysicBody*, PhysicBody*>>("_PhysicSignalPhysicBodyPhysicBody");

    script._registerEngineType<_TimerSignal<>>                         ("_TimerSignal");
}

} // namespace solstat
} // namespace config
} // namespace rmk