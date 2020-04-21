#include "Example.hpp"

#include "../generator.hpp"

#include <Geometry2D/Geometry.hpp>

namespace game {
const std::size_t Example::NUM_SHAPES = 20;
const ctp::gFloat Example::SHAPE_MIN_SIZE = 0.1f;
const ctp::gFloat Example::SHAPE_MAX_SIZE = 100.0f;
const std::size_t Example::POLY_MIN_VERTS = 3;
const std::size_t Example::POLY_MAX_VERTS = 20;

const Colour Example::SHAPE_COLOUR = Colour::LIGHT_BLUE;
const Colour Example::HIT_SHAPE_COLOUR = Colour::RED;

ctp::ShapeContainer Example::genShape() const {
	const ctp::gFloat rand(gen::gFloat(0.0f, 1.0f));
	if (rand < 0.2f)
		return ctp::ShapeContainer(genRect());
	if (rand < 0.70f)
		return ctp::ShapeContainer(genPoly());
	return ctp::ShapeContainer(genCircle());
}
ctp::Rect Example::genRect() const {
	return ctp::Rect(0, 0,
		gen::gFloat(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE),  // w
		gen::gFloat(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE)); // h
}
ctp::Polygon Example::genPoly() const {
	return gen::poly(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE, POLY_MIN_VERTS, POLY_MAX_VERTS);
}
ctp::Circle Example::genCircle() const {
	return ctp::Circle(gen::gFloat(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE));
}
}
