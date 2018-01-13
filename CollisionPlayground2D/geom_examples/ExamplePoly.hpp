#pragma once
#ifndef INCLUDE_GAME_EXAMPLE_POLY_HPP
#define INCLUDE_GAME_EXAMPLE_POLY_HPP

#include "Example.hpp"
#include "Mover.hpp"
#include "SimpleCollisionMap.hpp"

#include "../Geometry2D/Geometry.hpp"

namespace game {
	class ExamplePoly : public Example {
	public:
		static const std::size_t  NUM_POLYS;
		static const geom::gFloat POLY_MIN_RAD;
		static const geom::gFloat POLY_MAX_RAD;
		static const std::size_t  POLY_MIN_VERTS;
		static const std::size_t  POLY_MAX_VERTS;

		ExamplePoly(const geom::Rect& levelRegion);
		~ExamplePoly() = default;
		virtual void update(Input& input, const MS elapsedTime);
		virtual void draw(Graphics& graphics);
		virtual void reset();
	private:
		Mover mover_;
		SimpleCollisionMap map_;
		geom::Rect level_region_;

		void init_();
		void gen_mover_();
	};
}

#endif // INCLUDE_GAME_EXAMPLE_POLY_HPP