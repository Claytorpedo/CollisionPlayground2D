#include "ExampleShapes.hpp"

#include <iostream>

#include "../generator.hpp"
#include "../Input.hpp"
#include "../Graphics.hpp"

namespace game {
	const std::size_t  ExampleShapes::NUM_SHAPES = 20;
	const geom::gFloat ExampleShapes::SHAPE_MIN_SIZE = 0.1f;
	const geom::gFloat ExampleShapes::SHAPE_MAX_SIZE = 100.0f;
	const std::size_t  ExampleShapes::POLY_MIN_VERTS = 3;
	const std::size_t  ExampleShapes::POLY_MAX_VERTS = 20;

	ExampleShapes::ExampleShapes(ExampleType type, const geom::Rect& levelRegion) : type_(type), level_region_(levelRegion) {
		_init();
	}
	void ExampleShapes::_init() {
		for (std::size_t i = 0; i < NUM_SHAPES; ++i)
			map_.add(new geom::Wall(_gen_shape(), gen::coord2(level_region_)));
		_gen_mover();
	}
	void ExampleShapes::_gen_mover() {
		geom::ShapeContainer collider(_gen_shape());
		geom::Coord2 position(gen::coord2(level_region_));
		// Ensure that the mover doesn't start inside another shape (do collision tests until it is put down cleanly).
		bool isOccupied = false;
		do {
			for (std::size_t i = 0; i < NUM_SHAPES; ++i) {
				if ((isOccupied = geom::overlaps(collider, position, map_[i]->getCollider(), map_[i]->getPosition()))) {
					collider = _gen_shape();
					position = gen::coord2(level_region_);
					std::cout << "Spot occupied. Trying somewhere else...\n";
					break;
				}
			}
		} while (isOccupied);
		std::cout << "Mover has entered the level.\n";
		mover_ = Mover(collider, position);
	}
	geom::ShapeContainer ExampleShapes::_gen_shape() const {
		switch (type_) {
		case ExampleType::RECT:
			return geom::ShapeContainer(_gen_rect());
		case ExampleType::POLY:
			return geom::ShapeContainer(_gen_poly());
		case ExampleType::CIRCLE:
			return geom::ShapeContainer(_gen_circle());
		case ExampleType::MIXED:
		{
			const geom::gFloat rand(gen::gFloat(0.0f, 1.0f));
			if (rand < 0.33f)
				return geom::ShapeContainer(_gen_rect());
			if (rand < 0.66f)
				return geom::ShapeContainer(_gen_poly());
			return geom::ShapeContainer(_gen_circle());
		}
		default:
			std::cerr << "Unhandled example type.\n";
			return geom::ShapeContainer();
		}
	}
	geom::Rect ExampleShapes::_gen_rect() const {
		return geom::Rect(0, 0,
			gen::gFloat(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE),  // w
			gen::gFloat(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE)); // h
	}
	geom::Polygon ExampleShapes::_gen_poly() const {
		return gen::poly(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE, POLY_MIN_VERTS, POLY_MAX_VERTS);
	}
	geom::Circle ExampleShapes::_gen_circle() const {
		return geom::Circle(gen::gFloat(SHAPE_MIN_SIZE, SHAPE_MAX_SIZE));
	}
	void ExampleShapes::update(const Input& input, MS elapsedTime) {
		mover_.receiveInput(input);
		mover_.update(elapsedTime, map_);
	}
	void ExampleShapes::draw(const Graphics& graphics) {
		for (std::size_t i = 0; i < map_.size(); ++i) {
#ifdef DEBUG
			if (geom::overlaps(mover_.getCollider(), mover_.getPosition(), map_[i]->getCollider(), map_[i]->getPosition()))
				graphics.setRenderColour(255, 0, 0);
			else
				graphics.setRenderColour(0, 100, 255);
#else
			graphics.setRenderColour(0, 100, 255);
#endif
			graphics.renderShape(map_[i]->getCollider(), map_[i]->getPosition());
		}
		graphics.setRenderColour(255, 0, 0);
		graphics.renderShape(mover_.getCollider(), mover_.getPosition());
	}
	void ExampleShapes::reset() {
		map_.clear();
		_init();
	}
}
