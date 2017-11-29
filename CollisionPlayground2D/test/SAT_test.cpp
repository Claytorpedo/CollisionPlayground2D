#include "catch.hpp"
#include "definitions.h"

#include <vector>

#include "../Units.h"
#include "../Constants.h"
#include "../Geometry/SAT.h"
#include "../Geometry/ShapeContainer.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/Circle.h"

using namespace units;

SCENARIO("Testing two shapes for overlap.", "[sat]") {
	GIVEN("A triangle.") {
		Polygon p(shapes::rightTri);
		GIVEN("A second triangle.") {
			Polygon o(shapes::tri);
			THEN("They don't overlap until we move them together.") {
				CHECK_FALSE(sat::performSAT(p, o));
				o.translate(0, 1);
				CHECK(sat::performSAT(p, o));
				o.translate(1.9f, 1.5f);
				CHECK(sat::performSAT(p, o));
				o.translate(0.1f, 0);
				CHECK_FALSE(sat::performSAT(p, o));
			}
		}
		GIVEN("An identical triangle.") {
			Polygon o(shapes::rightTri);
			THEN("They overlap.")
				CHECK(sat::performSAT(p, o));
		}
		GIVEN("A triangle inside the first one.") {
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < shapes::rightTri.size(); ++i) inside.push_back(shapes::rightTri[i] * 0.5f);
			Polygon o(inside);
			THEN("They overlap.") {
				CHECK(sat::performSAT(p, o));
				o.translate(0.5f, 0.5f);
				CHECK(sat::performSAT(p, o));
			}
		}
		GIVEN("A triangle only sharing an edge.") {
			Polygon o(shapes::edgeTriR);
			CHECK_FALSE(sat::performSAT(p, o));
		}
	}
	GIVEN("Two large triangles.") {
		std::vector<Coordinate2D> points1 = { Coordinate2D(0,0), Coordinate2D(1000000,1000000), Coordinate2D(1000000,0) };
		Polygon q(points1);
		std::vector<Coordinate2D> points2 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 0), Coordinate2D(3000000, -1000000) };
		Polygon r(points2);
		CHECK_FALSE(sat::performSAT(q, r));
		std::vector<Coordinate2D> points3 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 1), Coordinate2D(3000000, -1000000) };
		r = Polygon(points3);
		CHECK(sat::performSAT(q, r));
	}
	GIVEN("A triangle and a rectangle.") {
		Polygon p(shapes::edgeTriR);
		Rectangle o(0, 0, 1, 1);
		CHECK(sat::performSAT(p, o));
		o += Coordinate2D(1, 0);
		CHECK_FALSE(sat::performSAT(p, o));
		o += Coordinate2D(-0.5f, 1);
		CHECK_FALSE(sat::performSAT(p, o));
		o += Coordinate2D(0, -0.1f);
		CHECK(sat::performSAT(p, o));
		o = Rectangle(0, 0, 2, 2);
		CHECK(sat::performSAT(p, o));
		o += Coordinate2D(-0.5f, -0.5f);
		CHECK(sat::performSAT(p, o));
		p.translate(0, -1.5f);
		CHECK_FALSE(sat::performSAT(p, o));
	}
	GIVEN("Convex shapes.") {
		GIVEN("An octagon and an arbitrary convex shape.") {
			Polygon p(shapes::arb);
			Polygon o(shapes::octagon);
			WHEN("They have a lot of overlap.") {
				THEN("They overlap.")
					CHECK(sat::performSAT(p, o));
			}
			WHEN("The octagon is nearly out the left side.") {
				o.translate(-1.9f, 0);
				THEN("They overlap.")
					CHECK(sat::performSAT(p, o));
			}
			WHEN("The octagon is nearly out of the right side.") {
				o.translate(4.9f, 0);
				THEN("They overlap.")
					CHECK(sat::performSAT(p, o));
			}
			WHEN("The octagon is nearly out of the top.") {
				o.translate(1, -3.9f);
				THEN("They overlap.")
					CHECK(sat::performSAT(p, o));
			}
			WHEN("The octagon is only touching the left side.") {
				o.translate(-2, 0);
				THEN("Touching shapes are not overlapping.")
					CHECK_FALSE(sat::performSAT(p, o));
			}
			WHEN("The octagon is only touching the right side.") {
				o.translate(5, 0);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(p, o));
			}
			WHEN("They are significantly apart.") {
				o.translate(20, 50);
				THEN("They do not overlap.")
					CHECK_FALSE(sat::performSAT(p, o));
			}
			WHEN("The octagon is only touching the top.") {
				o.translate(1, -4);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(p, o));
			}
			WHEN("The octagon is edged out of the top left.") {
				o.translate(-0.66f, -3.9f);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(p, o));
			}
		}
		GIVEN("An octagon and a smaller octagon.") {
			Polygon p(shapes::octagon);
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < shapes::octagon.size(); ++i) inside.push_back(shapes::octagon[i] * 0.5f); // Half-size octagon.
			Polygon o(inside);
			WHEN("One is completely inside the other.") {
				THEN("They overlap.")
					CHECK(sat::performSAT(p, o));
			}
			WHEN("One is inside the other, sharing edges.") {
				o.translate(-1, 0);
				THEN("They overlap.")
					CHECK(sat::performSAT(p, o));
			}
		}
	}
	GIVEN("A circle.") {
		Circle c(5);
		GIVEN("Another circle") {
			Circle c2(5);
			WHEN("The circles have significant overlap.") {
				c2.center = Coordinate2D(0, 8);
				THEN("They overlap.")
					CHECK(sat::performSAT(c, c2));
			}
			WHEN("Their edges are touching.") {
				c2.center = Coordinate2D(10, 0);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(c, c2));
			}
			WHEN("Their centers are the same position.") {
				THEN("They are overlapping.")
					CHECK(sat::performSAT(c, c2));
			}
		}
		GIVEN("A rectangle.") {
			Rectangle r(0, 0, 2, 2);
			WHEN("The circle is overlapping an edge.") {
				r += Coordinate2D(-1, 4.9f);
				THEN("They are overlapping.")
					CHECK(sat::performSAT(c, r));
			}
			WHEN("The circle is overlapping a corner.") {
				r += Coordinate2D(1, 1).normalize() * 4.9f;
				THEN("They are overlapping.")
					CHECK(sat::performSAT(c, r));
			}
			WHEN("The circle is touching an edge, but not overlapping.") {
				r += Coordinate2D(-1, 5);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(c, r));
			}
			WHEN("The circle is close to a corner, but not touching.") {
				r += Coordinate2D(1, 1).normalize() * 5.1f;
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(c, r));
			}
			WHEN("The circle is touching a corner, but not overlapping.") {
				r += Coordinate2D(1, 1).normalize() * 5.0f;
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(c, r));
			}
		}
		GIVEN("An octagon.") {
			Polygon p(shapes::octagon);
			WHEN("The circle is touching a vertex, but not overlapping.") {
				p.translate(7, 0);
				THEN("They don't overlap.")
					CHECK_FALSE(sat::performSAT(c, p));
			}
			WHEN("The circle is overlapping a vertex.") {
				p.translate(6.9f, 0);
				THEN("They overlap.")
					CHECK(sat::performSAT(c, p));
			}
			WHEN("The circle is in the middle of the octagon.") {
				THEN("They overlap.")
					CHECK(sat::performSAT(c, p));
			}
		}
	}
}

SCENARIO("Testing two shapes for overlap with given positions.", "[sat]") {
	GIVEN("A triangle.") {
		Polygon p(shapes::rightTri);
		Coordinate2D pos1(0, 0);
		GIVEN("A second triangle.") {
			Polygon o(shapes::tri);
			Coordinate2D pos2(0, 0);
			THEN("They don't overlap until we move them together.") {
				CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
				pos2 = Coordinate2D(0, 1);
				CHECK(sat::performSAT(p, pos1, o, pos2));
				pos2 = Coordinate2D(1.9f, 2.5f);
				CHECK(sat::performSAT(p, pos1, o, pos2));
				pos1 = Coordinate2D(-0.1f, 0);
				CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
			}
		}
		GIVEN("An identical triangle.") {
			Polygon o(shapes::rightTri);
			Coordinate2D pos2(0, 0);
			THEN("They overlap.")
				CHECK(sat::performSAT(p, pos1, o, pos2));
		}
		GIVEN("A triangle inside the first one.") {
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < shapes::rightTri.size(); ++i) inside.push_back(shapes::rightTri[i] * 0.5f);
			Polygon o(inside);
			Coordinate2D pos2(0, 0);
			THEN("They overlap.") {
				CHECK(sat::performSAT(p, pos1, o, pos2));
				pos2 = Coordinate2D(0.5f, 0.5f);
				CHECK(sat::performSAT(p, pos1, o, pos2));
			}
		}
		GIVEN("A triangle only sharing an edge.") {
			Polygon o(shapes::edgeTriR);
			Coordinate2D pos2(0, 0);
			CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
		}
	}
	GIVEN("Two large triangles.") {
		std::vector<Coordinate2D> points1 = { Coordinate2D(0,0), Coordinate2D(1000000,1000000), Coordinate2D(1000000,0) };
		Polygon q(points1);
		Coordinate2D posQ(0, 0);
		std::vector<Coordinate2D> points2 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 0), Coordinate2D(3000000, -1000000) };
		Polygon r(points2);
		Coordinate2D posR(0, 0);
		CHECK_FALSE(sat::performSAT(q, posQ, r, posR));
		std::vector<Coordinate2D> points3 = { Coordinate2D(-1000000, -2000000), Coordinate2D(1000000, 1), Coordinate2D(3000000, -1000000) };
		r = Polygon(points3);
		CHECK(sat::performSAT(q, posQ, r, posR));
	}
	GIVEN("A triangle and a rectangle.") {
		Polygon p(shapes::edgeTriR);
		Coordinate2D pos1(0, 0);
		Rectangle o(0, 0, 1, 1);
		Coordinate2D pos2(0, 0);
		CHECK(sat::performSAT(p, pos1, o, pos2));
		pos1 = Coordinate2D(0.5f, 0);
		pos2 = Coordinate2D(1.5f, 0);
		CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
		pos2 = Coordinate2D(1, 1);
		CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
		pos2 = Coordinate2D(1, 0.9f);
		CHECK(sat::performSAT(p, pos1, o, pos2));
		o = Rectangle(0, 0, 2, 2);
		pos1 = Coordinate2D(0, 0);
		pos2 = Coordinate2D(0, 0);
		CHECK(sat::performSAT(p, pos1, o, pos2));
		pos1 = Coordinate2D(0.5f, 0.5f);
		CHECK(sat::performSAT(p, pos1, o, pos2));
		pos1 = Coordinate2D(0.5f, -1);
		CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
	}
	GIVEN("Convex shapes.") {
		GIVEN("An octagon and an arbitrary convex shape.") {
			Polygon p(shapes::arb);
			Coordinate2D pos1(0, 0);
			Polygon o(shapes::octagon);
			WHEN("They have a lot of overlap.") {
				Coordinate2D pos2(0, 0);
				THEN("They overlap.")
					CHECK(sat::performSAT(p, pos1, o, pos2));
			}
			WHEN("The octagon is nearly out the left side.") {
				Coordinate2D pos2(-1.9f, 0);
				THEN("They overlap.")
					CHECK(sat::performSAT(p, pos1, o, pos2));
			}
			WHEN("The octagon is nearly out of the right side.") {
				Coordinate2D pos2(4.9f, 0);
				THEN("They overlap.")
					CHECK(sat::performSAT(p, pos1, o, pos2));
			}
			WHEN("The octagon is nearly out of the top.") {
				Coordinate2D pos2(1, -3.9f);
				THEN("They overlap.")
					CHECK(sat::performSAT(p, pos1, o, pos2));
			}
			WHEN("The octagon is only touching the left side.") {
				Coordinate2D pos2(-2, 0);
				THEN("Touching shapes are not overlapping.")
					CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
			}
			WHEN("The octagon is only touching the right side.") {
				Coordinate2D pos2(5, 0);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
			}
			WHEN("They are significantly apart.") {
				Coordinate2D pos2(20, 50);
				THEN("They do not overlap.")
					CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
			}
			WHEN("The octagon is only touching the top.") {
				Coordinate2D pos2(1, -4);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
			}
			WHEN("The octagon is edged out of the top left.") {
				Coordinate2D pos2(-0.66f, -3.9f);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
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
				THEN("They overlap.")
					CHECK(sat::performSAT(p, pos1, o, pos2));
			}
			WHEN("One is inside the other, sharing edges.") {
				Coordinate2D pos2(-1, 0);
				THEN("They overlap.")
					CHECK(sat::performSAT(p, pos1, o, pos2));
			}
		}
	}
	GIVEN("A circle.") {
		Circle c(5);
		GIVEN("Another circle") {
			Circle c2(5);
			WHEN("The circles have significant overlap.") {
				Coordinate2D pos1(0, -4);
				Coordinate2D pos2(0, 4);
				THEN("They overlap.")
					CHECK(sat::performSAT(c, pos1, c2, pos2));
			}
			WHEN("Their edges are touching.") {
				Coordinate2D pos1(-5, 0);
				Coordinate2D pos2(5, 0);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(c, pos1, c2, pos2));
			}
			WHEN("Their centers are the same position.") {
				Coordinate2D pos1(10, 11);
				Coordinate2D pos2(10, 11);
				THEN("They are overlapping.")
					CHECK(sat::performSAT(c, pos1, c2, pos2));
			}
		}
		GIVEN("A rectangle.") {
			Rectangle r(0, 0, 2, 2);
			WHEN("The circle is overlapping an edge.") {
				Coordinate2D pos1(1, 0);
				Coordinate2D pos2(0, 4.9f);
				THEN("They are overlapping.")
					CHECK(sat::performSAT(c, pos1, r, pos2));
			}
			WHEN("The circle is overlapping a corner.") {
				Coordinate2D dir(Coordinate2D(1, 1).normalize());
				Coordinate2D pos1(dir * 2.0f);
				Coordinate2D pos2(dir * 6.9f);
				THEN("They are overlapping.")
					CHECK(sat::performSAT(c, pos1, r, pos2));
			}
			WHEN("The circle is touching an edge, but not overlapping.") {
				Coordinate2D pos1(1, 0);
				Coordinate2D pos2(0, 5);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(c, pos1, r, pos2));
			}
			WHEN("The circle is close to a corner, but not touching.") {
				Coordinate2D dir(Coordinate2D(1, 1).normalize());
				Coordinate2D pos1(dir * -3.1f);
				Coordinate2D pos2(dir * 2.0f);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(c, pos1, r, pos2));
			}
			WHEN("The circle is touching a corner, but not overlapping.") {
				Coordinate2D dir(Coordinate2D(1, 1).normalize());
				Coordinate2D pos1(dir * -3.0f);
				Coordinate2D pos2(dir * 2.0f);
				THEN("They are not overlapping.")
					CHECK_FALSE(sat::performSAT(c, pos1, r, pos2));
			}
		}
		GIVEN("An octagon.") {
			Polygon p(shapes::octagon);
			WHEN("The circle is touching a vertex, but not overlapping.") {
				Coordinate2D pos1(-1, 0);
				Coordinate2D pos2(6, 0);
				THEN("They don't overlap.")
					CHECK_FALSE(sat::performSAT(c, pos1, p, pos2));
			}
			WHEN("The circle is overlapping a vertex.") {
				Coordinate2D pos1(-1, 0);
				Coordinate2D pos2(5.9f, 0);
				THEN("They overlap.")
					CHECK(sat::performSAT(c, pos1, p, pos2));
			}
			WHEN("The circle is in the middle of the octagon.") {
				Coordinate2D pos1(1, 2);
				Coordinate2D pos2(1, 2);
				THEN("They overlap.")
					CHECK(sat::performSAT(c, pos1, p, pos2));
			}
		}
	}
}

SCENARIO("Two shapes are overlapping, and need to be separated (by the minimum translation vector).", "[sat]") {
	Coordinate2D out_norm;
	Coordinate out_dist;
	GIVEN("The shape to be moved is a rectangle, and the stationary one is a triangle.") {
		Rectangle p(0, 0, 1, 1);
		Polygon o(shapes::rightTri);
		Coordinate2D pos1(0, 0), pos2(0, 0);
		WHEN("The rectangle slightly overlaps the triangle from the right.") {
			pos1 = Coordinate2D(0.8f, 0);
			THEN("The rectangle is separated out of the triangle to the right.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(1, 0);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The rectangle slightly overlaps the triangle from the top.") {
			pos1 = Coordinate2D(0, -0.8f);
			THEN("The rectangle is separated out of the triangle upwards.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(0, -1);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The rectangle slightly overlaps the triangle along the hypotenuse.") {
			pos1 = Coordinate2D(-0.4f, 0.4f);
			THEN("The rectangle is separated out of the triangle left-downwards") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1.0f/std::sqrt(2.0f) * 0.2f)); // dist is adjacent of right triangle with angle pi/4 and hypotenuse 0.2.
			}
		}
		WHEN("The rectangle slightly overlaps the triangle along the bottom.") {
			pos1 = Coordinate2D(0.5f, 0.8f);
			THEN("The rectangle is separated out of the triangle downwards") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(0, 1);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
	}
	GIVEN("A large rectangle, and a smaller triangle.") {
		Rectangle p(0, 0, 100, 100);
		Polygon o(shapes::rightTri);
		Coordinate2D pos1(0, 0), pos2(0, 0);
		WHEN("The rectangle slightly overlaps the triangle from the right.") {
			pos1 = Coordinate2D(0.8f, 0);
			THEN("The rectangle is separated out of the triangle to the right.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(1, 0);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The rectangle slightly overlaps the triangle from the top.") {
			pos1 = Coordinate2D(0, -99.8f);
			THEN("The rectangle is separated out of the triangle upwards.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(0, -1);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The rectangle slightly overlaps the triangle along the hypotenuse.") {
			pos1 = Coordinate2D(-99.4f, 0.4f);
			THEN("The rectangle is separated out of the triangle left-downwards") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1.0f / std::sqrt(2.0f) * 0.2f)); // dist is adjacent of right triangle with angle pi/4 and hypotenuse 0.2.
			}
		}
		WHEN("The rectangle slightly overlaps the triangle along the bottom.") {
			pos1 = Coordinate2D(-50, 0.8f);
			THEN("The rectangle is separated out of the triangle downwards") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(0, 1);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The triangle is just below the rectangle's top-left corner.") {
			pos2 = Coordinate2D(0, 0.1f);
			THEN("The rectangle is separated out of the triangle rightwards.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(1, 0));
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1));
			}
		}
		WHEN("The triangle is just below the rectangle's bottom-left corner.") {
			pos2 = Coordinate2D(0, 99.1f);
			THEN("The rectangle is separated out of the triangle upwards.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(0, -1));
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.9f));
			}
		}
		WHEN("The triangle is just to the right of the rectangle's top-right corner.") {
			pos2 = Coordinate2D(99.1f, 0);
			THEN("The rectangle is separated out of the triangle left-downwards.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(Coordinate2D(0.45f,0.45f).magnitude()));
			}
		}
		WHEN("The triangle is just below the rectangle's bottom-right corner.") {
			pos2 = Coordinate2D(99, 99.1f);
			THEN("The rectangle is separated out of the triangle upwards.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(0, -1));
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.9f));
			}
		}
		WHEN("The triangle is just above the rectangle's center.") {
			pos2 = Coordinate2D(49.5f, 49.1f);
			THEN("The rectangle is separated out of the triangle downwards.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(0, 1));
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(50.1f));
			}
		}
	}
	GIVEN("A smaller triangle and a larger triangle.") {
		Polygon p(shapes::rightTri);
		std::vector<Coordinate2D> large;
		for (std::size_t i = 0; i < shapes::rightTri.size(); ++i) large.push_back(shapes::rightTri[i] * 100.0f);
		Polygon o(large);
		Coordinate2D pos1(0, 0), pos2(0, 0);
		WHEN("The small triangle overlaps the large one along its hypotenuse.") {
			pos1 = Coordinate2D(50, 50);
			THEN("The small triangle is separated left-downward.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1.0f / std::sqrt(2.0f)));
			}
		}
		WHEN("The small triangle overlaps the large one inside its hypotenuse.") {
			pos1 = Coordinate2D(51, 50);
			THEN("The small triangle is separated left-downward.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(2.0f / std::sqrt(2.0f)));
			}
		}
	}
	GIVEN("A circle.") {
		Circle c(5);
		GIVEN("Another circle") {
			Circle c2(5);
			WHEN("The circles have significant overlap.") {
				Coordinate2D pos1(0, -4);
				Coordinate2D pos2(0, 4);
				THEN("The first circle is separated upwards.") {
					Coordinate2D expected_norm(0, -1);
					REQUIRE(sat::performSAT(c, pos1, c2, pos2, out_norm, out_dist));
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_dist == ApproxEps(2.0f));
				}
			}
			WHEN("Their centers are the same position.") {
				Coordinate2D pos1(10, 11);
				Coordinate2D pos2(10, 11);
				THEN("They are separated (in any direction).") {
					REQUIRE(sat::performSAT(c, pos1, c2, pos2, out_norm, out_dist));
					CHECK( (out_norm.x != 0.0f || out_norm.y != 0.0f) ); // Not a zero vector.
					CHECK(out_dist == ApproxEps(10.0f));
				}
			}
		}
		GIVEN("A rectangle.") {
			Rectangle r(0, 0, 2, 2);
			WHEN("The circle is overlapping an edge.") {
				Coordinate2D pos1(1, 0);
				Coordinate2D pos2(0, 4.9f);
				THEN("The circle is pushed out upwards by the edge normal.") {
					REQUIRE(sat::performSAT(c, pos1, r, pos2, out_norm, out_dist));
					CHECK(out_norm.x == ApproxEps(0));
					CHECK(out_norm.y == ApproxEps(-1));
					CHECK(out_dist == ApproxEps(0.1f));
				}
			}
			WHEN("The circle is overlapping a corner.") {
				Coordinate2D dir(Coordinate2D(1, 1).normalize());
				Coordinate2D pos1(dir * 2.0f);
				Coordinate2D pos2(dir * 6.9f);
				THEN("The circle is pushed out diagonally, along its axis with the corner.") {
					Coordinate2D expected_norm(-dir);
					REQUIRE(sat::performSAT(c, pos1, r, pos2, out_norm, out_dist));
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_dist == ApproxEps(0.1f));
				}
			}
		}
		GIVEN("An octagon.") {
			Polygon p(shapes::octagon);
			WHEN("The circle is overlapping a vertex.") {
				Coordinate2D pos1(-1, 0);
				Coordinate2D pos2(5.9f, 0);
				THEN("The circle is pushed out left, along its axis with the vertex..") {
					REQUIRE(sat::performSAT(c, pos1, p, pos2, out_norm, out_dist));
					CHECK(out_norm.x == ApproxEps(-1));
					CHECK(out_norm.y == ApproxEps(0));
					CHECK(out_dist == ApproxEps(0.1f));
				}
			}
			WHEN("The circle is in the middle of the octagon.") {
				Coordinate2D pos1(1, 2);
				Coordinate2D pos2(1, 2);
				THEN("The circle is pushed out of one of the octagon's faces.") {
					// Find the distance out of one of the octagon's faces.
					Coordinate2D dir(Coordinate2D(0.5f, 1.5f).normalize());
					Coordinate cosTheta(Coordinate2D(0, 1).dot(dir));
					Coordinate outDist = 5.0f + cosTheta * 2.0f; // Radius + cosTheta * octagon_height.
					REQUIRE(sat::performSAT(c, pos1, p, pos2, out_norm, out_dist));
					CHECK((out_norm.x != 0.0f || out_norm.y != 0.0f)); // Not a zero vector.
					CHECK(out_dist == ApproxEps(outDist));
				}
			}
		}
	}
	GIVEN("An octagon and arbitrary convex shape.") {
		Polygon p(shapes::octagon);
		Polygon o(shapes::arb);
		Coordinate2D pos1(0, 0), pos2(0, 0);
		WHEN("They octagon overlaps the arbitrary shape's left side.") {
			pos1 = Coordinate2D(4, 0);
			THEN("The octagon is separated out to the right.") {
				REQUIRE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
				Coordinate2D expected_norm(Coordinate2D(1, 0));
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1));
			}
		}
		// Check some non-overlapping cases.
		WHEN("The octagon is only touching the left side.") {
			Coordinate2D pos1(-2, 0);
			THEN("Touching shapes are not overlapping.")
				CHECK_FALSE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
		}
		WHEN("The octagon is only touching the right side.") {
			Coordinate2D pos1(5, 0);
			THEN("They are not overlapping.")
				CHECK_FALSE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
		}
		WHEN("They are significantly apart.") {
			Coordinate2D pos1(20, 50);
			THEN("They do not overlap.")
				CHECK_FALSE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
		}
		WHEN("The octagon is only touching the top.") {
			Coordinate2D pos1(1, -4);
			THEN("They are not overlapping.")
				CHECK_FALSE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
		}
		WHEN("The octagon is edged out of the top left.") {
			Coordinate2D pos1(-0.66f, -3.9f);
			THEN("They are not overlapping.")
				CHECK_FALSE(sat::performSAT(p, pos1, o, pos2, out_norm, out_dist));
		}
	}
}

SCENARIO("One shape is moving to collide with a stationary one, detected with hybrid SAT.", "[sat][hybrid_sat]") {
	Fraction out_t;
	Coordinate2D out_norm;
	GIVEN("The shapes will collide vertex to vertex.") {
		Polygon collider(shapes::rightTri);
		// What normal to use is not particularily clear with these cases. Should the collider stop, or deflect off a side?
		WHEN("The collider moves towards a shape it is already touching.") {
			GIVEN("The stationary shape is a triangle.") {
				Polygon stationary(shapes::tri);
				THEN("It should collide immediately and not move at all.") {
					REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), Coordinate2D(0, -10), stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
					REQUIRE(out_t == ApproxEps(0));
				}
			}
			GIVEN("The stationary shape is a rectangle.") {
				Rectangle stationary(-1, 0, 1, 1);
				THEN("It should collide immediately and not move at all.") {
					REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), Coordinate2D(-10, 0), stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
					REQUIRE(out_t == ApproxEps(0));
				}
			}
		}
		WHEN("The collider moves towards a distant shape.") {
			GIVEN("The stationary shape is an octagon.") {
				Polygon stationary = Polygon(shapes::octagon);
				Coordinate2D stationaryPos(10, 0);
				THEN("The collider should move the distance between them.") {
					REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), Coordinate2D(10, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
					REQUIRE(out_t == ApproxEps(0.7f));
				}
			}
			GIVEN("The stationary shape is a triangle.") {
				Polygon stationary = Polygon(shapes::tri);
				Coordinate2D stationaryPos(3, -4);
				THEN("The collider should move the distance between them.") {
					REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), Coordinate2D(6, -8), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
					REQUIRE(out_t == ApproxEps(0.5f));
				}
			}
		}
	}
	GIVEN("The shapes will collide vertex to edge.") {
		Polygon collider(shapes::rightTri);
		WHEN("The collider moves towards a shape it is already touching.") {
			GIVEN("A vertex on the collider is moving into a edge of a triangle immediately above and to the right of it.") {
				Polygon stationary(shapes::tri);
				Coordinate2D stationaryPos(1, 1);
				Coordinate2D expected_norm((stationary[1] - stationary[0]).perpCCW().normalize());
				WHEN("Moving up into the triangle.") {
					Coordinate2D delta(0, -10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving right into the triangle.") {
					Coordinate2D delta(10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving up-right into the triangle.") {
					Coordinate2D delta(10, -10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
			}
			GIVEN("A vertex on the collider is moving into a edge of a rectangle immediately to the left of it.") {
				Rectangle stationary(0, 0, 1, 2);
				Coordinate2D stationaryPos(-1, -1);
				Coordinate2D expected_norm(1, 0);
				WHEN("Moving left into the rectangle.") {
					Coordinate2D delta(-10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-left into the rectangle.") {
					Coordinate2D delta(-10, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving up-left into the rectangle.") {
					Coordinate2D delta(-1, -100);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
			}
		}
		WHEN("The collider moves towards a distant shape.") {
			GIVEN("The stationary shape is a rectangle to the left of the collider.") {
				Rectangle stationary(-10, -1, 1, 2);
				WHEN("Moving left into the rectangle.") {
					Coordinate2D expected_norm(1, 0);
					Coordinate2D delta(-10, 0);
					THEN("The collider should move the distance between them.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0.9f));
					}
				}
			}
			GIVEN("The stationary shape is a triangle above and to the right of the collider.") {
				Polygon stationary(shapes::tri);
				Coordinate2D stationaryPos(4, -3);
				WHEN("Moving up-right into a triangle.") {
					Coordinate2D expected_norm((stationary[1] - stationary[0]).perpCCW().normalize());
					Coordinate2D delta(6, -8);
					THEN("The collider should move the distance between them.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0.5f));
					}
				}
			}
		}
	}
	GIVEN("The shapes will collide edge to vertex.") {
		Polygon collider(shapes::rightTri);
		WHEN("The collider moves towards a shape it is already touching.") {
			GIVEN("An edge on the collider is moving into a vertex of a triangle immediately above and to the left of it.") {
				Polygon stationary(shapes::tri);
				Coordinate2D stationaryPos(-2.5f, 1.5f);
				Coordinate2D expected_norm((collider[1] - collider[0]).perpCW().normalize());
				WHEN("Moving left into the triangle.") {
					Coordinate2D delta(-10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down into the triangle.") {
					Coordinate2D delta(0, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-left into the triangle.") {
					Coordinate2D delta(-10, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
			}
			GIVEN("An edge on the collider is moving into a vertex of an octagon immediately to the right of it.") {
				Polygon stationary(shapes::octagon);
				Coordinate2D stationaryPos(3, 0.5f);
				Coordinate2D expected_norm((collider[2] - collider[1]).perpCW().normalize());
				WHEN("Moving right into the octagon.") {
					Coordinate2D delta(10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-right into the octagon.") {
					Coordinate2D delta(10, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving up-right into the octagon.") {
					Coordinate2D delta(1, -100);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
			}
		}
		WHEN("The collider moves towards a distant shape.") {
			GIVEN("The stationary shape is a triangle down and to the left of the collider.") {
				Polygon stationary(shapes::tri);
				Coordinate2D stationaryPos(-5.5f, 5.5f);
				WHEN("Moving down-left into a triangle.") {
					Coordinate2D expected_norm((collider[1] - collider[0]).perpCW().normalize());
					Coordinate2D delta(-6, 8);
					THEN("The collider should move the distance between them.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0.5f));
					}
				}
			}
		}
	}
	GIVEN("The shapes will collide edge to edge.") {
		WHEN("The collider moves towards a shape it is already touching.") {
			GIVEN("An edge on the collider is moving into an edge of a rectangle immediately to the right of it.") {
				Polygon collider(shapes::rightTri);
				Rectangle stationary(1, 0, 1, 1);
				Coordinate2D expected_norm(-1, 0);
				WHEN("Moving right into the edge.") {
					Coordinate2D delta(10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-right into the edge.") {
					Coordinate2D delta(10, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving up-right into the edge.") {
					Coordinate2D delta(1, -100);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
			}
			GIVEN("An edge on the collider is moving into a triangle with a matching edge immediately down and to the left of it.") {
				Polygon collider(shapes::tri);
				Polygon stationary(shapes::edgeTri);
				Coordinate2D expected_norm((stationary[0] - stationary[2]).perpCCW().normalize());
				WHEN("Moving left into the edge.") {
					Coordinate2D delta(-10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down into the edge.") {
					Coordinate2D delta(0, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-left into the edge.") {
					Coordinate2D delta(-1, 100);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0));
					}
				}
			}
		}
		WHEN("The collider moves towards a distant shape.") {
			GIVEN("The stationary shape is a rectangle below and to the left of the collider.") {
				Rectangle collider(0, 0, 1, 1);
				Rectangle stationary(-2, 2, 1, 2);
				WHEN("The collider moves down-left.") {
					Coordinate2D delta(-10, 10);
					Coordinate2D expected_norm(1, 0);
					THEN("The collider should move the distance between them.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0.1f));
					}
				}
			}
			GIVEN("The stationary shape is a rectangle to the right of the collider.") {
				Polygon collider(shapes::rightTri);
				Rectangle stationary(5, 0, 1, 10);
				Coordinate2D expected_norm(-1, 0);
				WHEN("Moving right into the edge.") {
					Coordinate2D delta(10, 0);
					THEN("The collider should move the distance between them.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0.4f));
					}
				}
				WHEN("Moving down-right into the edge.") {
					Coordinate2D delta(8, 6);
					THEN("The collider should move the distance between them.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0.5f));
					}
				}
			}
			GIVEN("The stationary shape is a triangle down and to the left of the collider.") {
				Polygon collider(shapes::tri);
				Polygon stationary(shapes::edgeTri);
				Coordinate2D stationaryPos(-4, 3);
				Coordinate2D expected_norm((stationary[0] - stationary[2]).perpCCW().normalize());
				WHEN("Moving down-left into the triangle.") {
					Coordinate2D delta(-8, 6);
					THEN("The collider should move the distance between them.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0.5f));
					}
				}
			}
		}
	}
	GIVEN("Two shapes with fine edges.") {
		Polygon collider(std::vector<Coordinate2D>{Coordinate2D(0, 0), Coordinate2D(0, 1), Coordinate2D(1000, 0)});
		Polygon stationary(std::vector<Coordinate2D>{Coordinate2D(0, 0), Coordinate2D(-1000, 0), Coordinate2D(0, 1)});
		WHEN("The edges slightly move through each other.") {
			Coordinate2D stationaryPos(1999.9f, 1);
			Coordinate2D expected_norm((collider[2] - collider[1]).perpCW().normalize());
			Coordinate2D delta(0, 1);
			THEN("The collider should move the distance between them.") {
				REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(1 - ((1.0 / 1000) * 0.1))); // very slightly less than 1
			}
		}
	}
	GIVEN("Both the collider and stationary shape are given a position.") {
		GIVEN("The collider is a triangle, and the stationary shape an octagon.") {
			Polygon collider(shapes::rightTri);
			Polygon stationary(shapes::octagon);
			WHEN("They are touching and the collider moves into the ocatagon.") {
				Coordinate2D colliderPos(-2, -0.25f), stationaryPos(1, 0.25f);
				Coordinate2D delta(10, 0);
				Coordinate2D expected_norm((collider[2] - collider[1]).perpCW().normalize());
				THEN("It should collide immediately and not move at all.") {
					REQUIRE(sat::performHybridSAT(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					REQUIRE(out_t == ApproxEps(0));
				}
			}
			WHEN("They are a distance apart, and the collider moves into the octagon.") {
				Coordinate2D colliderPos(-0.5f, 20), stationaryPos(0, 10);
				Coordinate2D delta(0, -10);
				Coordinate2D expected_norm((collider[0] - collider[2]).perpCW().normalize());
				THEN("It should move the distance between them.") {
					REQUIRE(sat::performHybridSAT(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					REQUIRE(out_t == ApproxEps(0.8f));
				}
			}
		}
	}
}
SCENARIO("Two shapes are currently overlapping, detected with hybrid SAT.", "[poly][SAT][hybridSAT]") {
	Fraction out_t;
	Coordinate2D out_norm;
	GIVEN("Two rectangles.") {
		Rectangle collider(0, 0, 1, 1);
		Rectangle stationary(0, 0, 1, 1);
		WHEN("They perfectly overlap.") {
			Coordinate2D colliderPos(0, 0), stationaryPos(0, 0);
			THEN("They give the same MTV, regardless of the direction of movement (if any).") {
				// We don't know which normal will be chosen, as all axes are the MTV.
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				REQUIRE(out_t == ApproxEps(1));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, 10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				REQUIRE(out_t == ApproxEps(1));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, -90), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				REQUIRE(out_t == ApproxEps(1));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(0, 10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				REQUIRE(out_t == ApproxEps(1));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(0, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				REQUIRE(out_t == ApproxEps(1));
			}
		}
		WHEN("The collider slightly overlaps the stationary rectangle.") {
			Coordinate2D colliderPos(-0.99f, 0), stationaryPos(0, 0);
			Coordinate2D expected_norm(-1, 0);
			THEN("They give the same MTV, regardless of the direction of movement (if any).") {
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0.01f));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, 10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0.01f));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, -90), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0.01f));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(0, 10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0.01f));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(0, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0.01f));
			}
		}
	}
	GIVEN("An arbitrary shape and an octagon.") {
		Polygon collider(shapes::arb);
		Polygon stationary(shapes::octagon);
		WHEN("They have significant overlap.") {
			Coordinate2D colliderPos(-1.5f, -3), stationaryPos(0, 0);
			Coordinate2D expected_norm(0, -1);
			THEN("They give the same MTV, regardless of the direction of movement (if any).") {
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(1));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, 10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(1));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, -90), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(1));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(0, 10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(1));
				REQUIRE(sat::performHybridSAT(collider, colliderPos, Coordinate2D(0, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(1));
			}
		}
	}
}
SCENARIO("Two shapes are separated and will not collide, detected with hybrid SAT.", "[poly][SAT][hybridSAT]") {
	Fraction out_t;
	Coordinate2D out_norm;
	Polygon collider(shapes::tri);
	Polygon stationary(shapes::edgeTri);
	GIVEN("Two touching shapes.") {
		Coordinate2D colliderPos(0, 0), stationaryPos(0, 0);
		WHEN("They move away from each other.") {
			THEN("They will not collide.") {
				CHECK(sat::performHybridSAT(collider, colliderPos, Coordinate2D(0, -10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
				CHECK(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
				CHECK(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, -10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
			}
		}
		WHEN("They slide along an edge.") {
			Coordinate2D delta(10, 10);
			THEN("They will not collide.") {
				CHECK(sat::performHybridSAT(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
				CHECK(sat::performHybridSAT(collider, colliderPos, -delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
			}
		}
		WHEN("They are not moving.") {
			Coordinate2D delta(0, 0);
			THEN("They will not collide.")
				CHECK(sat::performHybridSAT(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
		}
	}
	GIVEN("Two shapes are not touching.") {
		Coordinate2D colliderPos(5, 2), stationaryPos(-5, 2);
		WHEN("They move away from each other.") {
			THEN("They will not collide.") {
				CHECK(sat::performHybridSAT(collider, colliderPos, Coordinate2D(0, -10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
				CHECK(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
				CHECK(sat::performHybridSAT(collider, colliderPos, Coordinate2D(10, -10), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
			}
		}
		WHEN("They move towards each other, but fall short of a collision.") {
			Coordinate2D delta(-5, 0);
			THEN("They will not collide.")
				CHECK(sat::performHybridSAT(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
		}
		WHEN("They move until they are touching, but not overlapping.") {
			Coordinate2D delta(-10, 0);
			THEN("They will not collide.")
				CHECK(sat::performHybridSAT(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
		}
		WHEN("They \"glance\" each other, but never overlap.") {
			Coordinate2D colliderPos(11, 12), stationaryPos(1, 2);
			Coordinate2D delta(-20, -20);
			THEN("They will not collide.")
				CHECK(sat::performHybridSAT(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
		}
		WHEN("They are not moving.") {
			Coordinate2D delta(0, 0);
			THEN("They will not collide.")
				CHECK(sat::performHybridSAT(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::NONE);
		}
	}
}

// I am abusing the fact that the function for them both moving uses almost entirely the same code
// as the function for one moving, and not testing nearly as many cases.
SCENARIO("Hybrid SAT with both shapes moving.", "[sat][hybrid_sat]") {
	Fraction out_t;
	Coordinate2D out_norm;
	GIVEN("Two touching shapes.") {
		Polygon p1(shapes::tri);
		Polygon p2(shapes::edgeTri);
		Coordinate2D pos1(0, 0), pos2(0, 0);
		Coordinate2D expected_norm((p2[0] - p2[2]).perpCCW().normalize());
		WHEN("They move away from each other.") {
			Coordinate2D p1Delta(10, 0), p2Delta(-10, 0);
			THEN("They will not collide.") {
				CHECK(sat::performHybridSAT(p1, pos1, Coordinate2D(10, 0), p2, pos2, Coordinate2D(-10, 0), out_norm, out_t) == sat::HybridResult::NONE);
				CHECK(sat::performHybridSAT(p1, pos1, Coordinate2D(0, -10), p2, pos2, Coordinate2D(0, 10), out_norm, out_t) == sat::HybridResult::NONE);
				CHECK(sat::performHybridSAT(p1, pos1, Coordinate2D(10, -10), p2, pos2, Coordinate2D(-10, 10), out_norm, out_t) == sat::HybridResult::NONE);
			}
		}
		WHEN("They move towards each other.") {
			THEN("They will collide.") {
				REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(-10, 0), p2, pos2, Coordinate2D(10, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0));
				REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(0, 10), p2, pos2, Coordinate2D(0, -10), out_norm, out_t) == sat::HybridResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0));
				REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(-10, 10), p2, pos2, Coordinate2D(10, -10), out_norm, out_t) == sat::HybridResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0));
			}
		}
		WHEN("They slide along an edge.") {
			Coordinate2D p1Delta(10, 10), p2Delta(-10, -10);
			THEN("They will not collide.")
				CHECK(sat::performHybridSAT(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == sat::HybridResult::NONE);
		}
		WHEN("They move at the same speed.") {
			Coordinate2D p1Delta(10, 0), p2Delta(10, 0);
			THEN("They will not collide.")
				CHECK(sat::performHybridSAT(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == sat::HybridResult::NONE);
		}
		WHEN("The delta of one overtakes the delta of the other.") {
			Coordinate2D p1Delta(10, 0), p2Delta(20, 0);
			THEN("They will collide effectively immediately.") {
				REQUIRE(sat::performHybridSAT(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == sat::HybridResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0));
			}
		}
		WHEN("They are not moving.") {
			Coordinate2D p1Delta(0, 0), p2Delta(0, 0);
			THEN("They will not collide.")
				CHECK(sat::performHybridSAT(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == sat::HybridResult::NONE);
		}
	}
	GIVEN("Two shapes a distance apart.") {
		Polygon p1(shapes::octagon);
		Rectangle p2(0, 0, 1, 1);
		Coordinate2D pos1(0, 0), pos2(10, -0.5f);
		WHEN("They move towards each other.") {
			Coordinate2D expected_norm(-1, 0);
			THEN("They will collide, the time determined by how fast they cover the distance between them.") {
				REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(10, 0), p2, pos2, Coordinate2D(-10, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0.4f));
				REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(10, 0), p2, pos2, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0.8f));
				REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(100, 0), p2, pos2, Coordinate2D(0, 1), out_norm, out_t) == sat::HybridResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0.08f));
				REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(100, 0), p2, pos2, Coordinate2D(20, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				REQUIRE(out_t == ApproxEps(0.1f));
			}
		}
		WHEN("They move until they touch, but don't overlap.") {
			Coordinate2D p1Delta(4, 0), p2Delta(-4, 0);
			THEN("They do not collide.")
				CHECK(sat::performHybridSAT(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == sat::HybridResult::NONE);
		}
	}
	GIVEN("Two overlapping shapes.") {
		Rectangle p1(0, 0, 10, 10), p2(0, 0, 1, 1);
		Coordinate2D pos1(0, 0), pos2(-0.5f, 0);
		Coordinate2D expected_norm(1, 0);
		THEN("They get the same MTV, regardless of deltas.") {
			REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(1000, 1), p2, pos2, Coordinate2D(1000, 1), out_norm, out_t) == sat::HybridResult::MTV);
			CHECK(out_norm.x == ApproxEps(expected_norm.x));
			CHECK(out_norm.y == ApproxEps(expected_norm.y));
			REQUIRE(out_t == ApproxEps(0.5f));
			REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(0, 0), p2, pos2, Coordinate2D(10, 10), out_norm, out_t) == sat::HybridResult::MTV);
			CHECK(out_norm.x == ApproxEps(expected_norm.x));
			CHECK(out_norm.y == ApproxEps(expected_norm.y));
			REQUIRE(out_t == ApproxEps(0.5f));
			REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(10, 10), p2, pos2, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::MTV);
			CHECK(out_norm.x == ApproxEps(expected_norm.x));
			CHECK(out_norm.y == ApproxEps(expected_norm.y));
			REQUIRE(out_t == ApproxEps(0.5f));
			REQUIRE(sat::performHybridSAT(p1, pos1, Coordinate2D(0, 0), p2, pos2, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::MTV);
			CHECK(out_norm.x == ApproxEps(expected_norm.x));
			CHECK(out_norm.y == ApproxEps(expected_norm.y));
			REQUIRE(out_t == ApproxEps(0.5f));
		}
	}
}

SCENARIO("Finding the separating axes for two shapes.", "[sat]") {
	GIVEN("Two shapes.") {
		GIVEN("An octagon and a triangle.") {
			Polygon p(shapes::octagon);
			Polygon o(shapes::tri);
			WHEN("They get their separating axes.") {
				std::vector<units::Coordinate2D> axes = sat::getSeparatingAxes(p, o);
				std::size_t expected = p.size() + o.size();
				THEN("It's the sum of the number of axes between them.")
					CHECK(axes.size() == expected);
			}
			WHEN("They get their separating axes in the opposite order.") {
				std::vector<units::Coordinate2D> axes = sat::getSeparatingAxes(o, p);
				std::size_t expected = p.size() + o.size();
				THEN("It's the sum of the number of axes between them.")
					CHECK(axes.size() == expected);
			}
		}
		GIVEN("Two different triangles.") {
			Polygon p(shapes::rightTri);
			Polygon o(shapes::tri);
			WHEN("They get their separating axes.") {
				std::vector<units::Coordinate2D> axes = sat::getSeparatingAxes(p, o);
				std::size_t expected = p.size() + o.size();
				THEN("It's the sum of the number of axes between them.")
					CHECK(axes.size() == expected);
			}
			WHEN("They get their separating axes in the opposite order.") {
				std::vector<units::Coordinate2D> axes = sat::getSeparatingAxes(o, p);
				std::size_t expected = p.size() + o.size();
				THEN("It's the sum of the number of axes between them.")
					CHECK(axes.size() == expected);
			}
		}
	}
	GIVEN("A shape and a rectangle.") {
		Polygon p(shapes::octagon);
		Rectangle r(0, 10, 1, 5);
		WHEN("They get their separating axes.") {
			std::vector<units::Coordinate2D> axes = sat::getSeparatingAxes(p, r);
			std::size_t expected = p.size() + separating_axes::RECT_NUM_AXES;
			THEN("It's the sum of the number of axes between them.")
				CHECK(axes.size() == expected);
		}
		WHEN("They get their separating axes in the opposite order.") {
			std::vector<units::Coordinate2D> axes = sat::getSeparatingAxes(r, p);
			std::size_t expected = p.size() + separating_axes::RECT_NUM_AXES;
			THEN("It's the sum of the number of axes between them.")
				CHECK(axes.size() == expected);
		}
	}
	GIVEN("Two rectangles.") {
		Rectangle p(50, -10, 600, 2);
		Rectangle r(0, 10, 1, 5);
		WHEN("They get their separating axes.") {
			std::vector<units::Coordinate2D> axes = sat::getSeparatingAxes(p, r);
			std::size_t expected = separating_axes::RECT_NUM_AXES;
			THEN("There is a special case with only two axes.")
				CHECK(axes.size() == expected);
		}
		WHEN("They get their separating axes in the opposite order.") {
			std::vector<units::Coordinate2D> axes = sat::getSeparatingAxes(r, p);
			std::size_t expected = separating_axes::RECT_NUM_AXES;
			THEN("There is a special case with only two axes.")
				CHECK(axes.size() == expected);
		}
	}
}