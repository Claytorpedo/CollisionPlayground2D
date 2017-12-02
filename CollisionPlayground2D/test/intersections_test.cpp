#include "catch.hpp"
#include "definitions.h"

#include <vector>

#include "../Geometry/IntersectionMath.h"
#include "../Geometry/Units.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/LineSegment.h"
#include "../Geometry/Ray.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/ShapeContainer.h"

using namespace units;

TEST_CASE("Rectangle and coordinate intersections.", "[isect][rect][coord]") {
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

TEST_CASE("Line segment and coordinate intersections.", "[isect][lineseg][coord]") {
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

TEST_CASE("Ray and coordinate intersections.", "[isect][ray][coord]") {
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

TEST_CASE("Line segment intersections.", "[isect][lineseg]") {
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

TEST_CASE("Line segment intersections with coordinate of intersection output.", "[isect][lineseg]") {
	Coordinate2D out_p;
	SECTION("Intersecting line segments.") {
		SECTION("Zero-length line segments.") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(0, 0, 0, 0);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(0));
			s2 = LineSegment(0, 0, 1, 1);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(0));
		}
		SECTION("Diagonal line segments.") {
			LineSegment s1(1, 1, 0, 0);
			LineSegment s2(1, 1, 0, 0);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == ApproxEps(1));
			CHECK(out_p.y == ApproxEps(1));
			s2 = LineSegment(0, 0, 1, 1);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == ApproxEps(1));
			CHECK(out_p.y == ApproxEps(1));
			s1 = LineSegment(-1, -1, 0, 0);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(0));
			s1 = LineSegment(0, 1, 1, 0);
			REQUIRE(isect::intersects(s1, s2, out_p));
			CHECK(out_p.x == ApproxEps(0.5f));
			CHECK(out_p.y == ApproxEps(0.5f));
		}
		SECTION("Horizontal and vertical line segments.") {
			LineSegment v(0, -10, 0, 10);
			LineSegment s(0, -10, 0, 10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(-10));
			s = LineSegment(0, 10, 0, 20);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(10));
			s = LineSegment(0, -10, 0, -20);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(-10));
			s = LineSegment(-50, -10, 0, -10);
			REQUIRE(isect::intersects(v, s));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(-10));
			s = LineSegment(-50, -10, 50, -10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(-10));
			s = LineSegment(-50, 10, 0, 10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(10));
			s = LineSegment(-50, 10, 50, 10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(10));
			s = LineSegment(-50, 5, 0, 5);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(5));
			s = LineSegment(-50, 5, 50, 5);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(5));
			s = LineSegment(-10, 10, 10, -10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(0));
			v = LineSegment(30.5f, 20, 30.5f, 10); // v is now horizontal.
			s = LineSegment(30.5f, 9, 30.5f, 11);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(30.5f));
			CHECK(out_p.y == ApproxEps(11));
			s = LineSegment(30.5f, 10, 30.5f, 20);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(30.5f));
			CHECK(out_p.y == ApproxEps(20));
			s = LineSegment(30.5f, -10, 30.5f, 20);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(30.5f));
			CHECK(out_p.y == ApproxEps(20));
			s = LineSegment(0, 20, 40, 10);
			REQUIRE(isect::intersects(v, s, out_p));
			CHECK(out_p.x == ApproxEps(30.5f));
			CHECK(out_p.y == ApproxEps(12.375f));
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

TEST_CASE("Ray and line segment intersections with coordinate of intersection output.", "[isect][ray][lineseg]") {
	Coordinate2D out_p;
	SECTION("Ray and a point.") {
		Ray r(Coordinate2D(0, 0), Coordinate2D(1, 2).normalize());
		LineSegment s(1, 2, 1, 2);
		REQUIRE(isect::intersects(r, s, out_p));
		CHECK(out_p.x == ApproxEps(1));
		CHECK(out_p.y == ApproxEps(2));
		s = LineSegment(0, 0, 0, 0);
		REQUIRE(isect::intersects(r, s, out_p));
		CHECK(out_p.x == ApproxEps(0));
		CHECK(out_p.y == ApproxEps(0));
		s = LineSegment(1, 1.5f, 1, 1.5f);
		CHECK_FALSE(isect::intersects(r, s, out_p));
	}
	SECTION("Parallel line segments and rays.") {
		SECTION("Horizontal line segments and rays.") {
			SECTION("Right ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0).normalize());
				LineSegment s(0, 0, -1, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(-0.1f, 0, -10, 0);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(5, 0, 1, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(1));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(50, 0, 100, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(50));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(50, 0.1f, 100, 0.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coordinate2D(-10, 10), Coordinate2D(-1, 0).normalize());
				LineSegment s(10, 10, -100, 10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-10));
				CHECK(out_p.y == ApproxEps(10));
				s = LineSegment(-100, 10, -1000, 10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-100));
				CHECK(out_p.y == ApproxEps(10));
				s = LineSegment(-100, 10.1f, -1000, 10.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
		}
		SECTION("Vertical line segments and rays.") {
			SECTION("Down ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1).normalize());
				LineSegment s(0, 0, 0, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(0, -0.1f, -10, 0);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(0, 5, 0, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(1));
				s = LineSegment(0, 50, 0, 100);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(50));
				s = LineSegment(0.1f, 50, 0.1f, 100);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coordinate2D(10, -10), Coordinate2D(0, -1).normalize());
				LineSegment s(10, 10, 10, -100);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(10));
				CHECK(out_p.y == ApproxEps(-10));
				s = LineSegment(10, -100, 10, -1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(10));
				CHECK(out_p.y == ApproxEps(-100));
				s = LineSegment(10.1f, -100, 10.1f, -1000);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
		}
		SECTION("Diagonal line segments and rays.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
				LineSegment s(0, 0, -1, -1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(-0.1f, -0.1f, -10, -10);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(5, 5, 1, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(1));
				CHECK(out_p.y == ApproxEps(1));
				s = LineSegment(50, 50, 100, 100);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(50));
				CHECK(out_p.y == ApproxEps(50));
				s = LineSegment(50.1f, 50, 100.1f, 100);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coordinate2D(10, 10), Coordinate2D(-1, 2).normalize());
				LineSegment s(15, 0, 0, 30);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(10));
				CHECK(out_p.y == ApproxEps(10));
				s = LineSegment(0, 30, -35, 100);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(30));
				s = LineSegment(0.1f, 30, -34.9f, 100);
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
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(0, 10, 0, -10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(-0.1f, 10, -0.1f, 10);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(1000, 10, 1000, -10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(1000));
				CHECK(out_p.y == ApproxEps(0));;
			}
			SECTION("Leftwards ray not from origin.") {
				Ray r = Ray(Coordinate2D(-10, -10), Coordinate2D(-1, 0).normalize());
				LineSegment s = LineSegment(-10, -10, -10, 10);
				Coordinate2D out_p;
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-10));
				CHECK(out_p.y == ApproxEps(-10));
				s = LineSegment(-9.9f, -10, -9.9f, 10);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(-1000, 10, -1000, -20);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-1000));
				CHECK(out_p.y == ApproxEps(-10));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1).normalize());
				LineSegment s(0, 0, -1, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(10, 0, -10, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(10, -0.1f, 10, -0.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(10, 1000, -10, 1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(1000));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coordinate2D(-10, -10), Coordinate2D(0, -1).normalize());
				LineSegment s(10, -10, -10, -10);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-10));
				CHECK(out_p.y == ApproxEps(-10));
				s = LineSegment(10, -9.9f, -10, -9.9f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(10, -1000, -10, -1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-10));
				CHECK(out_p.y == ApproxEps(-1000));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
				LineSegment s(0, 0, -5, 5);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(5, -5, -5, 5);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(5, -5, 0.1f, -0.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(-5, 4.9f, 5, 5.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(0, 1000, 1000, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(500));
				CHECK(out_p.y == ApproxEps(500));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coordinate2D(-10, 10), Coordinate2D(1, -2).normalize());
				LineSegment s(-10, -5, 0, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-4));
				CHECK(out_p.y == ApproxEps(-2));
				s = LineSegment(-20, 5.1f, 0, 15.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(0, -2000, 4000, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(796));
				CHECK(out_p.y == ApproxEps(-1602));
			}
		}
	}
	SECTION("Diagonal line segment and ray intersections.") {
		SECTION("Horizontal ray.") {
			SECTION("Right ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0).normalize());
				LineSegment s(0, -1, 1000, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(500));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(1000, 1, 0, -1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(500));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(0, -4000, 1, 4000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0.5f));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(-2, -4000, 1, 4000);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coordinate2D(10, -10), Coordinate2D(-1, 0).normalize());
				LineSegment s(0, 20, -20, -40);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-10));
				CHECK(out_p.y == ApproxEps(-10));
				s = LineSegment(-65, 140, 15, -20);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(10));
				CHECK(out_p.y == ApproxEps(-10));
				s = LineSegment(-64.9f, 140, 15.1f, -20);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1).normalize());
				LineSegment s(-1, 0, 1, 1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(500));
				s = LineSegment(1, 1000, -1, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(500));
				s = LineSegment(-4000, 0, 4000, 1);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0.5f));
				s = LineSegment(-4000, -2, 4000, 1);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coordinate2D(10, -10), Coordinate2D(-1, 0).normalize());
				LineSegment s(20, 0, -40, -20);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-10));
				CHECK(out_p.y == ApproxEps(-10));
				s = LineSegment(100, -85, -20, 15);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(10));
				CHECK(out_p.y == ApproxEps(-10));
				s = LineSegment(140, -64.9f, -20, 15.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
				LineSegment s(-10, 0, 10, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(0));
				CHECK(out_p.y == ApproxEps(0));
				s = LineSegment(800, 0, 800, 1000);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(800));
				CHECK(out_p.y == ApproxEps(800));
				s = LineSegment(-0.1f, -10, -0.1f, -10);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(-20, 50, 20, 50.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(10, 100, 100, 55);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(70));
				CHECK(out_p.y == ApproxEps(70));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coordinate2D(-10, 10), Coordinate2D(1, -2).normalize());
				LineSegment s(-10, -5, 0, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(-4));
				CHECK(out_p.y == ApproxEps(-2));
				s = LineSegment(-20, 5.1f, 0, 15.1f);
				CHECK_FALSE(isect::intersects(r, s, out_p));
				s = LineSegment(0, -2000, 4000, 0);
				REQUIRE(isect::intersects(r, s, out_p));
				CHECK(out_p.x == ApproxEps(796));
				CHECK(out_p.y == ApproxEps(-1602));
			}
		}
	}
}

TEST_CASE("Rectangle line segment intersections.", "[isect][rect][lineseg]") {
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

TEST_CASE("Rectangle intersections.", "[isect][rect]") {
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

SCENARIO("Testing two shapes for intersection.", "[isect][shape]") {
	GIVEN("A triangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1,1), Coordinate2D(1,0) };
		Polygon p(shapes::rightTri);
		GIVEN("A second triangle.") {
			Polygon o(shapes::tri);
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
			Polygon o(shapes::rightTri);
			THEN("They intersect.")
				CHECK(isect::intersects(p, o));
		}
		GIVEN("A triangle inside the first one.") {
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < shapes::rightTri.size(); ++i) inside.push_back(shapes::rightTri[i] * 0.5f);
			Polygon o(inside);
			THEN("They intersect.") {
				CHECK(isect::intersects(p, o));
				o.translate(0.5f, 0.5f);
				CHECK(isect::intersects(p, o));
			}
		}
		GIVEN("A triangle only sharing an edge.") {
			Polygon o(shapes::edgeTri);
			CHECK_FALSE(isect::intersects(p, o));
		}
	}
	GIVEN("Two large triangles.") {
		std::vector<Coordinate2D> points1 = { Coordinate2D(0,0), Coordinate2D(1000000,1000000), Coordinate2D(1000000,0) };
		Polygon q(points1);
		std::vector<Coordinate2D> points2 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 0), Coordinate2D(3000000, -1000000) };
		Polygon r(points2);
		CHECK_FALSE(isect::intersects(q, r));
		std::vector<Coordinate2D> points3 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 1), Coordinate2D(3000000, -1000000) };
		r = Polygon(points3);
		CHECK(isect::intersects(q, r));
	}
	GIVEN("A triangle and a rectangle.") {
		Polygon p(shapes::edgeTriR);
		Rectangle o(0, 0, 1, 1);
		CHECK(isect::intersects(p, o));
		o += Coordinate2D(1, 0);
		CHECK_FALSE(isect::intersects(p, o));
		o += Coordinate2D(-0.5f, 1);
		CHECK_FALSE(isect::intersects(p, o));
		o += Coordinate2D(0, -0.1f);
		CHECK(isect::intersects(p, o));
		o = Rectangle(0, 0, 2, 2);
		CHECK(isect::intersects(p, o));
		o += Coordinate2D(-0.5f, -0.5f);
		CHECK(isect::intersects(p, o));
		p.translate(0, -1.5f);
		CHECK_FALSE(isect::intersects(p, o));
	}
	GIVEN("Convex polygons.") {
		GIVEN("An octagon and an arbitrary convex polygon.") {
			Polygon p(shapes::arb);
			Polygon o(shapes::octagon);
			WHEN("They have a lot of overlap.") {
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
			WHEN("The octagon is nearly out the left side.") {
				o.translate(-1.9f, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
			WHEN("The octagon is nearly out of the right side.") {
				o.translate(4.9f, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
			WHEN("The octagon is only touching the left side.") {
				o.translate(-2, 0);
				THEN("Touching polygons are not intersecting.")
					CHECK_FALSE(isect::intersects(p, o));
			}
			WHEN("The octagon is only touching the right side.") {
				o.translate(5, 0);
				THEN("They are not intersecting.")
					CHECK_FALSE(isect::intersects(p, o));
			}
			WHEN("They are significantly apart.") {
				o.translate(20, 50);
				THEN("They do not intersect.")
					CHECK_FALSE(isect::intersects(p, o));
			}
			WHEN("The octagon is nearly out of the top.") {
				o.translate(1, -3.9f);
				THEN("They intersect.")
					CHECK(isect::intersects(p, o));
			}
			WHEN("The octagon is only touching the top.") {
				o.translate(1, -4);
				THEN("They are not intersecting.")
					CHECK_FALSE(isect::intersects(p, o));
			}
			WHEN("The octagon is edged out of the top left.") {
				o.translate(-0.66f, -3.9f);
				THEN("They are not intersecting.")
					CHECK_FALSE(isect::intersects(p, o));
			}
		}
		GIVEN("An octagon and a smaller octagon.") {
			Polygon p(shapes::octagon);
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < shapes::octagon.size(); ++i) inside.push_back(shapes::octagon[i] * 0.5f); // Half-size octagon.
			Polygon o(inside);
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

SCENARIO("Testing two shapes for intersection with given positions.", "[isect][shape]") {
	GIVEN("A triangle.") {
		Polygon p(shapes::rightTri);
		Coordinate2D pos1(0, 0);
		GIVEN("A second triangle.") {
			Polygon o(shapes::tri);
			Coordinate2D pos2(0, 0);
			THEN("They don't intersect until we move them together.") {
				CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
				pos2 = Coordinate2D(0, 1);
				CHECK(isect::intersects(p, pos1, o, pos2));
				pos2 = Coordinate2D(1.9f, 2.5f);
				CHECK(isect::intersects(p, pos1, o, pos2));
				pos1 = Coordinate2D(-0.1f, 0);
				CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
			}
		}
		GIVEN("An identical triangle.") {
			Polygon o(shapes::rightTri);
			Coordinate2D pos2(0, 0);
			THEN("They intersect.")
				CHECK(isect::intersects(p, pos1, o, pos2));
		}
		GIVEN("A triangle inside the first one.") {
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < shapes::rightTri.size(); ++i) inside.push_back(shapes::rightTri[i] * 0.5f);
			Polygon o(inside);
			Coordinate2D pos2(0, 0);
			THEN("They intersect.") {
				CHECK(isect::intersects(p, pos1, o, pos2));
				pos2 = Coordinate2D(0.5f, 0.5f);
				CHECK(isect::intersects(p, pos1, o, pos2));
			}
		}
		GIVEN("A triangle only sharing an edge.") {
			Polygon o(shapes::edgeTri);
			Coordinate2D pos2(0, 0);
			CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
		}
	}
	GIVEN("Two large triangles.") {
		std::vector<Coordinate2D> points1 = { Coordinate2D(0,0), Coordinate2D(1000000,1000000), Coordinate2D(1000000,0) };
		Polygon q(points1);
		Coordinate2D posQ(0, 0);
		std::vector<Coordinate2D> points2 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 0), Coordinate2D(3000000, -1000000) };
		Polygon r(points2);
		Coordinate2D posR(0, 0);
		CHECK_FALSE(isect::intersects(q, posQ, r, posR));
		std::vector<Coordinate2D> points3 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 1), Coordinate2D(3000000, -1000000) };
		r = Polygon(points3);
		CHECK(isect::intersects(q, posQ, r, posR));
	}
	GIVEN("A triangle and a rectangle.") {
		Polygon p(shapes::edgeTriR);
		Coordinate2D pos1(0, 0);
		Rectangle o(0, 0, 1, 1);
		Coordinate2D pos2(0, 0);
		CHECK(isect::intersects(p, pos1, o, pos2));
		pos1 = Coordinate2D(0.5f, 0);
		pos2 = Coordinate2D(1.5f, 0);
		CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
		pos2 = Coordinate2D(1, 1);
		CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
		pos2 = Coordinate2D(1, 0.9f);
		CHECK(isect::intersects(p, pos1, o, pos2));
		o = Rectangle(0, 0, 2, 2);
		pos1 = Coordinate2D(0, 0);
		pos2 = Coordinate2D(0, 0);
		CHECK(isect::intersects(p, pos1, o, pos2));
		pos1 = Coordinate2D(0.5f, 0.5f);
		CHECK(isect::intersects(p, pos1, o, pos2));
		pos1 = Coordinate2D(0.5f, -1);
		CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
	}
	GIVEN("Convex polygons.") {
		GIVEN("An octagon and an arbitrary convex polygon.") {
			Polygon p(shapes::arb);
			Coordinate2D pos1(0, 0);
			Polygon o(shapes::octagon);
			WHEN("They have a lot of overlap.") {
				Coordinate2D pos2(0, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, pos1, o, pos2));
			}
			WHEN("The octagon is nearly out the left side.") {
				Coordinate2D pos2(-1.9f, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, pos1, o, pos2));
			}
			WHEN("The octagon is nearly out of the right side.") {
				Coordinate2D pos2(4.9f, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, pos1, o, pos2));
			}
			WHEN("The octagon is only touching the left side.") {
				Coordinate2D pos2(-2, 0);
				THEN("Touching polygons are not intersecting.")
					CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
			}
			WHEN("The octagon is only touching the right side.") {
				Coordinate2D pos2(5, 0);
				THEN("They are not intersecting.")
					CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
			}
			WHEN("They are significantly apart.") {
				Coordinate2D pos2(20, 50);
				THEN("They do not intersect.")
					CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
			}
			WHEN("The octagon is nearly out of the top.") {
				Coordinate2D pos2(1, -3.9f);
				THEN("They intersect.")
					CHECK(isect::intersects(p, pos1, o, pos2));
			}
			WHEN("The octagon is only touching the top.") {
				Coordinate2D pos2(1, -4);
				THEN("They are not intersecting.")
					CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
			}
			WHEN("The octagon is edged out of the top left.") {
				Coordinate2D pos2(-0.66f, -3.9f);
				THEN("They are not intersecting.")
					CHECK_FALSE(isect::intersects(p, pos1, o, pos2));
			}
		}
		GIVEN("An octagon and a smaller octagon.") {
			Polygon p(shapes::octagon);
			Coordinate2D pos1(0, 0);
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < shapes::octagon.size(); ++i) inside.push_back(shapes::octagon[i] * 0.5f); // Half-size octagon.
			Polygon o(inside);
			WHEN("One is completely inside the other.") {
				Coordinate2D pos2(0, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, pos1, o, pos2));
			}
			WHEN("One is inside the other, sharing edges.") {
				Coordinate2D pos2(-1, 0);
				THEN("They intersect.")
					CHECK(isect::intersects(p, pos1, o, pos2));
			}
		}
	}
}