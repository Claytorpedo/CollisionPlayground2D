#include "ExampleShapes.hpp"

#include <iostream>

#include "../generator.hpp"
#include "../Input.hpp"
#include "../Graphics.hpp"

namespace game {
	ExampleShapes::ExampleShapes(ExampleType type, const geom::Rect& levelRegion) : type_(type), level_region_(levelRegion) {
		_init();
	}
	void ExampleShapes::_init() {
		for (std::size_t i = 0; i < NUM_SHAPES; ++i)
			map_.add(new geom::Wall(_gen_example_shape(), gen::coord2(level_region_)));
		_gen_mover();
	}
	void ExampleShapes::_gen_mover() {
		geom::ShapeContainer collider(_gen_example_shape());
		geom::Coord2 position(gen::coord2(level_region_));
		// Ensure that the mover doesn't start inside another shape (do collision tests until it is put down cleanly).
		bool isOccupied = false;
		do {
			for (std::size_t i = 0; i < NUM_SHAPES; ++i) {
				if ((isOccupied = geom::overlaps(collider, position, map_[i]->getCollider(), map_[i]->getPosition()))) {
					collider = _gen_example_shape();
					position = gen::coord2(level_region_);
					std::cout << "Spot occupied. Trying somewhere else...\n";
					break;
				}
			}
		} while (isOccupied);
		std::cout << "Mover has entered the level.\n";
		mover_ = Mover(collider, position);
	}
	geom::ShapeContainer ExampleShapes::_gen_example_shape() const {
		switch (type_) {
		case ExampleType::RECT:
			return geom::ShapeContainer(Example::genRect());
		case ExampleType::POLY:
			return geom::ShapeContainer(Example::genPoly());
		case ExampleType::CIRCLE:
			return geom::ShapeContainer(Example::genCircle());
		case ExampleType::MIXED:
			return Example::genShape();
		default:
			std::cerr << "Unhandled example type.\n";
			return geom::ShapeContainer(Example::genRect());
		}
	}
	void ExampleShapes::update(const Input& input, MS elapsedTime) {
		mover_.receiveInput(input);
		mover_.update(elapsedTime, map_);
	}
	void ExampleShapes::draw(const Graphics& graphics) {
		for (std::size_t i = 0; i < map_.size(); ++i) {
#ifdef DEBUG
			if (geom::overlaps(mover_.getCollider(), mover_.getPosition(), map_[i]->getCollider(), map_[i]->getPosition()))
				graphics.setRenderColour(Example::HIT_SHAPE_COLOUR);
			else
				graphics.setRenderColour(Example::SHAPE_COLOUR);
#else
			graphics.setRenderColour(Example::SHAPE_COLOUR);
#endif
			graphics.renderShape(map_[i]->getCollider(), map_[i]->getPosition());
		}
		graphics.setRenderColour(Example::HIT_SHAPE_COLOUR);
		graphics.renderShape(mover_.getCollider(), mover_.getPosition());
	}
	void ExampleShapes::reset() {
		map_.clear();
		_init();
	}
}
