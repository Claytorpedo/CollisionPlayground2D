#include "catch.hpp"

#include "../Geometry/IntersectionMath.h"
#include "../Units.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/LineSegment.h"


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
		Coordinate2D c(r.left(), r.top() - 0.5f);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(r.left() - 100, 0);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(r.left() - 0.5f, 0);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(r.right() + 0.5f, 0);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(0, r.top() - 0.5f);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(0, r.bottom() + 0.5f);
		REQUIRE(isect::intersects(r, c) == false);
	}
}

TEST_CASE("Line segment and coordinate intersections", "[lineseg][coord]") {
	SECTION("A 0-length line segment is a coordinate, and should collide with one") {
		LineSegment s(0, 0, 0, 0);
		Coordinate2D c(0, 0);
		REQUIRE(isect::intersects(s, c) == true);
		s = LineSegment(1, 2, 1, 2);
		c = Coordinate2D(1, 2);
		REQUIRE(isect::intersects(s, c) == true);
		s = LineSegment(-1, -2, -1, -2);
		c = Coordinate2D(-1, -2);
		REQUIRE(isect::intersects(s, c) == true);
		s = LineSegment(10.5f, -20.3f, 10.5f, -20.3f);
		c = Coordinate2D(10.5f, -20.3f);
		REQUIRE(isect::intersects(s, c) == true);
	}
	SECTION("A 0-length line segment and a different coordinate") {
		LineSegment s(0, 0, 0, 0);
		Coordinate2D c(1, 0);
		REQUIRE(isect::intersects(s, c) == false);
		s = LineSegment(1, 1, 1, 1);
		c = Coordinate2D(1, 0);
		REQUIRE(isect::intersects(s, c) == false);
		s = LineSegment(-1, 0, -1, 0);
		c = Coordinate2D(1, 0);
		REQUIRE(isect::intersects(s, c) == false);
		s = LineSegment(1, 0, 1, 0);
		c = Coordinate2D(1, 0.5f);
		REQUIRE(isect::intersects(s, c) == false);
		s = LineSegment(10, 0, 10, 0);
		c = Coordinate2D(0, 0);
		REQUIRE(isect::intersects(s, c) == false);
	}
	SECTION("Coordinates on a line segment") {
		SECTION("Coordinates on diagonal line segments") {
			LineSegment s(0, 0, 1, 1);
			Coordinate2D c(0, 0);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(1, 1);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(0.5f, 0.5f);
			REQUIRE(isect::intersects(s, c) == true);
			s = LineSegment(-1, -1, 10, 10);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(-1, -1);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(10, 10);
			REQUIRE(isect::intersects(s, c) == true);
		}
		SECTION("Coordinates on vertical line segments") {
			LineSegment s(0, 0, 0, 1);
			Coordinate2D c(0, 0);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(0, 1);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(0, 0.5f);
			REQUIRE(isect::intersects(s, c) == true);
			s = LineSegment(10, -1, 10, -5);
			c = Coordinate2D(10, -1);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(10, -5);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(10, -3);
			REQUIRE(isect::intersects(s, c) == true);
		}
		SECTION("Coordinates on horizontal line segments") {
			LineSegment s(0, 0, 1, 0);
			Coordinate2D c(0, 0);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(1, 0);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(0.5f, 0);
			REQUIRE(isect::intersects(s, c) == true);
			s = LineSegment(-1, -10, -5, -10);
			c = Coordinate2D(-1, -10);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(-5, -10);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(-3, -10);
			REQUIRE(isect::intersects(s, c) == true);
		}
	}
	SECTION("Coordinates off a line segment") {
		SECTION("Coordinates off diagonal line segments") {
			LineSegment s(0, 0, 1, 1);
			Coordinate2D c(0, 1);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(1, 0);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(2, 2);
			REQUIRE(isect::intersects(s, c) == false);
			s = LineSegment(-1, -1, 10, 10);
			c = Coordinate2D(10.5f, 10.5f);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(-1, -1.5f);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(100, 0);
			REQUIRE(isect::intersects(s, c) == false);
		}
		SECTION("Coordinates off vertical line segments") {
			LineSegment s(0, 0, 0, 1);
			Coordinate2D c(1, 0);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(-1, 1);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(0, 2);
			REQUIRE(isect::intersects(s, c) == false);
			s = LineSegment(10, -1, 10, -5);
			c = Coordinate2D(10, 1);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(10, -6);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(9, -3);
			REQUIRE(isect::intersects(s, c) == false);
		}
		SECTION("Coordinates off horizontal line segments") {
			LineSegment s(0, 0, 1, 0);
			Coordinate2D c(0, 1);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(1, -1);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(0.5f, 0.5f);
			REQUIRE(isect::intersects(s, c) == false);
			s = LineSegment(-1, -10, -5, -10);
			c = Coordinate2D(0, -10);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(-6, -10);
			REQUIRE(isect::intersects(s, c) == false);
			c = Coordinate2D(-3, -9.5f);
			REQUIRE(isect::intersects(s, c) == false);
		}
	}
}
