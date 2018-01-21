#include "ExampleRays.hpp"

#include <iostream>
#include <algorithm>

#include "../generator.hpp"
#include "../Input.hpp"
#include "../Graphics.hpp"
#include "../util.hpp"

namespace game {

	const std::size_t ExampleRays::MAX_REFLECTIONS = 30;

	ExampleRays::ExampleRays(ExampleType type, const geom::Rect& levelRegion) : type_(type), level_region_(levelRegion),
		rotating_ray_(geom::Ray(level_region_.center(), geom::Coord2(1, 0)))
	{
		_init();
	}
	void ExampleRays::_init() {
		for (std::size_t i = 0; i < NUM_SHAPES; ++i)
			map_.add(new geom::Wall(Example::genShape(), gen::coord2(level_region_)));
	}
	void ExampleRays::update(const Input& input, const MS elapsedTime) {
		rotating_ray_.receiveInput(input);
		rotating_ray_.update(elapsedTime);
	}

	void ExampleRays::_draw_peircing(const Graphics& graphics) const {
		std::vector<SDL_Point> intersections;
		geom::gFloat near, far;
		const geom::Ray& r(rotating_ray_.getRay());
		for (std::size_t i = 0; i < map_.size(); ++i) {
			if (geom::intersects(r, map_[i]->getCollider(), map_[i]->getPosition(), near, far)) {
				intersections.push_back(util::coord2DToSDLPoint(r.origin + r.dir * near));
				intersections.push_back(util::coord2DToSDLPoint(r.origin + r.dir * far));
				graphics.setRenderColour(255, 0, 0);
			} else {
				graphics.setRenderColour(0, 100, 255);
			}
			graphics.renderShape(map_[i]->getCollider(), map_[i]->getPosition());
		}
		graphics.setRenderColour(255, 240, 0);
		graphics.renderRay(util::coord2DToSDLPoint(r.origin), r.dir.x, r.dir.y);
		graphics.setRenderColour(0, 240, 255);
		graphics.renderPoints(intersections, 4);
	}
	bool ExampleRays::_find_closest_isect(geom::Ray testRay,
		std::size_t& out_closest_ind, geom::gFloat& out_near, geom::Coord2& out_norm_near, geom::gFloat& out_far, geom::Coord2& out_norm_far) const {
		geom::gFloat closest(-1), testNear, testFar;
		geom::Coord2 testNormNear, testNormFar;
		for (std::size_t i = 0; i < map_.size(); ++i) {
			if (geom::intersects(testRay, map_[i]->getCollider(), map_[i]->getPosition(), testNear, testNormNear, testFar, testNormFar)) {
				if (closest == -1 || testNear < closest) {
					closest = testNear;
					out_closest_ind = i;
					out_near = testNear;
					out_norm_near = testNormNear;
					out_far = testFar;
					out_norm_far = testNormFar;
				}
				if (closest == 0.0f)
					return true;
			}
		}
		return closest != -1;
	}
	void ExampleRays::_draw_closest(const Graphics& graphics) const {
		const geom::Ray& r(rotating_ray_.getRay());
		std::size_t ind;
		geom::gFloat near, far;
		geom::Coord2 unused1, unused2;
		bool isCollision = _find_closest_isect(r, ind, near, unused1, far, unused2);
		// Draw results.
		for (std::size_t i = 0; i < map_.size(); ++i) {
			if (ind == i)
				graphics.setRenderColour(255, 0, 0);
			else 
				graphics.setRenderColour(0, 100, 255);
			graphics.renderShape(map_[i]->getCollider(), map_[i]->getPosition());
		}
		graphics.setRenderColour(240, 255, 0);
		graphics.renderRay(util::coord2DToSDLPoint(r.origin), r.dir.x, r.dir.y, isCollision ? static_cast<Uint16>(near) : 1000);
		if (isCollision) {
			graphics.setRenderColour(0, 240, 255);
			graphics.renderPoint(util::coord2DToSDLPoint(r.origin + r.dir * near), 4);
		}
	}
	bool ExampleRays::_find_reflection(geom::Ray testRay, std::size_t& out_ind, geom::gFloat& out_reflect_dist, geom::Ray& out_reflected) const {
		geom::gFloat near, far;
		geom::Coord2 norm_near, norm_far;
		if (!_find_closest_isect(testRay, out_ind, near, norm_near, far, norm_far))
			return false;
		if (near == 0.0f) { // Check if inside a shape.
			near = far; // Use the exit point.
			norm_near = -norm_far; // Reflect inside the shape.
		}
		const geom::Coord2 reflectedPos(testRay.origin + testRay.dir * geom::math::clamp(near - 0.01f, 0.0f, near));
		const geom::Coord2 reflectedDir(testRay.dir - 2.0f * norm_near * testRay.dir.dot(norm_near));
		out_reflected = geom::Ray(reflectedPos, reflectedDir);
		out_reflect_dist = near;
		return true;
	}
	void ExampleRays::_draw_reflecting(const Graphics& graphics) const {
		std::vector<int> indices; // Keep track of shapes that were hit.
		std::vector<SDL_Point> reflectPoints;
		std::size_t numReflects(0), ind;
		geom::Ray currentRay(rotating_ray_.getRay()), reflectedRay;
		geom::gFloat reflectDist;
		graphics.setRenderColour(240, 255, 0);
		while (numReflects < MAX_REFLECTIONS && _find_reflection(currentRay, ind, reflectDist, reflectedRay)) {
			graphics.renderRay(util::coord2DToSDLPoint(currentRay.origin), currentRay.dir.x, currentRay.dir.y, static_cast<Uint16>(reflectDist));

			indices.push_back(ind);
			reflectPoints.push_back(util::coord2DToSDLPoint(currentRay.origin + currentRay.dir * reflectDist));
			currentRay = reflectedRay;
			++numReflects;
		}
		if (numReflects < MAX_REFLECTIONS) // No reflections.
			graphics.renderRay(util::coord2DToSDLPoint(currentRay.origin), currentRay.dir.x, currentRay.dir.y);
		for (std::size_t i = 0; i < map_.size(); ++i) {
			if (std::find(indices.begin(), indices.end(), i) != indices.end())
				graphics.setRenderColour(255, 0, 0);
			else
				graphics.setRenderColour(0, 100, 255);
			graphics.renderShape(map_[i]->getCollider(), map_[i]->getPosition());
		}
		graphics.setRenderColour(0, 240, 255);
		graphics.renderPoints(reflectPoints, 4);
	}
	void ExampleRays::draw(const Graphics& graphics) {
		// Draw ray's origin.
		graphics.setRenderColour(255, 120, 0);
		graphics.renderCircle(util::coord2DToSDLPoint(rotating_ray_.getRay().origin), 5, 1);
		switch (type_) {
		case ExampleType::PEIRCING:
			_draw_peircing(graphics);
			return;
		case ExampleType::CLOSEST:
			_draw_closest(graphics);
			return;
		case ExampleType::REFLECTING:
			_draw_reflecting(graphics);
			return;
		default:
			std::cerr << "Unhandled example type.\n";
		}
	}
	void ExampleRays::reset() {
		map_.clear();
		_init();
	}
}
