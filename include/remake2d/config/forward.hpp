#ifndef REMAKE2D_FORWARD_
#define REMAKE2D_FORWARD_

#include <remake2d/concept.hpp>

namespace rmk {

struct Area;
struct Vec2d;
struct Dim2d;
struct Grid2d;
struct Fact2d;

struct HSL;
struct Color;

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
class Timer;
class Chronometer;

class Camera;
class Window;
class TileMap;
class TileGrid;
class Parallax;

class Savable;
class Followable;

template<typename... Args> class Signal;
template<typename... Args> class Croutine;

}//namespace rmk
#endif