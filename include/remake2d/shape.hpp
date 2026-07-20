#ifndef REMAKE2D_SHAPE_
#define REMAKE2D_SHAPE_

#include <remake2d/vector.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/numeric.hpp>

#include <SDL2/SDL.h>

#include <array>
#include <vector>
#include <algorithm>

namespace rmk {

class Color;

class Geometry {

protected:
    Dim2d             m_size{0.0f, 0.0f};
    Vec2d             m_center{0.0f, 0.0f};
    bool              m_is_changed{false};

protected:
    virtual void _build(void) 		noexcept = 0;
    virtual void _triangulate(void) noexcept = 0;

protected:
    virtual const Triangulation* _triangulations(void) const noexcept = 0;
    virtual std::vector<SDL_FPoint> _toContour(void)   const noexcept = 0;
    virtual std::vector<SDL_Vertex> _toVertices(void)  const noexcept = 0;

public:
    Geometry(void)                          = default;
    Geometry(Geometry&&)                    = default;
    Geometry(const Geometry&)               = default;
    Geometry& operator=(Geometry&&)         = default;
    Geometry& operator=(const Geometry&)    = default;

public:
    Geometry(const Vec2d&, const Dim2d&);

public:
    template<IsShape S> S as(void) const noexcept;
    
public:

    virtual void rotate(f32) 		  noexcept = 0;
    virtual void move(const Vec2d&)   noexcept = 0;
    virtual void resize(const Dim2d&) noexcept = 0;
    virtual void scale(const Fact2d&) noexcept = 0;
    virtual void transform(const Vec2d& , f32, const Fact2d&) noexcept = 0;

    virtual u8 points(void)   			 const noexcept = 0;
    virtual Dim2d size(void)   			 const noexcept = 0;
    virtual Vec2d center(void) 			 const noexcept = 0;
    virtual const Vec2d* pointsPos(void) const noexcept = 0;
  

    virtual bool hasIntersected(const Geometry&) const noexcept = 0;

public:
    virtual ~Geometry(void) = default;

private:
    friend class Window;
    friend class Camera;
    friend class PhysicBody;
    template<IsShape S> friend class Texture;
    
};


enum class point : u8 {
	min = 0,
	max = 50
};


template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
class Shape : public Geometry {
protected:
    u8                  m_n{POINT_COUNT};
    Vec2d               m_points[POINT_COUNT + 1];
    mutable SDL_FPoint  m_contour_cache[POINT_COUNT + 1];
    Triangulation       m_tris[POINT_COUNT >= 3 ? POINT_COUNT - 2 : 0];
    mutable SDL_Vertex  m_vertex_cache[POINT_COUNT >= 3 ? (POINT_COUNT - 2) * 3 : 0];

protected:
    void _triangulate(void)   noexcept override;
    virtual void _build(void) noexcept override;

protected:
    const Triangulation* _triangulations(void) const noexcept override;
    std::vector<SDL_FPoint> _toContour(void)   const noexcept override;
    std::vector<SDL_Vertex> _toVertices(void)  const noexcept override;

public:
  Shape(void) 									= default;
  Shape(Shape<POINT_COUNT>&&) 					= default;
  Shape(const Shape<POINT_COUNT>&)				= default;
  Shape& operator=(Shape<POINT_COUNT>&&)		= default;
  Shape& operator=(const Shape<POINT_COUNT>&)	= default;

public:
  Shape(const Vec2d&, const Dim2d&);

public:
    void rotate(f32) 									noexcept override;
    void move(const Vec2d&) 							noexcept override;
    virtual void scale(const Fact2d&) 					noexcept override;
	virtual void resize(const Dim2d&) 					noexcept override;
    void transform(const Vec2d& , f32, const Fact2d&)	noexcept override;

public:
  u8 points(void)				const noexcept override;
  Dim2d size(void)				const noexcept override;
  Vec2d center(void)			const noexcept override;
  const Vec2d* pointsPos(void)	const noexcept override;

public:
  bool hasIntersected(const Geometry&) const noexcept override;

public: 
  virtual ~Shape(void) = default;
  
private:
    friend class Window;
    friend class Camera;
    friend class PhysicBody;
    template<IsShape S> friend class Texture;
};


class Point : public Shape<1> {
public:
    Point(void)						= default;
    Point(Point&&)					= default;
    Point(const Point&)				= default;
    Point& operator=(Point&&)		= default;
    Point& operator=(const Point&)	= default;

public:
    Point(const Vec2d&);
	
public:
    void scale(const Fact2d&) noexcept override;
    void resize(const Dim2d&) noexcept override;
    
public:
    friend class Window;
    friend class Camera;
    friend class PhysicBody;
    template<IsShape S> friend class Texture;
};

class Circle : public Shape<36> {
public:
    Circle(void)						= default;
    Circle(Circle&&)					= default;
    Circle(const Circle&)				= default;
    Circle& operator=(Circle&&)			= default;
    Circle& operator=(const Circle&)	= default;

public:
    Circle(const Vec2d&, f32);

public:
    void scale(const Fact2d&) noexcept override;
    void resize(const Dim2d&) noexcept override;
    
public:
    friend class Window;
    friend class Camera;
    friend class PhysicBody;
    template<IsShape S> friend class Texture;
};

class Triangle : public Shape<3> {
public:
    Triangle(void)						    = default;
    Triangle(Triangle&&)					= default;
    Triangle(const Triangle&)				= default;
    Triangle& operator=(Triangle&&)			= default;
    Triangle& operator=(const Triangle&)	= default;

public:
    Triangle(const Vec2d&, const Dim2d&);
    
public:
    friend class Window;
    friend class Camera;
    friend class PhysicBody;
    template<IsShape S> friend class Texture;
};

class Rectangle : public Shape<4> {
public:
    Rectangle(void)							= default;
    Rectangle(Rectangle&&)					= default;
    Rectangle(const Rectangle&)				= default;
    Rectangle& operator=(Rectangle&&)		= default;
    Rectangle& operator=(const Rectangle&)	= default;

public:
    Rectangle(const Vec2d&, const Dim2d&);

private:
    void _build(void) noexcept override;
    
private:
    friend class Window;
    friend class Camera;
    friend class PhysicBody;
    template<IsShape S> friend class Texture;
};

class Square : public Rectangle {
public:
    Square(void)						= default;
    Square(Square&&)					= default;
    Square(const Square&)				= default;
    Square& operator=(Square&&)			= default;
    Square& operator=(const Square&)	= default;

public:
    Square(const Vec2d&, f32);

public:
	void scale(const Fact2d&) noexcept override;
    void resize(const Dim2d&) noexcept override;
    
public:
    friend class Window;
    friend class Camera;
    friend class PhysicBody;
    template<IsShape S> friend class Texture;
};


using Line      = Shape<2>;
using Losange   = Shape<4>;
using Hexagone  = Shape<6>;
using Ellipse   = Shape<36>;

template<> Circle Geometry::as(void) const noexcept;
template<> Square Geometry::as(void) const noexcept;

}//namespace rmk

#include <remake2d/template/shape.tpp>

#endif