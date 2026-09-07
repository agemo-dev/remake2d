#include <remake2d/all/everything.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaTrait(void) noexcept {

    script._registerEngineType<Drawable>      ("Drawable");
    script._registerEngineType<Fillable>      ("Fillable");
    script._registerEngineType<Followable>    ("Followable");

    script._registerEngineType<Trackable<Actor>>             ("Trackable::Actor");
    script._registerEngineType<Trackable<Camera>>            ("Trackable::Camera");
    script._registerEngineType<Trackable<Window>>            ("Trackable::Window");
    script._registerEngineType<Trackable<Animation>>         ("Trackable::Animation");
    script._registerEngineType<Trackable<PhysicBody>>        ("Trackable::PhysicBody");
    script._registerEngineType<Trackable<Followable>>        ("Trackable::Followable");
    script._registerEngineType<Trackable<Window::Viewport>>  ("Trackable::Window::Viewport");
}

} // namespace solstat
} // namespace config
} // namespace rmk