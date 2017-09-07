#include "catch.hpp"

#include <vector>

#include "../Geometry/IntersectionMath.h"
#include "../Units.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/LineSegment.h"
#include "../Geometry/Ray.h"
#include "../Geometry/Polygon.h"

using namespace units;

TEST_CASE("Rectangle and coordinate intersections.", "[rect][coord]") {
	SECTION("A 0-sized rectangle is a coordinate, and should intersect with one.") {
		Rectangle r(0, 0, 0, 0);
		Coordinate2D c(0, 0);
		CHECK(isect::intersects(r, c));
		r = Rectangle(1, 1, 0, 0);
		c = Coordinate2D(1, 1);
		CHECK(isect::intersects(r, c));
		r = Rectangle(-1, -2, 0, 0);
		c = Coordinate2D(-1, -2);
		CHECK(isect::intersects(r, c));
		r = Rectangle(10.5f, -20.3f, 0, 0);
		c = Coordinate2D(10.5f, -20.3f);
		CHECK(isect::intersects(r, c));
	}
	SECTION("A 0-sized rectangle and a different coordinate.") {
		Rectangle r(0, 0, 0, 0);
		Coordinate2D c(1, 0);
		CHECK_FALSE(isect::intersects(r, c));
		r = Rectangle(1, 1, 0, 0);
		c = Coordinate2D(1, 0);
		CHECK_FALSE(isect::intersects(r, c));
		r = Rectangle(-1, 0, 0, 0);
		c = Coordinate2D(1, 0);
		CHECK_FALSE(isect::intersects(r, c));
		r = Rectangle(1, 0, 0, 0);
		c = Coordinate2D(1, 0.5f);
		CHECK_FALSE(isect::intersects(r, c));
		r = Rectangle(10, 0, 0, 0);
		c = Coordinate2D(0, 0);
		CHECK_FALSE(isect::intersects(r, c));
	}
	SECTION("Coordinates inside a rectangle.") {
		Rectangle r(-5, -2, 8, 7.5f);
		Coordinate2D c(0, 0);
		CHECK(isect::intersects(r, c));
		c = Coordinate2D(r.left(), r.top());
		CHECK(isect::intersects(r, c));
		c = Coordinate2D(r.left(), r.bottom());
		CHECK(isect::intersects(r, c));
		c = Coordinate2D(r.right(), r.top());
		CHECK(isect::intersects(r, c));
		c = Coordinate2D(r.right(), r.bottom());
		CHECK(isect::intersects(r, c));
		c = Coordinate2D(r.left(), 0);
		CHECK(isect::intersects(r, c));
		c = Coordinate2D(r.right(), 0);
		CHECK(isect::intersects(r, c));
		c = Coordinate2D(0, r.top());
		CHECK(isect::intersects(r, c));
		c = Coordinate2D(0, r.bottom());
		CHECK(isect::intersects(r, c));
		c = Coordinate2D(-1, 2);
		CHECK(isect::intersects(r, c));
	}
	SECTION("Coordinates outside a rectangle.") {
		Rectangle r(-5, -2, 8, 7.5f);
		Coordinate2D c(r.left(), r.top() - 0.5f);
		CHECK_FALSE(isect::intersects(r, c));
		c = Coordinate2D(r.left() - 100, 0);
		CHECK_FALSE(isect::intersects(r, c));
		c = Coordinate2D(r.left() - 0.5f, 0);
		CHECK_FALSE(isect::intersects(r, c));
		c = Coordinate2D(r.right() + 0.5f, 0);
		CHECK_FALSE(isect::intersects(r, c));
		c = Coordinate2D(0, r.top() - 0.5f);
		CHECK_FALSE(isect::intersects(r, c));
		c = Coordinate2D(0, r.bottom() + 0.5f);
		CHECK_FALSE(isect::intersects(r, c));
	}
}

TEST_CASE("Line segment and coordinate intersections.", "[lineseg][coord]") {
	SECTION("A 0-length line segment is a coordinate, and should intersect with one.") {
		LineSegment s(0, 0, 0, 0);
		Coordinate2D c(0, 0);
		CHECK(isect::intersects(s, c));
		s = LineSegment(1, 2, 1, 2);
		c = Coordinate2D(1, 2);
		CHECK(isect::intersects(s, c));
		s = LineSegment(-1, -2, -1, -2);
		c = Coordinate2D(-1, -2);
		CHECK(isect::intersects(s, c));
		s = LineSegment(10.5f, -20.3f, 10.5f, -20.3f);
		c = Coordinate2D(10.5f, -20.3f);
		CHECK(isect::intersects(s, c));
	}
	SECTION("A 0-length line segment and a different coordinate.") {
		LineSegment s(0, 0, 0, 0);
		Coordinate2D c(1, 0);
		CHECK_FALSE(isect::intersects(s, c));
		s = LineSegment(1, 1, 1, 1);
		c = Coordinate2D(1, 0);
		CHECK_FALSE(isect::intersects(s, c));
		s = LineSegment(-1, 0, -1, 0);
		c = Coordinate2D(1, 0);
		CHECK_FALSE(isect::intersects(s, c));
		s = LineSegment(1, 0, 1, 0);
		c = Coordinate2D(1, 0.5f);
		CHECK_FALSE(isect::intersects(s, c));
		s = LineSegment(10, 0, 10, 0);
		c = Coordinate2D(0, 0);
		CHECK_FALSE(isect::intersects(s, c));
	}
	SECTION("Coordinates on a line segment.") {
		SECTION("Coordinates on diagonal line segments.") {
			LineSegment s(0, 0, 1, 1);
			Coordinate2D c(0, 0);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(1, 1);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(0.5f, 0.5f);
			CHECK(isect::intersects(s, c));
			s = LineSegment(2, 0, -10, 6);
			c = Coordinate2D(2, 0);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(-10, 6);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(-4, 3);
			CHECK(isect::intersects(s, c));
		}
		SECTION("Coordinates on vertical line segments.") {
			LineSegment s(0, 0, 0, 1);
			Coordinate2D c(0, 0);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(0, 1);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(0, 0.5f);
			CHECK(isect::intersects(s, c));
			s = LineSegment(10, -1, 10, -5);
			c = Coordinate2D(10, -1);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(10, -5);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(10, -3);
			CHECK(isect::intersects(s, c));
		}
		SECTION("Coordinates on horizontal line segments.") {
			LineSegment s(0, 0, 1, 0);
			Coordinate2D c(0, 0);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(1, 0);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(0.5f, 0);
			CHECK(isect::intersects(s, c));
			s = LineSegment(-1, -10, -5, -10);
			c = Coordinate2D(-1, -10);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(-5, -10);
			CHECK(isect::intersects(s, c));
			c = Coordinate2D(-3, -10);
			CHECK(isect::intersects(s, c));
		}
	}
	SECTION("Coordinates off a line segment.") {
		SECTION("Coordinates off diagonal line segments.") {
			LineSegment s(0, 0, 1, 1);
			Coordinate2D c(0, 1);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(1, 0);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(2, 2);
			CHECK_FALSE(isect::intersects(s, c));
			s = LineSegment(-1, -1, 10, 10);
			c = Coordinate2D(10.5f, 10.5f);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(-1, -1.5f);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(100, 0);
			CHECK_FALSE(isect::intersects(s, c));
		}
		SECTION("Coordinates off vertical line segments.") {
			LineSegment s(0, 0, 0, 1);
			Coordinate2D c(1, 0);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(-1, 1);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(0, 2);
			CHECK_FALSE(isect::intersects(s, c));
			s = LineSegment(10, -1, 10, -5);
			c = Coordinate2D(10, 1);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(10, -6);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(9, -3);
			CHECK_FALSE(isect::intersects(s, c));
		}
		SECTION("Coordinates off horizontal line segments.") {
			LineSegment s(0, 0, 1, 0);
			Coordinate2D c(0, 1);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(1, -1);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(0.5f, 0.5f);
			CHECK_FALSE(isect::intersects(s, c));
			s = LineSegment(-1, -10, -5, -10);
			c = Coordinate2D(0, -10);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(-6, -10);
			CHECK_FALSE(isect::intersects(s, c));
			c = Coordinate2D(-3, -9.5f);
			CHECK_FALSE(isect::intersects(s, c));
		}
	}
}

TEST_CASE("Ray and coordinate intersections.", "[ray][coord]") {
	SECTION("Coordinates on a diagonal ray.") {
		SECTION("Diagonal ray from origin.") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
			Coordinate2D c(0, 0);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(1, 1);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(100000.5f, 100000.5f);
			CHECK(isect::intersects(r, c));
		}
		SECTION("Diagonal ray not from origin.") {
			Ray r(Coordinate2D(10, 5), Coordinate2D(-1, -0.5f).normalize());
			Coordinate2D c(10, 5);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(9, 4.5f);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(0, 0);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(-60, -30);
			CHECK(isect::intersects(r, c));
		}
	}
	SECTION("Coordinates on a vertical ray.") {
		SECTION("Down ray from origin.") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1));
			Coordinate2D c(0, 0);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(0, 1);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(0, 10000000);
			CHECK(isect::intersects(r, c));
		}
		SECTION("Up ray not from origin.") {
			Ray r(Coordinate2D(10, 2), Coordinate2D(0, -1));
			Coordinate2D c(10, 2);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(10, 1);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(10, -1000000);
			CHECK(isect::intersects(r, c));
		}
	}
	SECTION("Coordinates on a horizontal ray.") {
		SECTION("Right ray from origin.") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0));
			Coordinate2D c(0, 0);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(1, 0);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(10000000, 0);
			CHECK(isect::intersects(r, c));
		}
		SECTION("Left ray not from origin.") {
			Ray r(Coordinate2D(12, 4), Coordinate2D(-1, 0));
			Coordinate2D c(12, 4);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(11, 4);
			CHECK(isect::intersects(r, c));
			c = Coordinate2D(-10000000, 4);
			CHECK(isect::intersects(r, c));
		}
	}
	SECTION("Coordinates off a diagonal ray.") {
		SECTION("Diagonal ray from origin.") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
			Coordinate2D c(1, 0);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(0, 1);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(100000, 100000.5f);
			CHECK_FALSE(isect::intersects(r, c));
		}
		SECTION("Diagonal ray not from origin.") {
			Ray r(Coordinate2D(10, 5), Coordinate2D(-1, -0.5f).normalize());
			Coordinate2D c(12, 6);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(9, 5);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(9, 3);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(-60.5f, -30);
			CHECK_FALSE(isect::intersects(r, c));
		}
	}
	SECTION("Coordinates off a vertical ray.") {
		SECTION("Down ray from origin.") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1));
			Coordinate2D c(1, 0);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(0, -0.5f);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(0.5f, 10000000);
			CHECK_FALSE(isect::intersects(r, c));
		}
		SECTION("Up ray not from origin.") {
			Ray r(Coordinate2D(10, 2), Coordinate2D(0, -1));
			Coordinate2D c(10, 2.5f);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(10.5f, 1);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(9.5f, -1000000);
			CHECK_FALSE(isect::intersects(r, c));
		}
	}
	SECTION("Coordinates off a horizontal ray.") {
		SECTION("Right ray from origin.") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0));
			Coordinate2D c(-0.5f, 0);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(1, 0.5f);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(10000000, -0.5f);
			CHECK_FALSE(isect::intersects(r, c));
		}
		SECTION("Left ray not from origin.") {
			Ray r(Coordinate2D(12, 4), Coordinate2D(-1, 0));
			Coordinate2D c(12.5f, 4);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(12, 3.5f);
			CHECK_FALSE(isect::intersects(r, c));
			c = Coordinate2D(-10000000, -4);
			CHECK_FALSE(isect::intersects(r, c));
		}
	}
}

TEST_CASE("Line segment intersections.", "[lineseg]") {
	SECTION("Intersecting line segments.") {
		SECTION("Zero-length line segments.") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(0, 0, 0, 0);
			CHECK(isect::intersects(s1, s2));
			s2 = LineSegment(0, 0, 1, 1);
			CHECK(isect::intersects(s1, s2));
		}
		SECTION("Diagonal line segments.") {
			LineSegment s1(1, 1, 0, 0);
			LineSegment s2(1, 1, 0, 0);
			CHECK(isect::intersects(s1, s2));
			s2 = LineSegment(0, 0, 1, 1);
			CHECK(isect::intersects(s1, s2));
			s1 = LineSegment(-1, -1, 0, 0);
			CHECK(isect::intersects(s1, s2));
			s1 = LineSegment(0, 1, 1, 0);
			CHECK(isect::intersects(s1, s2));
		}
		SECTION("Horizontal and vertical line segments.") {
			LineSegment v(0, -10, 0, 10);
			LineSegment s(0, -10, 0, 10);
			CHECK(isect::intersects(v, s));
			s = LineSegment(0, 10, 0, 20);
			CHECK(isect::intersects(v, s));
			s = LineSegment(0, -10, 0, -20);
			CHECK(isect::intersects(v, s));
			s = LineSegment(-50, -10, 0, -10);
			CHECK(isect::intersects(v, s));
			s = LineSegment(-50, -10, 50, -10);
			CHECK(isect::intersects(v, s));
			s = LineSegment(-50, 10, 0, 10);
			CHECK(isect::intersects(v, s));
			s = LineSegment(-50, 10, 50, 10);
			CHECK(isect::intersects(v, s));
			s = LineSegment(-50, 5, 0, 5);
			CHECK(isect::intersects(v, s));
			s = LineSegment(-50, 5, 50, 5);
			CHECK(isect::intersects(v, s));
			s = LineSegment(-10, 10, 10, -10);
			CHECK(isect::intersects(v, s));
			v = LineSegment(30.5f, 20, 30.5f, 10); // v is now horizontal.
			s = LineSegment(30.5f, 9, 30.5f, 11);
			CHECK(isect::intersects(v, s));
			s = LineSegment(30.5f, 10, 30.5f, 20);
			CHECK(isect::intersects(v, s));
			s = LineSegment(30.5f, -10, 30.5f, 20);
			CHECK(isect::intersects(v, s));
			s = LineSegment(0, 20, 40, 10);
			CHECK(isect::intersects(v, s));
		}
	}
	SECTION("Non-intersecting line segments.") {
		SECTION("Zero-length line segments.") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(1, 0, 1, 0);
			CHECK_FALSE(isect::intersects(s1, s2));
			s2 = LineSegment(1, 1, 1, 1);
			CHECK_FALSE(isect::intersects(s1, s2));
		}
		SECTION("Diagonal line segments.") {
			LineSegment s1(1, 1, 0, 0);
			LineSegment s2(1, 2, 0, 1);
			CHECK_FALSE(isect::intersects(s1, s2));
			s2 = LineSegment(0, -0.5f, 1, 0.5f);
			CHECK_FALSE(isect::intersects(s1, s2));
			s2 = LineSegment(2, 2, 1.5f, 1.5f);
			CHECK_FALSE(isect::intersects(s1, s2));
			s1 = LineSegment(-1.5f, -1, 0.5f, 0);
			CHECK_FALSE(isect::intersects(s1, s2));
			s2 = LineSegment(0, -0.5f, 1.5f, 0.5f);
			CHECK_FALSE(isect::intersects(s1, s2));
			s2 = LineSegment(10, 0, 11, 1);
			CHECK_FALSE(isect::intersects(s1, s2));
		}
		SECTION("Horizontal and vertical line segments.") {
			LineSegment v(0, -10, 0, 10);
			LineSegment s(0, -11, 0, -12);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(0, 11, 0, 20);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(0.5f, -10, 0.5f, -20);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(-50, -10, -0.5f, -10);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(-50, -10.5f, 50, -10.5f);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(-50, 10.5f, 0, 10.5f);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(-50, 10.5f, 50, 10.5f);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(0.5f, 5, 50, 5);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(-10, 10, 10, -50);
			CHECK_FALSE(isect::intersects(v, s));
			v = LineSegment(30.5f, 20, 30.5f, 10); // v is now horizontal.
			s = LineSegment(30.5f, 9, 30.5f, 9.5f);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(30.5f, 20.5f, 30.5f, 25);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(30, -10, 30, 20);
			CHECK_FALSE(isect::intersects(v, s));
			s = LineSegment(0, 20, 40, -10);
			CHECK_FALSE(isect::intersects(v, s));
		}
	}
}

TEST_CASE("Line segment intersections with coordinate of intersection output.", "[lineseg]") {
	Coordinate2D out_p;
	SECTION("Intersecting line segments.") {
		SECTION("Zero-length line segments.") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(0, 0, 0, 0);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
			s2 = LineSegment(0, 0, 1, 1);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
		}
		SECTION("Diagonal line segments.") {
			LineSegment s1(1, 1, 0, 0);
			LineSegment s2(1, 1, 0, 0);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == Approx(1).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(1).margin(constants::EPSILON));
			s2 = LineSegment(0, 0, 1, 1);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == Approx(1).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(1).margin(constants::EPSILON));
			s1 = LineSegment(-1, -1, 0, 0);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
			s1 = LineSegment(0, 1, 1, 0);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == Approx(0.5f).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(0.5f).margin(constants::EPSILON));
		}
		SECTION("Horizontal and vertical line segments.") {
			LineSegment v(0, -10, 0, 10);
			LineSegment s(0, -10, 0, 10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
			s = LineSegment(0, 10, 0, 20);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(10).margin(constants::EPSILON));
			s = LineSegment(0, -10, 0, -20);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
			s = LineSegment(-50, -10, 0, -10);
			REQUIRE(isect::intersects(v, s));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
			s = LineSegment(-50, -10, 50, -10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
			s = LineSegment(-50, 10, 0, 10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(10).margin(constants::EPSILON));
			s = LineSegment(-50, 10, 50, 10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(10).margin(constants::EPSILON));
			s = LineSegment(-50, 5, 0, 5);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(5).margin(constants::EPSILON));
			s = LineSegment(-50, 5, 50, 5);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(5).margin(constants::EPSILON));
			s = LineSegment(-10, 10, 10, -10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
			v = LineSegment(30.5f, 20, 30.5f, 10); // v is now horizontal.
			s = LineSegment(30.5f, 9, 30.5f, 11);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(30.5f).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(11).margin(constants::EPSILON));
			s = LineSegment(30.5f, 10, 30.5f, 20);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(30.5f).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(20).margin(constants::EPSILON));
			s = LineSegment(30.5f, -10, 30.5f, 20);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(30.5f).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(20).margin(constants::EPSILON));
			s = LineSegment(0, 20, 40, 10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == Approx(30.5f).margin(constants::EPSILON));
			CHECK(out_p.y == Approx(12.375f).margin(constants::EPSILON));
		}
	}
	SECTION("Non-intersecting line segments.") {
		SECTION("Zero-length line segments.") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(1, 0, 1, 0);
			CHECK_FALSE(isect::intersects(s1, s2, out_p));
			s2 = LineSegment(1, 1, 1, 1);
			CHECK_FALSE(isect::intersects(s1, s2, out_p));
		}
		SECTION("Diagonal line segments.") {
			LineSegment s1(1, 1, 0, 0);
			LineSegment s2(1, 2, 0, 1);
			CHECK_FALSE(isect::intersects(s1, s2, out_p));
			s2 = LineSegment(0, -0.5f, 1, 0.5f);
			CHECK_FALSE(isect::intersects(s1, s2, out_p));
			s2 = LineSegment(2, 2, 1.5f, 1.5f);
			CHECK_FALSE(isect::intersects(s1, s2, out_p));
			s1 = LineSegment(-1.5f, -1, 0.5f, 0);
			CHECK_FALSE(isect::intersects(s1, s2, out_p));
			s2 = LineSegment(0, -0.5f, 1.5f, 0.5f);
			CHECK_FALSE(isect::intersects(s1, s2, out_p));
			s2 = LineSegment(10, 0, 11, 1);
			CHECK_FALSE(isect::intersects(s1, s2, out_p));
		}
		SECTION("Horizontal and vertical line segments.") {
			LineSegment v(0, -10, 0, 10);
			LineSegment s(0, -11, 0, -12);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(0, 11, 0, 20);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(0.5f, -10, 0.5f, -20);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(-50, -10, -0.5f, -10);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(-50, -10.5f, 50, -10.5f);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(-50, 10.5f, 0, 10.5f);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(-50, 10.5f, 50, 10.5f);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(0.5f, 5, 50, 5);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(-10, 10, 10, -50);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			v = LineSegment(30.5f, 20, 30.5f, 10); // v is now horizontal.
			s = LineSegment(30.5f, 9, 30.5f, 9.5f);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(30.5f, 20.5f, 30.5f, 25);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(30, -10, 30, 20);
			CHECK_FALSE(isect::intersects(v, s, out_p));
			s = LineSegment(0, 20, 40, -10);
			CHECK_FALSE(isect::intersects(v, s, out_p));
		}
	}
}

TEST_CASE("Ray and line segment intersections with coordinate of intersection output.", "[ray][lineseg]") {
	Coordinate2D out_p;
	SECTION("Ray and a point.") {
		Ray r(Coordinate2D(0, 0), Coordinate2D(1, 2).normalize());
		LineSegment s(1, 2, 1, 2);
		REQUIRE(isect::intersects(r, s, out_p));
		CHECK(out_p.x == Approx(1).margin(constants::EPSILON));
		CHECK(out_p.y == Approx(2).margin(constants::EPSILON));
		s = LineSegment(0, 0, 0, 0);
		REQUIRE(isect::intersects(r, s, out_p));
		CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
		CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
		s = LineSegment(1, 1.5f, 1, 1.5f);
		CHECK_FALSE(isect::intersects(r, s, out_p));
	}
	SECTION("Parallel line segments and rays.") {
		SECTION("Horizontal line segments and rays.") {
			SECTION("Right ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0).normalize());
				LineSegment s(0, 0, -1, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(-0.1f, 0, -10, 0);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(5, 0, 1, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(1).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(50, 0, 100, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(50).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(50, 0.1f, 100, 0.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coordinate2D(-10, 10), Coordinate2D(-1, 0).normalize());
				LineSegment s(10, 10, -100, 10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(10).margin(constants::EPSILON));
				s = LineSegment(-100, 10, -1000, 10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-100).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(10).margin(constants::EPSILON));
				s = LineSegment(-100, 10.1f, -1000, 10.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
		}
		SECTION("Vertical line segments and rays.") {
			SECTION("Down ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1).normalize());
				LineSegment s(0, 0, 0, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(0, -0.1f, -10, 0);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(0, 5, 0, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(1).margin(constants::EPSILON));
				s = LineSegment(0, 50, 0, 100);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(50).margin(constants::EPSILON));
				s = LineSegment(0.1f, 50, 0.1f, 100);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coordinate2D(10, -10), Coordinate2D(0, -1).normalize());
				LineSegment s(10, 10, 10, -100);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
				s = LineSegment(10, -100, 10, -1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-100).margin(constants::EPSILON));
				s = LineSegment(10.1f, -100, 10.1f, -1000);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
		}
		SECTION("Diagonal line segments and rays.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
				LineSegment s(0, 0, -1, -1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(-0.1f, -0.1f, -10, -10);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(5, 5, 1, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(1).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(1).margin(constants::EPSILON));
				s = LineSegment(50, 50, 100, 100);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(50).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(50).margin(constants::EPSILON));
				s = LineSegment(50.1f, 50, 100.1f, 100);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coordinate2D(10, 10), Coordinate2D(-1, 2).normalize());
				LineSegment s(15, 0, 0, 30);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(10).margin(constants::EPSILON));
				s = LineSegment(0, 30, -35, 100);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(30).margin(constants::EPSILON));
				s = LineSegment(0.1f, 30, -34.9, 100);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
		}
	}
	SECTION("Perpendicular line segments and rays.") {
		SECTION("Horizontal ray.") {
			SECTION("Rightwards ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0).normalize());
				LineSegment s(0, 0, 0, -1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(0, 10, 0, -10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(-0.1f, 10, -0.1f, 10);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(1000, 10, 1000, -10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(1000).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));;
			}
			SECTION("Leftwards ray not from origin.") {
				Ray r = Ray(Coordinate2D(-10, -10), Coordinate2D(-1, 0).normalize());
				LineSegment s = LineSegment(-10, -10, -10, 10);
				Coordinate2D out_p;
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
				s = LineSegment(-9.9f, -10, -9.9f, 10);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(-1000, 10, -1000, -20);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-1000).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1).normalize());
				LineSegment s(0, 0, -1, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(10, 0, -10, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(10, -0.1f, 10, -0.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(10, 1000, -10, 1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(1000).margin(constants::EPSILON));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coordinate2D(-10, -10), Coordinate2D(0, -1).normalize());
				LineSegment s(10, -10, -10, -10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
				s = LineSegment(10, -9.9f, -10, -9.9f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(10, -1000, -10, -1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-1000).margin(constants::EPSILON));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
				LineSegment s(0, 0, -5, 5);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(5, -5, -5, 5);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(5, -5, 0.1f, -0.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(-5, 4.9f, 5, 5.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(0, 1000, 1000, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(500).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(500).margin(constants::EPSILON));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coordinate2D(-10, 10), Coordinate2D(1, -2).normalize());
				LineSegment s(-10, -5, 0, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-4).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-2).margin(constants::EPSILON));
				s = LineSegment(-20, 5.1f, 0, 15.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(0, -2000, 4000, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(796).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-1602).margin(constants::EPSILON));
			}
		}
	}
	SECTION("Diagonal line segment and ray intersections.") {
		SECTION("Horizontal ray.") {
			SECTION("Right ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0).normalize());
				LineSegment s(0, -1, 1000, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(500).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(1000, 1, 0, -1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(500).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(0, -4000, 1, 4000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0.5f).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(-2, -4000, 1, 4000);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coordinate2D(10, -10), Coordinate2D(-1, 0).normalize());
				LineSegment s(0, 20, -20, -40);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
				s = LineSegment(-65, 140, 15, -20);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
				s = LineSegment(-64.9f, 140, 15.1f, -20);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1).normalize());
				LineSegment s(-1, 0, 1, 1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(500).margin(constants::EPSILON));
				s = LineSegment(1, 1000, -1, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(500).margin(constants::EPSILON));
				s = LineSegment(-4000, 0, 4000, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0.5f).margin(constants::EPSILON));
				s = LineSegment(-4000, -2, 4000, 1);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coordinate2D(10, -10), Coordinate2D(-1, 0).normalize());
				LineSegment s(20, 0, -40, -20);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
				s = LineSegment(100, -85, -20, 15);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(10).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-10).margin(constants::EPSILON));
				s = LineSegment(140, -64.9f, -20, 15.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
				LineSegment s(-10, 0, 10, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(0).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(0).margin(constants::EPSILON));
				s = LineSegment(800, 0, 800, 1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(800).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(800).margin(constants::EPSILON));
				s = LineSegment(-0.1f, -10, -0.1f, -10);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(-20, 50, 20, 50.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(10, 100, 100, 55);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(70).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(70).margin(constants::EPSILON));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coordinate2D(-10, 10), Coordinate2D(1, -2).normalize());
				LineSegment s(-10, -5, 0, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(-4).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-2).margin(constants::EPSILON));
				s = LineSegment(-20, 5.1f, 0, 15.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(0, -2000, 4000, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == Approx(796).margin(constants::EPSILON));
				CHECK(out_p.y == Approx(-1602).margin(constants::EPSILON));
			}
		}
	}
}

TEST_CASE("Rectangle line segment intersections.", "[rect][lineseg]") {
	Rectangle r(5, 5, 2, 2);
	LineSegment s(5, 5, 5, 5);
	CHECK(isect::intersects(r, s));
	s = LineSegment(6, 6, 6, 6);
	CHECK(isect::intersects(r, s));
	s = LineSegment(7, 7, 7, 7);
	CHECK(isect::intersects(r, s));
	s = LineSegment(0, 0, 5, 5);
	CHECK(isect::intersects(r, s));
	s = LineSegment(7, 6, 8, 8);
	CHECK(isect::intersects(r, s));
	s = LineSegment(6, 5.5f, 6, 6.5f);
	CHECK(isect::intersects(r, s));
	s = LineSegment(4, 6, 6, 6);
	CHECK(isect::intersects(r, s));
	s = LineSegment(4, 6, 8, 6);
	CHECK(isect::intersects(r, s));
	s = LineSegment(4, 5, 8, 6);
	CHECK(isect::intersects(r, s));
	s = LineSegment(6, 6, 9, 10);
	CHECK(isect::intersects(r, s));
	s = LineSegment(4, 8, 8, 4);
	CHECK(isect::intersects(r, s));
	s = LineSegment(6, 6, 6, 8);
	CHECK(isect::intersects(r, s));
	s = LineSegment(6, 6, 6, -8);
	CHECK(isect::intersects(r, s));
	s = LineSegment(6, 6, 8, 6);
	CHECK(isect::intersects(r, s));
	s = LineSegment(6, 6, -8, 6);
	CHECK(isect::intersects(r, s));
	s = LineSegment(7.1f, 6, 8, 6);
	CHECK_FALSE(isect::intersects(r, s));
	s = LineSegment(4.9f, -7, 4.9f, 10);
	CHECK_FALSE(isect::intersects(r, s));
	s = LineSegment(6, 7.1f, 8, 8);
	CHECK_FALSE(isect::intersects(r, s));
	s = LineSegment(7.1f, 7, 8, 6);
	CHECK_FALSE(isect::intersects(r, s));
	s = LineSegment(6, 4.9f, 6, 4.9f);
	CHECK_FALSE(isect::intersects(r, s));
}

TEST_CASE("Rectangle intersections.", "[rect]") {
	Rectangle r(0, 0, 1, 1);
	Rectangle o(0, 0, 1, 1);
	CHECK(isect::intersects(r, o));
	o = Rectangle(-1, 0, 1, 1);
	CHECK_FALSE(isect::intersects(r, o));
	o = Rectangle(-10, 0, 10, 1);
	CHECK_FALSE(isect::intersects(r, o));
	o = Rectangle(-10, 0, 10.1f, 1);
	CHECK(isect::intersects(r, o));
	o = Rectangle(0.9f, -0.9f, 1, 1);
	CHECK(isect::intersects(r, o));
	o = Rectangle(0.5f, -1, 1, 1);
	CHECK_FALSE(isect::intersects(r, o));
	o = Rectangle(0.5f, 0.5f, 0.25f, 0.25f);
	CHECK(isect::intersects(r, o));
	r = Rectangle(-10, -10, 5, 5);
	CHECK_FALSE(isect::intersects(r, o));
	o = Rectangle(-10, -5, 1, 1);
	CHECK_FALSE(isect::intersects(r, o));
	o = Rectangle(-10, -5.1f, 1, 1);
	CHECK(isect::intersects(r, o));
}

SCENARIO("Testing two polygons for intersection.", "[poly]") {
	GIVEN("A triangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1,1), Coordinate2D(1,0) };
		Polygon p(points);
		GIVEN("A second triangle.") {
			std::vector<Coordinate2D> points2 = { Coordinate2D(-1, -2), Coordinate2D(1, 0), Coordinate2D(3, -1) };
			Polygon o(points2);
			THEN("They don't intersect until we move them together.") {
				CHECK_FALSE(isect::intersects(p, o));
				o.translate(0, 1);
				CHECK(isect::intersects(p, o));
				o.translate(1.9f, 1.5f);
				CHECK(isect::intersects(p, o));
				o.translate(0.1f, 0);
				CHECK_FALSE(isect::intersects(p, o));
			}
		}
		GIVEN("An identical triangle.") {
			Polygon o(points);
			THEN("They intersect.")
				CHECK(isect::intersects(p, o));
		}
		GIVEN("A triangle inside the first one.") {
			for (std::size_t i = 0; i < points.size(); ++i) points[i] *= 0.5f;
			Polygon o(points);
			THEN("They intersect.") {
				CHECK(isect::intersects(p, o));
				o.translate(0.5f, 0.5f);
				CHECK(isect::intersects(p, o));
			}
		}
		GIVEN("A triangle only sharing an edge.") {
			std::vector<Coordinate2D> points2 = { Coordinate2D(0,0), Coordinate2D(0,1), Coordinate2D(1,1) };
			Polygon o(points2);
			CHECK_FALSE(isect::intersects(p, o));
		}
	}
	GIVEN("Two large triangles.") {
		std::vector<Coordinate2D> points3 = { Coordinate2D(0,0), Coordinate2D(1000000,1000000), Coordinate2D(1000000,0) };
		Polygon q(points3);
		std::vector<Coordinate2D> points4 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 0), Coordinate2D(3000000, -1000000) };
		Polygon r(points4);
		CHECK_FALSE(isect::intersects(q, r));
		std::vector<Coordinate2D> points5 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 1), Coordinate2D(3000000, -1000000) };
		r = Polygon(points5);
		CHECK(isect::intersects(q, r));
	}
	GIVEN("A triangle and a rectangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(0,1), Coordinate2D(1,1) };
		Polygon p(points);
		Polygon o(Rectangle(0, 0, 1, 1).toPoly());
		CHECK(isect::intersects(p, o));
		o.translate(1, 0);
		CHECK_FALSE(isect::intersects(p, o));
		o.translate(-0.5f, 1);
		CHECK_FALSE(isect::intersects(p, o));
		o.translate(0, -0.1f);
		CHECK(isect::intersects(p, o));
		o = Rectangle(0, 0, 2, 2).toPoly();
		CHECK(isect::intersects(p, o));
		o.translate(-0.5f, -0.5f);
		CHECK(isect::intersects(p, o));
		p.translate(0, -1.5f);
		CHECK_FALSE(isect::intersects(p, o));
	}
	GIVEN("Convex polygons.") {
		std::vector<Coordinate2D> arb = { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
		std::vector<Coordinate2D> octagon = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                  Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		GIVEN("An octagon and an arbitrary convex polygon.") {
			Polygon p(arb);
			Polygon o(octagon);
			WHEN("They have a lot of overlap.") {
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
			WHEN("The octogon is nearly out the left side.") {
				o.translate(-1.9f, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
			WHEN("The octogon is nearly out of the right side.") {
				o.translate(4.9f, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
			WHEN("The octogon is only touching the left side.") {
				o.translate(-2, 0);
				THEN("Touching polygons are not intersecting.")
					CHECK_FALSE(isect::intersects(p, o));
			}
			WHEN("The octogon is only touching the right side.") {
				o.translate(5, 0);
				THEN("They are not intersecting.")
					CHECK_FALSE(isect::intersects(p, o));
			}
			WHEN("They are significantly apart.") {
				o.translate(20, 50);
				THEN("They do not intersect.")
					CHECK_FALSE(isect::intersects(p, o));
			}
			WHEN("The octogon is nearly out of the bottom.") {
				o.translate(1, -3.9f);
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
			WHEN("The octogon is only touching the bottom.") {
				o.translate(1, -4);
				THEN("They are not intersecting.")
					CHECK_FALSE(isect::intersects(p, o));
			}
			WHEN("The octogon is edged ouf of the bottom left.") {
				o.translate(-0.66f, -3.9f);
				THEN("They are not intersecting.")
					CHECK_FALSE(isect::intersects(p, o));
			}
		}
		GIVEN("An octogon and a smaller octogon.") {
			Polygon p(octagon);
			for (std::size_t i = 0; i < octagon.size(); ++i) octagon[i] *= 0.5f; // Half-size octagon.
			Polygon o(octagon);
			WHEN("One is completely inside the other.") {
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
			WHEN("One is inside the other, sharing edges.") {
				o.translate(-1, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
		}
	}
}