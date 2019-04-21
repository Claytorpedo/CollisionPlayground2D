#include "Example.hpp"

#include "../generator.hpp"

#include <Geometry2D/Geometry.hpp>

namespace game {
	const std::size_t  Example::NUM_SHAPES = 20;
	const geom::gFloat Example::SHAPE_MIN_SIZE = 0.1f;
	const geom::gFloat Example::SHAPE_MAX_SIZE = 100.0f;
	const std::size_t  Example::POLY_MIN_VERTS = 3;
	const std::size_t  Example::POLY_MAX_VERTS = 20;

	const Colour Example::SHAPE_COLOUR     = Colour::LIGHT_BLUE;
	const Colour Example::HIT_SHAPE_COLOUR = Colour::RED;

	geom::ShapeContainer Example::genShape() const {
		const geom::gFloat rand(gen::gFloat(0.0f, 1.0f));
		if (rand < 0.2f)
			return geom::ShapeContainer(genRect());
		if (rand < 0.70f)
			return geom::ShapeContainer(genPoly());
		return geom::ShapeContainer(genCircle());
	}
	geom::Rect Example::genRect() const {
		return geom::Rect(0, 0,
			gen::gFloat(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE),  // w
			gen::gFloat(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE)); // h
	}
	geom::Polygon Example::genPoly() const {
		return gen::poly(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE, POLY_MIN_VERTS, POLY_MAX_VERTS);
	}
	geom::Circle Example::genCircle() const {
		return geom::Circle(gen::gFloat(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE));
	}
}
