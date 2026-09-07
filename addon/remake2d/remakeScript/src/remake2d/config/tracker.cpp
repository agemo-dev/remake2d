#include <remake2d/all/everything.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaTracker(void) noexcept {

    script._registerEngineType<Tracker<Actor>>              ("Tracker::Actor");
    script._registerEngineType<Tracker<Window>>             ("Tracker::Window");
    script._registerEngineType<Tracker<Camera>>             ("Tracker::Camera");
    script._registerEngineType<Tracker<Animation>>          ("Tracker::Animation");
    script._registerEngineType<Tracker<Followable>>         ("Tracker::Followable");
    script._registerEngineType<Tracker<PhysicBody>>         ("Tracker::PhysicBody");
    script._registerEngineType<Tracker<Window::Viewport>>   ("Tracker::Window::Viewport");

}

} // namespace solstat
} // namespace config
} // namespace rmk