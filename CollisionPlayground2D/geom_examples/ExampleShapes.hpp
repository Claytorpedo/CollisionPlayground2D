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
		geom::ShapeContainer _gen_example_shape() const;
	};
}

#endif // INCLUDE_GAME_EXAMPLE_SHAPES_HPP
