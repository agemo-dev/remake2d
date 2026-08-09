#include <remake2d/all/everything.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaTracker(void) noexcept {

    script._registerEngineType<Tracker<Followable>>  ("FollowableTracker");
    script._registerEngineType<Tracker<Window>>       ("WindowTracker");
    script._registerEngineType<Tracker<PhysicBody>>   ("PhysicBodyTracker");
    script._registerEngineType<Tracker<StaticBody>>   ("StaticBodyTracker");
    script._registerEngineType<Tracker<DynamicBody>>  ("DynamicBodyTracker");

}

} // namespace solstat
} // namespace config
} // namespace rmk
