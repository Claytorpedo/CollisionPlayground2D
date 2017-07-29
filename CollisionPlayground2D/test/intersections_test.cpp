#include "catch.hpp"

#include "../Geometry/IntersectionMath.h"
#include "../Units.h"
#include "../Geometry/Rectangle.h"


TEST_CASE("Rrectangle and coordinate intersections", "[rect][coord]") {
	SECTION("A 0-sized rectangle is a coordinate, and should collide with one") {
		Rectangle r(0, 0, 0, 0);
		Coordinate2D c(0, 0);
		REQUIRE(isect::intersects(r, c) == true);
		r = Rectangle(1, 1, 0, 0);
		c = Coordinate2D(1, 1);
		REQUIRE(isect::intersects(r, c) == true);
		r = Rectangle(-1, -2, 0, 0);
		c = Coordinate2D(-1, -2);
		REQUIRE(isect::intersects(r, c) == true);
		r = Rectangle(10.5f, -20.3f, 0, 0);
		c = Coordinate2D(10.5f, -20.3f);
		REQUIRE(isect::intersects(r, c) == true);
	}
	SECTION("A 0-sized rectangle and a different coordinate") {
		Rectangle r(0, 0, 0, 0);
		Coordinate2D c(1, 0);
		REQUIRE(isect::intersects(r, c) == false);
		r = Rectangle(1, 1, 0, 0);
		c = Coordinate2D(1, 0);
		REQUIRE(isect::intersects(r, c) == false);
		r = Rectangle(-1, 0, 0, 0);
		c = Coordinate2D(1, 0);
		REQUIRE(isect::intersects(r, c) == false);
		r = Rectangle(1, 0, 0, 0);
		c = Coordinate2D(1, 0.5f);
		REQUIRE(isect::intersects(r, c) == false);
		r = Rectangle(10, 0, 0, 0);
		c = Coordinate2D(0, 0);
		REQUIRE(isect::intersects(r, c) == false);
	}
	SECTION("Coordinates inside a rectangle") {
		Rectangle r(-5, -2, 8, 7.5f);
		Coordinate2D c(0, 0);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(r.left(), r.top());
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(r.left(), r.bottom());
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(r.right(), r.top());
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(r.right(), r.bottom());
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(r.left(), 0);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(r.right(), 0);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(0, r.top());
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(0, r.bottom());
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(-1, 2);
		REQUIRE(isect::intersects(r, c) == true);
	}
	SECTION("Coordinates outside a rectangle") {
		Rectangle r(-5, -2, 8, 7.5f);
		Coordinate2D c(-5, -2.5f);
		REQUIRE(isect::intersects(r, c) == false);
	}
}

TEST_CASE("Line segment and coordinate intersections", "[lineseg][coord]") {


}
