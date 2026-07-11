#ifndef REMAKE2D_CONCEPT_
#define REMAKE2D_CONCEPT_

#include <concepts>

namespace rmk {

template<typename G>
concept IsShape = std::derived_from<G, class Geometry>;

template<typename T>
concept IsTexture = std::derived_from<T, class TextureBase>;

template<typename P>
concept IsPhysic = std::derived_from<P, class PhysicBody>;

template<typename A>
concept IsActor  = std::derived_from<A, class ActorBase>;

template<typename S>
concept IsSignal = std::derived_from<S, class SignalBase>;

template<typename S>
concept IsSavable = std::derived_from<S, class ISavable>;

template<typename T>
concept IsBasicType = std::same_as<T, struct Vec2d>  		|| 
                      std::same_as<T, struct Fact2d> 		|| 
                      std::same_as<T, struct Grid2d> 		|| 
                      std::same_as<T, struct Dim2d>  		||
                      std::same_as<T, struct Area>   		||
                      std::same_as<T, struct Triangulation> ||
                      std::same_as<T, struct Color>  		||
                      std::same_as<T, struct HSL>			||
					  std::is_arithmetic_v<T>;

} // namespace rmk
#endif