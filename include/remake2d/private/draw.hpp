#ifndef REMAKE2D_DRAW_
#define REMAKE2D_DRAW_

#include <remake2d/color.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/config/forward.hpp>
#include <remake2d/private/heritable.hpp>

#include <vector>

namespace rmk {

struct Vertex {
	f32   x{0.0f}, y{0.0f};
	Color color{};
	f32   u{0.0f}, v{0.0f};

public:
	constexpr Vertex(void)                    = default;
	constexpr Vertex(Vertex&&)                = default;
	constexpr Vertex(const Vertex&)           = default;
	constexpr Vertex& operator=(Vertex&&)     = default;
	constexpr Vertex& operator=(const Vertex&)= default;
	constexpr Vertex(f32 X, f32 Y, Color C, f32 U = 0.0f, f32 V = 0.0f) : x(X), y(Y), color(C), u(U), v(V) {}

public:
	operator SDL_Vertex(void) const;
};

struct VertexBatch {
	SDL_Texture*         texture{nullptr};
	std::vector<Vertex>  vertices{};
};

struct DrawPack {
    Color  color;
    std::vector<SDL_FPoint> points;
};

namespace contour {
SDL_FPoint  		 breaker(void)			 noexcept;
bool	 	 		 isBreak(SDL_FPoint)     noexcept;
} // namespace contour


class Printable {
public:
	Color        m_color{color::white};
	u16          m_layer{0};
	f32          m_zoom{1.0f};
	mutable bool m_is_dirty{true};
	mutable bool m_is_fill_dirty{true};

public:
	void  color(Color)      noexcept;
	Color color(void) const noexcept;

public:
	void layer(u16)        noexcept;
	u16  layer(void) const noexcept;

rmk_heritableBaseClass(Printable);
};


class Drawable : public virtual Printable {
public:
	mutable std::vector<DrawPack> __draw_cache__{};

public:
    virtual void draw(const Drawable&) const noexcept = 0;
	const std::vector<DrawPack>& __draw__(void) const;

rmk_heritableBaseClass(Drawable);
};


class Fillable : public virtual Printable {
public:
	mutable std::vector<VertexBatch> __fill_cache__{};

public:
	const std::vector<VertexBatch>& __fill__(void) const;
    virtual void fill(const Fillable&) const noexcept = 0;

rmk_heritableBaseClass(Fillable);
};

} // namespace rmk

#endif
