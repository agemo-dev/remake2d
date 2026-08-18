#include <remake2d/all/everything.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaTracker(void) noexcept {

    script._registerEngineType<Tracker<Window>>       ("Tracker::Window");
    script._registerEngineType<Tracker<ActorBase>>    ("Tracker::ActorBase");
    script._registerEngineType<Tracker<Followable>>   ("Tracker::Followable");
    script._registerEngineType<Tracker<PhysicBody>>   ("Tracker::PhysicBody");

}

} // namespace solstat
} // namespace config
} // namespace rmk