#include "catch.hpp"

#include "../Geometry/IntersectionMath.h"
#include "../Units.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/LineSegment.h"
#include "../Geometry/Ray.h"

using namespace units;

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
			s = LineSegment(2, 0, -10, 6);
			c = Coordinate2D(2, 0);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(-10, 6);
			REQUIRE(isect::intersects(s, c) == true);
			c = Coordinate2D(-4, 3);
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

TEST_CASE("Ray and coordinate intersections", "[ray][coord]") {
	SECTION("Coordinates on a diagonal ray") {
		Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
		Coordinate2D c(0, 0);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(1, 1);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(100000.5f, 100000.5f);
		REQUIRE(isect::intersects(r, c) == true);
		r = Ray(Coordinate2D(10, 5), Coordinate2D(-1, -0.5f).normalize());
		c = Coordinate2D(10, 5);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(9, 4.5f);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(0, 0);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(-60, -30);
		REQUIRE(isect::intersects(r, c) == true);
	}
	SECTION("Coordinates on a vertical ray") {
		Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1));
		Coordinate2D c(0, 0);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(0, 1);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(0, 10000000);
		REQUIRE(isect::intersects(r, c) == true);
		r = Ray(Coordinate2D(10, 2), Coordinate2D(0, -1));
		c = Coordinate2D(10, 2);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(10, 1);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(10, -1000000);
		REQUIRE(isect::intersects(r, c) == true);
	}
	SECTION("Coordinates on a horizontal ray") {
		Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0));
		Coordinate2D c(0, 0);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(1, 0);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(10000000, 0);
		REQUIRE(isect::intersects(r, c) == true);
		r = Ray(Coordinate2D(12, 4), Coordinate2D(-1, 0));
		c = Coordinate2D(12, 4);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(11, 4);
		REQUIRE(isect::intersects(r, c) == true);
		c = Coordinate2D(-10000000, 4);
		REQUIRE(isect::intersects(r, c) == true);
	}
	SECTION("Coordinates off a diagonal ray") {
		Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
		Coordinate2D c(1, 0);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(0, 1);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(100000, 100000.5f);
		REQUIRE(isect::intersects(r, c) == false);
		r = Ray(Coordinate2D(10, 5), Coordinate2D(-1, -0.5f).normalize());
		c = Coordinate2D(12, 6);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(9, 5);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(9, 3);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(-60.5f, -30);
		REQUIRE(isect::intersects(r, c) == false);
	}
	SECTION("Coordinates off a vertical ray") {
		Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1));
		Coordinate2D c(1, 0);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(0, -0.5f);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(0.5f, 10000000);
		REQUIRE(isect::intersects(r, c) == false);
		r = Ray(Coordinate2D(10, 2), Coordinate2D(0, -1));
		c = Coordinate2D(10, 2.5f);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(10.5f, 1);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(9.5f, -1000000);
		REQUIRE(isect::intersects(r, c) == false);
	}
	SECTION("Coordinates off a horizontal ray") {
		Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0));
		Coordinate2D c(-0.5f, 0);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(1, 0.5f);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(10000000, -0.5f);
		REQUIRE(isect::intersects(r, c) == false);
		r = Ray(Coordinate2D(12, 4), Coordinate2D(-1, 0));
		c = Coordinate2D(12.5f, 4);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(12, 3.5f);
		REQUIRE(isect::intersects(r, c) == false);
		c = Coordinate2D(-10000000, -4);
		REQUIRE(isect::intersects(r, c) == false);
	}
}

TEST_CASE("Line segment intersections", "[lineseg]") {
	SECTION("Intersecting line segments") {
		SECTION("Zero-length line segments") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(0, 0, 0, 0);
			REQUIRE(isect::intersects(s1, s2) == true);
			s2 = LineSegment(0, 0, 1, 1);
			REQUIRE(isect::intersects(s1, s2) == true);
		}
		SECTION("Diagonal line segments") {
			LineSegment s1(1, 1, 0, 0);
			LineSegment s2(1, 1, 0, 0);
			REQUIRE(isect::intersects(s1, s2) == true);
			s2 = LineSegment(0, 0, 1, 1);
			REQUIRE(isect::intersects(s1, s2) == true);
			s1 = LineSegment(-1, -1, 0, 0);
			REQUIRE(isect::intersects(s1, s2) == true);
			s1 = LineSegment(0, 1, 1, 0);
			REQUIRE(isect::intersects(s1, s2) == true);
		}
		SECTION("Horizontal and vertical line segments") {
			LineSegment v(0, -10, 0, 10);
			LineSegment s(0, -10, 0, 10);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(0, 10, 0, 20);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(0, -10, 0, -20);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(-50, -10, 0, -10);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(-50, -10, 50, -10);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(-50, 10, 0, 10);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(-50, 10, 50, 10);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(-50, 5, 0, 5);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(-50, 5, 50, 5);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(-10, 10, 10, -10);
			REQUIRE(isect::intersects(v, s) == true);
			v = LineSegment(30.5f, 20, 30.5f, 10);
			s = LineSegment(30.5f, 9, 30.5f, 11);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(30.5f, 10, 30.5f, 20);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(30.5f, -10, 30.5f, 20);
			REQUIRE(isect::intersects(v, s) == true);
			s = LineSegment(0, 20, 40, 10);
			REQUIRE(isect::intersects(v, s) == true);
		}
	}
	SECTION("Non-intersecting line segments") {
		SECTION("Zero-length line segments") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(1, 0, 1, 0);
			REQUIRE(isect::intersects(s1, s2) == false);
			s2 = LineSegment(1, 1, 1, 1);
			REQUIRE(isect::intersects(s1, s2) == false);
		}
		SECTION("Diagonal line segments") {
			LineSegment s1(1, 1, 0, 0);
			LineSegment s2(1, 2, 0, 1);
			REQUIRE(isect::intersects(s1, s2) == false);
			s2 = LineSegment(0, -0.5f, 1, 0.5f);
			REQUIRE(isect::intersects(s1, s2) == false);
			s2 = LineSegment(2, 2, 1.5f, 1.5f);
			REQUIRE(isect::intersects(s1, s2) == false);
			s1 = LineSegment(-1.5f, -1, 0.5f, 0);
			REQUIRE(isect::intersects(s1, s2) == false);
			s2 = LineSegment(0, -0.5f, 1.5f, 0.5f);
			REQUIRE(isect::intersects(s1, s2) == false);
			s2 = LineSegment(10, 0, 11, 1);
			REQUIRE(isect::intersects(s1, s2) == false);
		}
		SECTION("Horizontal and vertical line segments") {
			LineSegment v(0, -10, 0, 10);
			LineSegment s(0, -11, 0, -12);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(0, 11, 0, 20);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(0.5f, -10, 0.5f, -20);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(-50, -10, -0.5f, -10);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(-50, -10.5f, 50, -10.5f);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(-50, 10.5f, 0, 10.5f);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(-50, 10.5f, 50, 10.5f);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(0.5f, 5, 50, 5);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(-10, 10, 10, -50);
			REQUIRE(isect::intersects(v, s) == false);
			v = LineSegment(30.5f, 20, 30.5f, 10);
			s = LineSegment(30.5f, 9, 30.5f, 9.5f);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(30.5f, 20.5f, 30.5f, 25);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(30, -10, 30, 20);
			REQUIRE(isect::intersects(v, s) == false);
			s = LineSegment(0, 20, 40, -10);
			REQUIRE(isect::intersects(v, s) == false);
		}
	}
}

TEST_CASE("Line segment intersections with coordinate of intersection output", "[lineseg]") {
	SECTION("Intersecting line segments") {
		SECTION("Zero-length line segments") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(0, 0, 0, 0);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(s1, s2, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)));
			s2 = LineSegment(0, 0, 1, 1);
			REQUIRE(isect::intersects(s1, s2, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)));
		}
		SECTION("Diagonal line segments") {
			LineSegment s1(1, 1, 0, 0);
			LineSegment s2(1, 1, 0, 0);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(s1, s2, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(1, 1)));
			s2 = LineSegment(0, 0, 1, 1);
			REQUIRE(isect::intersects(s1, s2, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(1, 1)));
			s1 = LineSegment(-1, -1, 0, 0);
			REQUIRE(isect::intersects(s1, s2, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)));
			s1 = LineSegment(0, 1, 1, 0);
			REQUIRE(isect::intersects(s1, s2, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0.5f, 0.5f)));
		}
		SECTION("Horizontal and vertical line segments") {
			LineSegment v(0, -10, 0, 10);
			LineSegment s(0, -10, 0, 10);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, -10)));
			s = LineSegment(0, 10, 0, 20);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 10)));
			s = LineSegment(0, -10, 0, -20);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, -10)));
			s = LineSegment(-50, -10, 0, -10);
			REQUIRE(isect::intersects(v, s) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, -10)));
			s = LineSegment(-50, -10, 50, -10);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, -10)));
			s = LineSegment(-50, 10, 0, 10);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 10)));
			s = LineSegment(-50, 10, 50, 10);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 10)));
			s = LineSegment(-50, 5, 0, 5);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 5)));
			s = LineSegment(-50, 5, 50, 5);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 5)));
			s = LineSegment(-10, 10, 10, -10);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)));
			v = LineSegment(30.5f, 20, 30.5f, 10);
			s = LineSegment(30.5f, 9, 30.5f, 11);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(30.5f, 11)));
			s = LineSegment(30.5f, 10, 30.5f, 20);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(30.5f, 20)));
			s = LineSegment(30.5f, -10, 30.5f, 20);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(30.5f, 20)));
			s = LineSegment(0, 20, 40, 10);
			REQUIRE(isect::intersects(v, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(30.5f, 12.375f)));
		}
	}
	SECTION("Non-intersecting line segments") {
		SECTION("Zero-length line segments") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(1, 0, 1, 0);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(s1, s2, out_p) == false);
			s2 = LineSegment(1, 1, 1, 1);
			REQUIRE(isect::intersects(s1, s2, out_p) == false);
		}
		SECTION("Diagonal line segments") {
			LineSegment s1(1, 1, 0, 0);
			LineSegment s2(1, 2, 0, 1);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(s1, s2, out_p) == false);
			s2 = LineSegment(0, -0.5f, 1, 0.5f);
			REQUIRE(isect::intersects(s1, s2, out_p) == false);
			s2 = LineSegment(2, 2, 1.5f, 1.5f);
			REQUIRE(isect::intersects(s1, s2, out_p) == false);
			s1 = LineSegment(-1.5f, -1, 0.5f, 0);
			REQUIRE(isect::intersects(s1, s2, out_p) == false);
			s2 = LineSegment(0, -0.5f, 1.5f, 0.5f);
			REQUIRE(isect::intersects(s1, s2, out_p) == false);
			s2 = LineSegment(10, 0, 11, 1);
			REQUIRE(isect::intersects(s1, s2, out_p) == false);
		}
		SECTION("Horizontal and vertical line segments") {
			LineSegment v(0, -10, 0, 10);
			LineSegment s(0, -11, 0, -12);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(0, 11, 0, 20);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(0.5f, -10, 0.5f, -20);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(-50, -10, -0.5f, -10);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(-50, -10.5f, 50, -10.5f);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(-50, 10.5f, 0, 10.5f);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(-50, 10.5f, 50, 10.5f);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(0.5f, 5, 50, 5);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(-10, 10, 10, -50);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			v = LineSegment(30.5f, 20, 30.5f, 10);
			s = LineSegment(30.5f, 9, 30.5f, 9.5f);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(30.5f, 20.5f, 30.5f, 25);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(30, -10, 30, 20);
			REQUIRE(isect::intersects(v, s, out_p) == false);
			s = LineSegment(0, 20, 40, -10);
			REQUIRE(isect::intersects(v, s, out_p) == false);
		}
	}
}

TEST_CASE("Ray and line segment intersections with coordinate of intersection output", "[ray][lineseg]") {
	SECTION("Ray and a point") {
		Ray r(Coordinate2D(0, 0), Coordinate2D(1, 2).normalize());
		LineSegment s(1, 2, 1, 2);
		Coordinate2D out_p;
		REQUIRE(isect::intersects(r, s, out_p) == true);
		REQUIRE(util::almostEquals(out_p, Coordinate2D(1, 2)) == true);
		s = LineSegment(0, 0, 0, 0);
		REQUIRE(isect::intersects(r, s, out_p) == true);
		REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
		s = LineSegment(1, 1.5f, 1, 1.5f);
		REQUIRE(isect::intersects(r, s, out_p) == false);
	}
	SECTION("Parallel line segments and rays") {
		SECTION("Horizontal line segments and rays") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0).normalize());
			LineSegment s(0, 0, -1, 0);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(-0.1f, 0, -10, 0);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(5, 0, 1, 0);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(1, 0)) == true);
			s = LineSegment(50, 0, 100, 0);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(50, 0)) == true);
			s = LineSegment(50, 0.1f, 100, 0.1f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			r = Ray(Coordinate2D(-10, 10), Coordinate2D(-1, 0).normalize());
			s = LineSegment(10, 10, -100, 10);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-10, 10)) == true);
			s = LineSegment(-100, 10, -1000, 10);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-100, 10)) == true);
			s = LineSegment(-100, 10.1f, -1000, 10.1f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
		}
		SECTION("Vertical line segments and rays") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1).normalize());
			LineSegment s(0, 0, 0, 1);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(0, -0.1f, -10, 0);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(0, 5, 0, 1);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 1)) == true);
			s = LineSegment(0, 50, 0, 100);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 50)) == true);
			s = LineSegment(0.1f, 50, 0.1f, 100);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			r = Ray(Coordinate2D(10, -10), Coordinate2D(0, -1).normalize());
			s = LineSegment(10, 10, 10, -100);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(10, -10)) == true);
			s = LineSegment(10, -100, 10, -1000);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(10, -100)) == true);
			s = LineSegment(10.1f, -100, 10.1f, -1000);
			REQUIRE(isect::intersects(r, s, out_p) == false);
		}
		SECTION("Diagonal line segments and rays") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
			LineSegment s(0, 0, -1, -1);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(-0.1f, -0.1f, -10, -10);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(5, 5, 1, 1);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(1, 1)) == true);
			s = LineSegment(50, 50, 100, 100);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(50, 50)) == true);
			s = LineSegment(50.1f, 50, 100.1f, 100);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			r = Ray(Coordinate2D(10, 10), Coordinate2D(-1, 2).normalize());
			s = LineSegment(15, 0, 0, 30);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(10, 10)) == true);
			s = LineSegment(0, 30, -35, 100);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 30)) == true);
			s = LineSegment(0.1f, 30, -34.9, 100);
			REQUIRE(isect::intersects(r, s, out_p) == false);
		}
	}
	SECTION("Perpendicular line segments and rays") {
		SECTION("Horizontal ray") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0).normalize());
			LineSegment s(0, 0, 0, -1);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(0, 10, 0, -10);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(-0.1f, 10, -0.1f, 10);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(1000, 10, 1000, -10);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(1000, 0)) == true);
			r = Ray(Coordinate2D(-10, -10), Coordinate2D(-1, 0).normalize());
			s = LineSegment(-10, -10, -10, 10);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-10, -10)) == true);
			s = LineSegment(-9.9f, -10, -9.9f, 10);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(-1000, 10, -1000, -20);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-1000, -10)) == true);
		}
		SECTION("Vertical ray") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1).normalize());
			LineSegment s(0, 0, -1, 0);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(10, 0, -10, 0);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(10, -0.1f, 10, -0.1f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(10, 1000, -10, 1000);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 1000)) == true);
			r = Ray(Coordinate2D(-10, -10), Coordinate2D(0, -1).normalize());
			s = LineSegment(10, -10, -10, -10);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-10, -10)) == true);
			s = LineSegment(10, -9.9f, -10, -9.9f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(10, -1000, -10, -1000);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-10, -1000)) == true);
		}
		SECTION("Diagonal ray") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
			LineSegment s(0, 0, -5, 5);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(5, -5, -5, 5);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(5, -5, 0.1f, -0.1f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(-5, 4.9f, 5, 5.1f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(0, 1000, 1000, 0);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(500, 500)) == true);
			r = Ray(Coordinate2D(-10, 10), Coordinate2D(1, -2).normalize());
			s = LineSegment(-10, -5, 0, 0);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-4, -2)) == true);
			s = LineSegment(-20, 5.1f, 0, 15.1f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(0, -2000, 4000, 0);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(796, -1602)) == true);
		}
	}
	SECTION("Diagonal line segment and ray intersections") {
		SECTION("Horizontal ray") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 0).normalize());
			LineSegment s(0, -1, 1000, 1);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(500, 0)) == true);
			s = LineSegment(1000, 1, 0, -1);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(500, 0)) == true);
			s = LineSegment(0, -4000, 1, 4000);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0.5f, 0)) == true);
			s = LineSegment(-2, -4000, 1, 4000);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			r = Ray(Coordinate2D(10, -10), Coordinate2D(-1, 0).normalize());
			s = LineSegment(0, 20, -20, -40);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-10, -10)) == true);
			s = LineSegment(-65, 140, 15, -20);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(10, -10)) == true);
			s = LineSegment(-64.9f, 140, 15.1f, -20);
			REQUIRE(isect::intersects(r, s, out_p) == false);
		}
		SECTION("Vertical ray") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(0, 1).normalize());
			LineSegment s(-1, 0, 1, 1000);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 500)) == true);
			s = LineSegment(1, 1000, -1, 0);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 500)) == true);
			s = LineSegment(-4000, 0, 4000, 1);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0.5f)) == true);
			s = LineSegment(-4000, -2, 4000, 1);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			r = Ray(Coordinate2D(10, -10), Coordinate2D(-1, 0).normalize());
			s = LineSegment(20, 0, -40, -20);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-10, -10)) == true);
			s = LineSegment(100, -85, -20, 15);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(10, -10)) == true);
			s = LineSegment(140, -64.9f, -20, 15.1f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
		}
		SECTION("Diagonal ray") {
			Ray r(Coordinate2D(0, 0), Coordinate2D(1, 1).normalize());
			LineSegment s(-10, 0, 10, 0);
			Coordinate2D out_p;
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(0, 0)) == true);
			s = LineSegment(800, 0, 800, 1000);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(800, 800)) == true);
			s = LineSegment(-0.1f, -10, -0.1f, -10);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(-20, 50, 20, 50.1f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(10, 100, 100, 55);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(70, 70)) == true);
			r = Ray(Coordinate2D(-10, 10), Coordinate2D(1, -2).normalize());
			s = LineSegment(-10, -5, 0, 0);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(-4, -2)) == true);
			s = LineSegment(-20, 5.1f, 0, 15.1f);
			REQUIRE(isect::intersects(r, s, out_p) == false);
			s = LineSegment(0, -2000, 4000, 0);
			REQUIRE(isect::intersects(r, s, out_p) == true);
			REQUIRE(util::almostEquals(out_p, Coordinate2D(796, -1602)) == true);
		}
	}
}

TEST_CASE("Rectangle line segment intersections", "[rect][lineseg]") {
	Rectangle r(5, 5, 2, 2);
	LineSegment s(5, 5, 5, 5);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(6, 6, 6, 6);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(7, 7, 7, 7);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(0, 0, 5, 5);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(7, 6, 8, 8);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(6, 5.5f, 6, 6.5f);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(4, 6, 6, 6);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(4, 6, 8, 6);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(4, 5, 8, 6);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(6, 6, 9, 10);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(4, 8, 8, 4);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(6, 6, 6, 8);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(6, 6, 6, -8);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(6, 6, 8, 6);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(6, 6, -8, 6);
	REQUIRE(isect::intersects(r, s) == true);
	s = LineSegment(7.1f, 6, 8, 6);
	REQUIRE(isect::intersects(r, s) == false);
	s = LineSegment(4.9f, -7, 4.9f, 10);
	REQUIRE(isect::intersects(r, s) == false);
	s = LineSegment(6, 7.1f, 8, 8);
	REQUIRE(isect::intersects(r, s) == false);
	s = LineSegment(7.1f, 7, 8, 6);
	REQUIRE(isect::intersects(r, s) == false);
	s = LineSegment(6, 4.9f, 6, 4.9f);
	REQUIRE(isect::intersects(r, s) == false);
}

TEST_CASE("Rectangle intersections", "[rect]") {
	Rectangle r(0, 0, 1, 1);
	Rectangle o(0, 0, 1, 1);
	REQUIRE(isect::intersects(r, o) == true);
	o = Rectangle(-1, 0, 1, 1);
	REQUIRE(isect::intersects(r, o) == false);
	o = Rectangle(-10, 0, 10, 1);
	REQUIRE(isect::intersects(r, o) == false);
	o = Rectangle(-10, 0, 10.1f, 1);
	REQUIRE(isect::intersects(r, o) == true);
	o = Rectangle(0.9f, -0.9f, 1, 1);
	REQUIRE(isect::intersects(r, o) == true);
	o = Rectangle(0.5f, -1, 1, 1);
	REQUIRE(isect::intersects(r, o) == false);
	o = Rectangle(0.5f, 0.5f, 0.25f, 0.25f);
	REQUIRE(isect::intersects(r, o) == true);
	r = Rectangle(-10, -10, 5, 5);
	REQUIRE(isect::intersects(r, o) == false);
	o = Rectangle(-10, -5, 1, 1);
	REQUIRE(isect::intersects(r, o) == false);
	o = Rectangle(-10, -5.1f, 1, 1);
	REQUIRE(isect::intersects(r, o) == true);
}

TEST_CASE("Polygon intersections", "[poly]") {
	SECTION("Triangles") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1,1), Coordinate2D(1,0) };
		Polygon p(points);
		std::vector<Coordinate2D> points2 = { Coordinate2D(-1, -2), Coordinate2D(1, 0), Coordinate2D(3, -1) };
		Polygon o(points2);
		REQUIRE(isect::intersects(p, o) == false);
		o.translate(Coordinate2D(0, 1));
		REQUIRE(isect::intersects(p, o) == true);
		o.translate(Coordinate2D(1.9f, 1.5f));
		REQUIRE(isect::intersects(p, o) == true);
		o.translate(Coordinate2D(0.1f, 0));
		REQUIRE(isect::intersects(p, o) == false);
		o = Polygon(points);
		REQUIRE(isect::intersects(p, o) == true);
		for (size_t i = 0; i < points.size(); ++i) points[i] *= 0.5f;
		o = Polygon(points);
		REQUIRE(isect::intersects(p, o) == true);
		o.translate(Coordinate2D(0.5f, 0.5f));
		REQUIRE(isect::intersects(p, o) == true);
	}
	SECTION("Triangle and rectangle") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(0,1), Coordinate2D(1,1) };
		Polygon p(points);
		Polygon o(Rectangle(0, 0, 1, 1).toPoly());
		REQUIRE(isect::intersects(p, o) == true);
		o.translate(Coordinate2D(1, 0));
		REQUIRE(isect::intersects(p, o) == false);
		o.translate(Coordinate2D(-0.5f, 1));
		REQUIRE(isect::intersects(p, o) == false);
		o.translate(Coordinate2D(0, -0.1f));
		REQUIRE(isect::intersects(p, o) == true);
		o = Rectangle(0, 0, 2, 2).toPoly();
		REQUIRE(isect::intersects(p, o) == true);
		o.translate(Coordinate2D(-0.5f, -0.5f));
		REQUIRE(isect::intersects(p, o) == true);
		p.translate(Coordinate2D(0, -1.5f));
		REQUIRE(isect::intersects(p, o) == false);
	}
	SECTION("Convex polygons") {
		// Arbitrary convex polygon.
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
		Polygon p(points);
		// Octagon.
		std::vector<Coordinate2D> points2 = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                  Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		Polygon o(points2);
		REQUIRE(isect::intersects(p, o) == true);
		o.translate(Coordinate2D(-1.9f, 0));
		REQUIRE(isect::intersects(p, o) == true);
		o.translate(Coordinate2D(-0.1f, 0));
		REQUIRE(isect::intersects(p, o) == false);
		o.translate(Coordinate2D(7, 0));
		REQUIRE(isect::intersects(p, o) == false);
		o.translate(Coordinate2D(-0.1f, 0));
		REQUIRE(isect::intersects(p, o) == true);
		o = Polygon(points2); // Rreset it.
		o.translate(Coordinate2D(1, -3.9f));
		REQUIRE(isect::intersects(p, o) == true);
		o.translate(Coordinate2D(-0.33f, 0));
		REQUIRE(isect::intersects(p, o) == false);
		o.translate(Coordinate2D(0.33f, -0.1f));
		REQUIRE(isect::intersects(p, o) == false);
		SECTION("Polygon in polygon") {
			o = Polygon(points2); // Reset it again.
			for (size_t i = 0; i < points2.size(); ++i) points2[i] *= 0.5f; // Half-size octagon.
			p = Polygon(points2);
			REQUIRE(isect::intersects(p, o) == true);
			o.translate(Coordinate2D(-0.5f, -0.5f));
			REQUIRE(isect::intersects(p, o) == true);
		}
	}
}