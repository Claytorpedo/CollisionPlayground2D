#ifndef INCLUDE_GAME_EXAMPLE_RAYS_HPP
#define INCLUDE_GAME_EXAMPLE_RAYS_HPP

#include "Example.hpp"
#include "Mover.hpp"
#include "SimpleCollisionMap.hpp"
#include "RotatingRay.hpp"

#include <Geometry2D/Geometry.hpp>

namespace game {
class ExampleRays : public Example {
public:

	static const std::size_t MAX_REFLECTIONS;
	static const Uint16      MAX_RAY_LENGTH;
	static const Uint8       HIT_POINT_SIZE;
	// Colours.
	static const Colour RAY_COLOUR;
	static const Colour RAY_ORIGIN_COLOUR;
	static const Colour HIT_POINT_COLOUR;
	static const Colour RAY_REFLECT_COLOURS[];
	static const std::size_t NUM_REFLECT_COLOURS;

	enum class ExampleType {
		PEIRCING,
		CLOSEST,
		REFLECTING,
	};

	ExampleRays(ExampleType type, const ctp::Rect& levelRegion);
	~ExampleRays() = default;
	virtual void update(const Input& input, const MS elapsedTime);
	virtual void draw(const Graphics& graphics);
	virtual void reset();
private:
	ExampleType type_;
	SimpleCollisionMap map_;
	ctp::Rect level_region_;
	RotatingRay rotating_ray_;

	void _init();
	bool _find_closest_isect(ctp::Ray testRay,
		std::size_t& out_closest_ind, ctp::gFloat& out_near, ctp::Coord2& out_norm_near, ctp::gFloat& out_far, ctp::Coord2& out_norm_far) const;
	bool _find_reflection(ctp::Ray testRay, std::size_t& out_ind, ctp::gFloat& out_reflect_dist, ctp::Ray& out_reflected) const;
	void _draw_peircing(const Graphics& graphics) const;
	void _draw_closest(const Graphics& graphics) const;
	void _draw_reflecting(const Graphics& graphics) const;
	Colour _reflect_interp_colour(std::size_t reflectDepth) const; // Change the ray's colour while reflecting.
};
}

#endif // INCLUDE_GAME_EXAMPLE_RAYS_HPP
