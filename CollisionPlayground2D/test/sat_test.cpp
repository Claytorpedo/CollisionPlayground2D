#include "catch.hpp"
#include "definitions.h"

#include "../Geometry/sat.h"
#include "../Geometry/Units.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Circle.h"
#include "../Geometry/ShapeContainer.h"

using geom::gFloat;
using geom::Coord2;
using geom::Rect;
using geom::Polygon;
using geom::Circle;

SCENARIO("Finding the separating axes for two shapes.", "[sat]") {
	GIVEN("Two polygons.") {
		GIVEN("An octagon and a triangle.") {
			Polygon p(shapes::octagon);
			Polygon o(shapes::tri);
			WHEN("They get their separating axes.") {
				std::vector<Coord2> axes = geom::sat::getSeparatingAxes(p, o);
				std::size_t expected = p.size() + o.size();
				THEN("It's the sum of the number of axes between them.")
					CHECK(axes.size() == expected);
			}
			WHEN("They get their separating axes in the opposite order.") {
				std::vector<Coord2> axes = geom::sat::getSeparatingAxes(o, p);
				std::size_t expected = p.size() + o.size();
				THEN("It's the sum of the number of axes between them.")
					CHECK(axes.size() == expected);
			}
		}
		GIVEN("Two different triangles.") {
			Polygon p(shapes::rightTri);
			Polygon o(shapes::tri);
			WHEN("They get their separating axes.") {
				std::vector<Coord2> axes = geom::sat::getSeparatingAxes(p, o);
				std::size_t expected = p.size() + o.size();
				THEN("It's the sum of the number of axes between them.")
					CHECK(axes.size() == expected);
			}
			WHEN("They get their separating axes in the opposite order.") {
				std::vector<Coord2> axes = geom::sat::getSeparatingAxes(o, p);
				std::size_t expected = p.size() + o.size();
				THEN("It's the sum of the number of axes between them.")
					CHECK(axes.size() == expected);
			}
		}
	}
	GIVEN("A polygon and a rectangle.") {
		Polygon p(shapes::octagon);
		Rect r(0, 10, 1, 5);
		WHEN("They get their separating axes.") {
			std::vector<Coord2> axes = geom::sat::getSeparatingAxes(p, r);
			std::size_t expected = p.size() + separating_axes::RECT_NUM_AXES;
			THEN("It's the sum of the number of axes between them.")
				CHECK(axes.size() == expected);
		}
		WHEN("They get their separating axes in the opposite order.") {
			std::vector<Coord2> axes = geom::sat::getSeparatingAxes(r, p);
			std::size_t expected = p.size() + separating_axes::RECT_NUM_AXES;
			THEN("It's the sum of the number of axes between them.")
				CHECK(axes.size() == expected);
		}
	}
	GIVEN("Two rectangles.") {
		Rect r1(50, -10, 600, 2);
		Rect r2(0, 10, 1, 5);
		WHEN("They get their separating axes.") {
			std::vector<Coord2> axes = geom::sat::getSeparatingAxes(r1, r2);
			std::size_t expected = separating_axes::RECT_NUM_AXES;
			THEN("There is a special case with only two axes.")
				CHECK(axes.size() == expected);
		}
		WHEN("They get their separating axes in the opposite order.") {
			std::vector<Coord2> axes = geom::sat::getSeparatingAxes(r2, r1);
			std::size_t expected = separating_axes::RECT_NUM_AXES;
			THEN("There is a special case with only two axes.")
				CHECK(axes.size() == expected);
		}
	}
	GIVEN("Two circles.") {
		Circle c1(0, 0, 10);
		Circle c2(10, 10, 1);
		WHEN("They get their separated axis.") {
			std::vector<Coord2> axes = geom::sat::getSeparatingAxes(c1, c2);
			THEN("There is a special case with only one axis.")
				CHECK(axes.size() == 1);
		}
		WHEN("Their centers are the same.") {
			Coord2 c1_pos(10, 10);
			std::vector<Coord2> axes = geom::sat::getSeparatingAxes(c1, c2, c1_pos);
			THEN("There will still be a special case with only one axis.")
				CHECK(axes.size() == 1);
		}
	}
}