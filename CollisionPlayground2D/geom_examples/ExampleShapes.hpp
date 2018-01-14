#pragma once
#ifndef INCLUDE_GAME_EXAMPLE_SHAPES_HPP
#define INCLUDE_GAME_EXAMPLE_SHAPES_HPP

#include "Example.hpp"
#include "Mover.hpp"
#include "SimpleCollisionMap.hpp"

#include "../Geometry2D/Geometry.hpp"

namespace game {
	class ExampleShapes : public Example {
	public:

		enum class ExampleType {
			RECT,
			POLY,
			CIRCLE,
			MIXED,
		};

		static const std::size_t  NUM_SHAPES;
		static const geom::gFloat SHAPE_MIN_SIZE;
		static const geom::gFloat SHAPE_MAX_SIZE;
		static const std::size_t  POLY_MIN_VERTS;
		static const std::size_t  POLY_MAX_VERTS;

		ExampleShapes(ExampleType type, const geom::Rect& levelRegion);
		~ExampleShapes() = default;
		virtual void update(const Input& input, const MS elapsedTime);
		virtual void draw(const Graphics& graphics);
		virtual void reset();
	private:
		ExampleType type_;
		Mover mover_;
		SimpleCollisionMap map_;
		geom::Rect level_region_;

		void _init();
		void _gen_mover();
		geom::ShapeContainer _gen_shape() const;
		geom::Rect _gen_rect() const;
		geom::Polygon _gen_poly() const;
		geom::Circle _gen_circle() const;
	};
}

#endif // INCLUDE_GAME_EXAMPLE_SHAPES_HPP
