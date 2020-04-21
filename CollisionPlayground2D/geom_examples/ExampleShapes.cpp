#include "ExampleShapes.hpp"

#include <iostream>

#include "../generator.hpp"
#include "../Input.hpp"
#include "../Graphics.hpp"

namespace game {
ExampleShapes::ExampleShapes(ExampleType type, const ctp::Rect& levelRegion) : type_(type), level_region_(levelRegion) {
	_init();
}
void ExampleShapes::_init() {
	for (std::size_t i = 0; i < NUM_SHAPES; ++i)
		map_.add(new ctp::Wall(_gen_example_shape(), gen::coord2(level_region_)));
	_gen_mover();
}
void ExampleShapes::_gen_mover() {
	ctp::ShapeContainer collider = _gen_example_shape();
	ctp::Coord2 position = gen::coord2(level_region_);
	// Ensure that the mover doesn't start inside another shape (do collision tests until it is put down cleanly).
	// Just assume that it will always be possible to place the mover...
	for (;;) {
		if (std::none_of(map_.obstacles.cbegin(), map_.obstacles.cend(), [&](const auto& obs) { return ctp::overlaps(collider, position, obs->getCollider(), obs->getPosition()); }))
			break;
		collider = _gen_example_shape();
		position = gen::coord2(level_region_);
		std::cout << "Spot occupied. Trying somewhere else...\n";
	}
	std::cout << "Mover has entered the level.\n";
	mover_ = Mover(collider, position);
}
ctp::ShapeContainer ExampleShapes::_gen_example_shape() const {
	switch (type_) {
	case ExampleType::RECT:
		return ctp::ShapeContainer(Example::genRect());
	case ExampleType::POLY:
		return ctp::ShapeContainer(Example::genPoly());
	case ExampleType::CIRCLE:
		return ctp::ShapeContainer(Example::genCircle());
	case ExampleType::MIXED:
		return Example::genShape();
	default:
		std::cerr << "Unhandled example type.\n";
		return ctp::ShapeContainer(Example::genRect());
	}
}
void ExampleShapes::update(const Input& input, MS elapsedTime) {
	mover_.receiveInput(input);
	mover_.update(elapsedTime, map_);
}
void ExampleShapes::draw(const Graphics& graphics) {
	for (std::size_t i = 0; i < map_.size(); ++i) {
#ifdef DEBUG
		if (ctp::overlaps(mover_.getCollider(), mover_.getPosition(), map_[i]->getCollider(), map_[i]->getPosition()))
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
