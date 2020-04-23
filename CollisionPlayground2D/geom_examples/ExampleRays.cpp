#include "ExampleRays.hpp"

#include <iostream>
#include <algorithm>

#include "../generator.hpp"
#include "../Input.hpp"
#include "../Graphics.hpp"
#include "../util.hpp"

namespace game {

const std::size_t ExampleRays::MAX_REFLECTIONS = 30;
const Uint16      ExampleRays::MAX_RAY_LENGTH = 1000;
const Uint8       ExampleRays::HIT_POINT_SIZE = 4;

const Colour ExampleRays::RAY_COLOUR = Colour::YELLOW;
const Colour ExampleRays::RAY_ORIGIN_COLOUR = Colour::ORANGE;
const Colour ExampleRays::HIT_POINT_COLOUR = Colour::CYAN;
const Colour ExampleRays::RAY_REFLECT_COLOURS[] = {RAY_COLOUR, Colour::LIGHT_GREEN, Colour::FUCHSIA, Colour::ORANGE};
const std::size_t ExampleRays::NUM_REFLECT_COLOURS = 4;

ExampleRays::ExampleRays(ExampleType type, const ctp::Rect& levelRegion)
	: type_(type), level_region_(levelRegion), rotating_ray_{ctp::Ray{level_region_.center(), ctp::Coord2(1, 0)}} {
	_init();
}
void ExampleRays::_init() {
	for (std::size_t i = 0; i < NUM_SHAPES; ++i)
		map_.add(new ctp::Wall(Example::genShape(), gen::coord2(level_region_)));
}
void ExampleRays::update(const Input& input, const MS elapsedTime) {
	rotating_ray_.receiveInput(input);
	rotating_ray_.update(elapsedTime);
}

void ExampleRays::_draw_peircing(const Graphics& graphics) const {
	std::vector<SDL_Point> intersections;
	ctp::gFloat near, far;
	const ctp::Ray& r(rotating_ray_.getRay());
	for (std::size_t i = 0; i < map_.size(); ++i) {
		if (ctp::intersects(r, map_[i]->getCollider(), map_[i]->getPosition(), near, far)) {
			intersections.push_back(util::coord2DToSDLPoint(r.origin + r.dir * near));
			intersections.push_back(util::coord2DToSDLPoint(r.origin + r.dir * far));
			graphics.setRenderColour(Example::HIT_SHAPE_COLOUR);
		} else {
			graphics.setRenderColour(Example::SHAPE_COLOUR);
		}
		graphics.renderShape(map_[i]->getCollider(), map_[i]->getPosition());
	}
	graphics.setRenderColour(RAY_COLOUR);
	graphics.renderRay(util::coord2DToSDLPoint(r.origin), r.dir.x, r.dir.y, MAX_RAY_LENGTH);
	graphics.setRenderColour(HIT_POINT_COLOUR);
	graphics.renderPoints(intersections, HIT_POINT_SIZE);
}
bool ExampleRays::_find_closest_isect(ctp::Ray testRay,
	std::size_t& out_closest_ind, ctp::gFloat& out_near, ctp::Coord2& out_norm_near, ctp::gFloat& out_far, ctp::Coord2& out_norm_far) const {
	ctp::gFloat closest(-1), testNear, testFar;
	ctp::Coord2 testNormNear, testNormFar;
	for (std::size_t i = 0; i < map_.size(); ++i) {
		if (ctp::intersects(testRay, map_[i]->getCollider(), map_[i]->getPosition(), testNear, testNormNear, testFar, testNormFar)) {
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
	const ctp::Ray& r(rotating_ray_.getRay());
	std::size_t ind;
	ctp::gFloat near, far;
	ctp::Coord2 unused1, unused2;
	bool isCollision = _find_closest_isect(r, ind, near, unused1, far, unused2);
	// Draw results.
	for (std::size_t i = 0; i < map_.size(); ++i) {
		if (ind == i)
			graphics.setRenderColour(Example::HIT_SHAPE_COLOUR);
		else
			graphics.setRenderColour(Example::SHAPE_COLOUR);
		graphics.renderShape(map_[i]->getCollider(), map_[i]->getPosition());
	}
	graphics.setRenderColour(RAY_COLOUR);
	graphics.renderRay(util::coord2DToSDLPoint(r.origin), r.dir.x, r.dir.y, isCollision ? static_cast<Uint16>(near) : MAX_RAY_LENGTH);
	if (isCollision) {
		graphics.setRenderColour(HIT_POINT_COLOUR);
		graphics.renderPoint(util::coord2DToSDLPoint(r.origin + r.dir * near), HIT_POINT_SIZE);
	}
}
bool ExampleRays::_find_reflection(ctp::Ray testRay, std::size_t& out_ind, ctp::gFloat& out_reflect_dist, ctp::Ray& out_reflected) const {
	ctp::gFloat near, far;
	ctp::Coord2 norm_near, norm_far;
	if (!_find_closest_isect(testRay, out_ind, near, norm_near, far, norm_far))
		return false;
	if (near == 0.0f) { // Check if inside a shape.
		near = far; // Use the exit point.
		norm_near = -norm_far; // Reflect inside the shape.
	}
	const ctp::Coord2 reflectedPos(testRay.origin + testRay.dir * std::clamp(near - 0.1f, 0.0f, near));
	out_reflected = {reflectedPos, ctp::math::reflect(testRay.dir, norm_near)};
	out_reflect_dist = near;
	return true;
}
Colour ExampleRays::_reflect_interp_colour(std::size_t reflectDepth) const {
	// Determine which "segment" of interpolation we're on (the colours to switch between).
	const float interpFactor(reflectDepth * (NUM_REFLECT_COLOURS - 1) / static_cast<float>(MAX_REFLECTIONS));
	const int segment(static_cast<int>(interpFactor));
	Colour first(RAY_REFLECT_COLOURS[segment]);
	Colour second(RAY_REFLECT_COLOURS[segment + 1]);
	const int interp_R = second.r - static_cast<int>(first.r);
	const int interp_G = second.g - static_cast<int>(first.g);
	const int interp_B = second.b - static_cast<int>(first.b);
	const int interp_A = second.a - static_cast<int>(first.a);
	const float interpolation(interpFactor - segment);
	return Colour{
		static_cast<Uint8>(first.r + interp_R * interpolation),
		static_cast<Uint8>(first.g + interp_G * interpolation),
		static_cast<Uint8>(first.b + interp_B * interpolation),
		static_cast<Uint8>(first.a + interp_A * interpolation)
	};
}
void ExampleRays::_draw_reflecting(const Graphics& graphics) const {
	std::vector<std::size_t> indices; // Keep track of shapes that were hit.
	std::vector<SDL_Point> reflectPoints;
	std::size_t numReflects(0), ind;
	ctp::Ray currentRay(rotating_ray_.getRay()), reflectedRay;
	ctp::gFloat reflectDist;
	while (numReflects < MAX_REFLECTIONS && _find_reflection(currentRay, ind, reflectDist, reflectedRay)) {
		graphics.setRenderColour(_reflect_interp_colour(numReflects));
		graphics.renderRay(util::coord2DToSDLPoint(currentRay.origin), currentRay.dir.x, currentRay.dir.y, static_cast<Uint16>(reflectDist));

		indices.push_back(ind);
		reflectPoints.push_back(util::coord2DToSDLPoint(currentRay.origin + currentRay.dir * reflectDist));
		currentRay = reflectedRay;
		++numReflects;
	}
	if (numReflects < MAX_REFLECTIONS) { // Draw final ray, if necessary.
		graphics.setRenderColour(_reflect_interp_colour(numReflects));
		graphics.renderRay(util::coord2DToSDLPoint(currentRay.origin), currentRay.dir.x, currentRay.dir.y, MAX_RAY_LENGTH);
	}
	for (std::size_t i = 0; i < map_.size(); ++i) {
		if (std::find(indices.begin(), indices.end(), i) != indices.end())
			graphics.setRenderColour(Example::HIT_SHAPE_COLOUR);
		else
			graphics.setRenderColour(Example::SHAPE_COLOUR);
		graphics.renderShape(map_[i]->getCollider(), map_[i]->getPosition());
	}
	graphics.setRenderColour(HIT_POINT_COLOUR);
	graphics.renderPoints(reflectPoints, HIT_POINT_SIZE);
}
void ExampleRays::draw(const Graphics& graphics) {
	// Draw ray's origin.
	graphics.setRenderColour(RAY_ORIGIN_COLOUR);
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
