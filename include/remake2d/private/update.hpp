#ifndef REMAKE2D_UPDATE_
#define REMAKE2D_UPDATE_

#include <remake2d/tracker.hpp>
#include <remake2d/private/heritable.hpp>

namespace rmk {

class Updatable : Trackable<Updatable> {
public:
    virtual void update(void) = 0;

rmk_heritableBaseClass(Updatable);
};

} // namespace rmk
#endif