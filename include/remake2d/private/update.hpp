#ifndef REMAKE2D_UPDATE_
#define REMAKE2D_UPDATE_

#include <remake2d/tracker.hpp>
#include <remake2d/private/struct.hpp>

namespace rmk {

class Updatable : public Trackable<Updatable> {
public:
    virtual void update(void) = 0;

public:
    rmk_heritableBaseClass(Updatable);
};

} // namespace rmk
#endif