#ifndef REMAKE2D_FORWARD_
#define REMAKE2D_FORWARD_

#include <remake2d/concept.hpp>

namespace rmk {

class PhysicBody;
class StaticBody;
class DynamicBody;

class Geometry;

class Text;
class Sprite;
class Animation;
class TextureBase;
template<IsShape S> class Texture;

class Date;
class Clock;

class Camera;
class Window;
class TileMap;
class TileGrid;
class Parallax;

class ISavable;
template<typename... Args> class Croutine;

}//namespace rmk
#endif