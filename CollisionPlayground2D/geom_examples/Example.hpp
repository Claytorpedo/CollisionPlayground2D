#ifndef INCLUDE_GAME_EXAMPLE_HPP
#define INCLUDE_GAME_EXAMPLE_HPP

#include "../units.hpp"
#include "../Colour.hpp"

namespace ctp {
class ShapeContainer;
class Rect;
class Polygon;
class Circle;
}

class Input;
class Graphics;
namespace game {
class Example {
public:
	static const std::size_t  NUM_SHAPES;
	static const ctp::gFloat SHAPE_MIN_SIZE;
	static const ctp::gFloat SHAPE_MAX_SIZE;
	static const std::size_t  POLY_MIN_VERTS;
	static const std::size_t  POLY_MAX_VERTS;

	static const Colour	SHAPE_COLOUR;
	static const Colour	HIT_SHAPE_COLOUR;

	virtual ~Example() {}
	virtual void update(const Input& input, const MS elapsedTime) = 0;
	virtual void draw(const Graphics& graphics) = 0;
	virtual void reset() = 0;
protected:
	ctp::ShapeContainer genShape() const;
	ctp::Rect genRect() const;
	ctp::Polygon genPoly() const;
	ctp::Circle genCircle() const;
};
}

#endif // INCLUDE_GAME_EXAMPLE_HPP
