#ifndef REMAKE2D_ACTOR_TPP_
#define REMAKE2D_ACTOR_TPP_

namespace rmk {

template<IsPhysic P>
PhysicActor<P>::PhysicActor(const Geometry& shape) : Actor(), body(shape) {}

} // namespace rmk

#endif