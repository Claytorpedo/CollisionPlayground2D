#include "ExamplePoly.hpp"

#include "../generator.hpp"
#include "../Input.hpp"
#include "../Graphics.hpp"

namespace game {
	const std::size_t  ExamplePoly::NUM_POLYS = 20;
	const geom::gFloat ExamplePoly::POLY_MIN_RAD   = 0.1f;
	const geom::gFloat ExamplePoly::POLY_MAX_RAD   = 100.0f;
	const std::size_t  ExamplePoly::POLY_MIN_VERTS = 3;
	const std::size_t  ExamplePoly::POLY_MAX_VERTS = 20;

	ExamplePoly::ExamplePoly(const geom::Rect& levelRegion) : level_region_(levelRegion) {
		init_();
	}
	void ExamplePoly::init_() {
		for (std::size_t i = 0; i < NUM_POLYS; ++i)
			map_.add(new geom::Wall(gen::poly(POLY_MIN_RAD, POLY_MAX_RAD, POLY_MIN_VERTS, POLY_MAX_VERTS), gen::coord2(level_region_)));
		gen_mover_();
	}
	void ExamplePoly::gen_mover_() {
		geom::Polygon collider(gen::poly(POLY_MIN_RAD, POLY_MAX_RAD, POLY_MIN_VERTS, POLY_MAX_VERTS));
		geom::Coord2 position(gen::coord2(level_region_));
		// Ensure that the moving polygon doesn't start inside another polygon (do collision tests until it is put down cleanly).
		bool isOccupied = false;
		do {
			for (std::size_t i = 0; i < NUM_POLYS; ++i) {
				if (isOccupied = geom::overlaps(collider, position, map_[i]->getCollider(), map_[i]->getPosition())) {
					collider = gen::poly(POLY_MIN_RAD, POLY_MAX_RAD, POLY_MIN_VERTS, POLY_MAX_VERTS);
					position = gen::coord2(level_region_);
					std::cout << "Spot occupied. Trying somewhere else...\n";
					break;
				}
			}
		} while (isOccupied);
		std::cout << "Mover has entered the level.\n";
		mover_ = Mover(geom::ShapeContainer(collider), position);
	}
	void ExamplePoly::update(Input& input, MS elapsedTime) {
		mover_.receiveInput(input);
		mover_.update(elapsedTime, map_);
	}
	void ExamplePoly::draw(Graphics& graphics) {
		for (std::size_t i = 0; i < map_.size(); ++i) {
#ifdef DEBUG
			if (geom::overlaps(mover_.getCollider().poly(), mover_.getPosition(), map_[i]->getCollider().poly(), map_[i]->getPosition()))
				graphics.setRenderColour(255, 0, 0);
			else
				graphics.setRenderColour(0, 100, 255);
#else
			graphics.setRenderColour(0, 100, 255);
#endif
			graphics.renderPoly(map_[i]->getCollider().poly(), map_[i]->getPosition());
		}
		graphics.setRenderColour(255, 0, 0);
		graphics.renderPoly(mover_.getCollider().poly(), mover_.getPosition());
	}
	void ExamplePoly::reset() {
		map_.clear();
		init_();
	}
}
