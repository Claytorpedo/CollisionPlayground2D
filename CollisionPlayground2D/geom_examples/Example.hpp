#pragma once
#ifndef INCLUDE_GAME_EXAMPLE_HPP
#define INCLUDE_GAME_EXAMPLE_HPP

#include "../units.hpp"

namespace geom {
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
		static const geom::gFloat SHAPE_MIN_SIZE;
		static const geom::gFloat SHAPE_MAX_SIZE;
		static const std::size_t  POLY_MIN_VERTS;
		static const std::size_t  POLY_MAX_VERTS;

		virtual ~Example() {}
		virtual void update(const Input& input, const MS elapsedTime) = 0;
		virtual void draw(const Graphics& graphics) = 0;
		virtual void reset() = 0;
	protected:
		geom::ShapeContainer genShape() const;
		geom::Rect genRect() const;
		geom::Polygon genPoly() const;
		geom::Circle genCircle() const;
	};
}

#endif // INCLUDE_GAME_EXAMPLE_HPP
