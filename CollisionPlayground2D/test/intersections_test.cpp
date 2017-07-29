#include "catch.hpp"

#include "../Geometry/IntersectionMath.h"
#include "../Units.h"
#include "../Geometry/Rectangle.h"


TEST_CASE("rectangle and coordinate intersections", "[rect][coord]") {
	SECTION("0-sized rectangles can't intersect coordinates") {
		Rectangle r(0, 0, 0, 0);
		Coordinate2D c(0, 0);
		REQUIRE(isect::intersects(r, c) == false);
	}

}