#ifndef REMAKE2D_FORWARD_
#define REMAKE2D_FORWARD_

#include <remake2d/concept.hpp>

struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Point;
struct SDL_FPoint;
struct SDL_Vertex;
struct SDL_Texture;
struct TTF_Font;
struct SDL_GameController;
struct Mix_Music;
struct Mix_Chunk;

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
class ActorBase;
class TextureBase;
class TrackerBase;
class TrackableBase;

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
template<typename T> class Trackable;

template<IsTrackable T>    struct Slot
template<IsTrackable T>    class Tracker;
template<typename... Args> class Signal;
template<typename... Args> class Croutine;

}//namespace rmk
#endif