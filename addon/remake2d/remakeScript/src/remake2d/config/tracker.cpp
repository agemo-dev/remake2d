#include <remake2d/all/everything.hpp>
#include <remake2d/config/otracker.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaTracker(void) noexcept {

    script._registerEngineType<Tracker<Actor>>              ("Tracker::Actor");
    script._registerEngineType<Tracker<Window>>             ("Tracker::Window");
    script._registerEngineType<Tracker<Camera>>             ("Tracker::Camera");
    script._registerEngineType<Tracker<Trackable>>          ("Tracker::Trackable");
    script._registerEngineType<Tracker<Animation>>          ("Tracker::Animation");
    script._registerEngineType<Tracker<Followable>>         ("Tracker::Followable");
    script._registerEngineType<Tracker<PhysicBody>>         ("Tracker::PhysicBody");
    script._registerEngineType<Tracker<StaticBody>>         ("Tracker::StaticBody");
    script._registerEngineType<Tracker<DynamicBody>>        ("Tracker::DynamicBody");
    script._registerEngineType<Tracker<Window::Viewport>>   ("Tracker::Window::Viewport");

    script._registerEngineType<UnsafeTracker<Actor>>              ("UnsafeTracker::Actor");
    script._registerEngineType<UnsafeTracker<Window>>             ("UnsafeTracker::Window");
    script._registerEngineType<UnsafeTracker<Camera>>             ("UnsafeTracker::Camera");
    script._registerEngineType<UnsafeTracker<Trackable>>          ("UnsafeTracker::Trackable");
    script._registerEngineType<UnsafeTracker<Animation>>          ("UnsafeTracker::Animation");
    script._registerEngineType<UnsafeTracker<Followable>>         ("UnsafeTracker::Followable");
    script._registerEngineType<UnsafeTracker<PhysicBody>>         ("UnsafeTracker::PhysicBody");
    script._registerEngineType<UnsafeTracker<StaticBody>>         ("UnsafeTracker::StaticBody");
    script._registerEngineType<UnsafeTracker<DynamicBody>>        ("UnsafeTracker::DynamicBody");
    script._registerEngineType<UnsafeTracker<Window::Viewport>>   ("UnsafeTracker::Window::Viewport");

}

} // namespace solstat
} // namespace config
} // namespace rmk