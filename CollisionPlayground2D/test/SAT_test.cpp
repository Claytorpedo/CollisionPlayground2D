#include "catch.hpp"
#include "definitions.h"

#include <vector>

#include "../Units.h"
#include "../Constants.h"
#include "../Geometry/SAT.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Rectangle.h"

using namespace units;

namespace { // Some shapes we'll use.
	const std::vector<Coordinate2D> rightTri = { Coordinate2D(0,0), Coordinate2D(1,1), Coordinate2D(1,0) };
	const std::vector<Coordinate2D> edgeTri = { Coordinate2D(0,0), Coordinate2D(0,1), Coordinate2D(1,1) };
	const std::vector<Coordinate2D> tri = { Coordinate2D(-1, -2), Coordinate2D(1, 0), Coordinate2D(3, -1) };
	const std::vector<Coordinate2D> edgetri = { Coordinate2D(-1, -2), Coordinate2D(-1, 0), Coordinate2D(1, 0) }; // shares edge with tri
	const std::vector<Coordinate2D> octagon = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
	                                            Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
	const std::vector<Coordinate2D> smallOct = { Coordinate2D(0,0.5f), Coordinate2D(0.2f,0.8f), Coordinate2D(0.5f,1), Coordinate2D(0.8f,0.8f),
	                                             Coordinate2D(1,0.5f), Coordinate2D(0.8f,0.2f), Coordinate2D(0.5f,0), Coordinate2D(0.2f,0.2f) };
	// An arbitrary polygon.
	const std::vector<Coordinate2D> arb = { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
}

SCENARIO("Testing two polygons for overlap.", "[poly][SAT]") {
	GIVEN("A triangle.") {
		Polygon p(rightTri);
		GIVEN("A second triangle.") {
			Polygon o(tri);
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
			Polygon o(rightTri);
			THEN("They overlap.")
				CHECK(sat::performSAT(p, o));
		}
		GIVEN("A triangle inside the first one.") {
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < rightTri.size(); ++i) inside.push_back(rightTri[i] * 0.5f);
			Polygon o(inside);
			THEN("They overlap.") {
				CHECK(sat::performSAT(p, o));
				o.translate(0.5f, 0.5f);
				CHECK(sat::performSAT(p, o));
			}
		}
		GIVEN("A triangle only sharing an edge.") {
			Polygon o(edgeTri);
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
		Polygon p(edgeTri);
		Polygon o(Rectangle(0, 0, 1, 1).toPoly());
		CHECK(sat::performSAT(p, o));
		o.translate(1, 0);
		CHECK_FALSE(sat::performSAT(p, o));
		o.translate(-0.5f, 1);
		CHECK_FALSE(sat::performSAT(p, o));
		o.translate(0, -0.1f);
		CHECK(sat::performSAT(p, o));
		o = Rectangle(0, 0, 2, 2).toPoly();
		CHECK(sat::performSAT(p, o));
		o.translate(-0.5f, -0.5f);
		CHECK(sat::performSAT(p, o));
		p.translate(0, -1.5f);
		CHECK_FALSE(sat::performSAT(p, o));
	}
	GIVEN("Convex polygons.") {
		GIVEN("An octagon and an arbitrary convex polygon.") {
			Polygon p(arb);
			Polygon o(octagon);
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
				THEN("Touching polygons are not overlapping.")
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
			Polygon p(octagon);
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < octagon.size(); ++i) inside.push_back(octagon[i] * 0.5f); // Half-size octagon.
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
}

SCENARIO("Testing two polygons for overlap with given positions.", "[poly][SAT]") {
	GIVEN("A triangle.") {
		Polygon p(rightTri);
		Coordinate2D pos1(0, 0);
		GIVEN("A second triangle.") {
			Polygon o(tri);
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
			Polygon o(rightTri);
			Coordinate2D pos2(0, 0);
			THEN("They overlap.")
				CHECK(sat::performSAT(p, pos1, o, pos2));
		}
		GIVEN("A triangle inside the first one.") {
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < rightTri.size(); ++i) inside.push_back(rightTri[i] * 0.5f);
			Polygon o(inside);
			Coordinate2D pos2(0, 0);
			THEN("They overlap.") {
				CHECK(sat::performSAT(p, pos1, o, pos2));
				pos2 = Coordinate2D(0.5f, 0.5f);
				CHECK(sat::performSAT(p, pos1, o, pos2));
			}
		}
		GIVEN("A triangle only sharing an edge.") {
			Polygon o(edgeTri);
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
		Polygon p(edgeTri);
		Coordinate2D pos1(0, 0);
		Polygon o(Rectangle(0, 0, 1, 1).toPoly());
		Coordinate2D pos2(0, 0);
		CHECK(sat::performSAT(p, pos1, o, pos2));
		pos1 = Coordinate2D(0.5f, 0);
		pos2 = Coordinate2D(1.5f, 0);
		CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
		pos2 = Coordinate2D(1, 1);
		CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
		pos2 = Coordinate2D(1, 0.9f);
		CHECK(sat::performSAT(p, pos1, o, pos2));
		o = Rectangle(0, 0, 2, 2).toPoly();
		pos1 = Coordinate2D(0, 0);
		pos2 = Coordinate2D(0, 0);
		CHECK(sat::performSAT(p, pos1, o, pos2));
		pos1 = Coordinate2D(0.5f, 0.5f);
		CHECK(sat::performSAT(p, pos1, o, pos2));
		pos1 = Coordinate2D(0.5f, -1);
		CHECK_FALSE(sat::performSAT(p, pos1, o, pos2));
	}
	GIVEN("Convex polygons.") {
		GIVEN("An octagon and an arbitrary convex polygon.") {
			Polygon p(arb);
			Coordinate2D pos1(0, 0);
			Polygon o(octagon);
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
				THEN("Touching polygons are not overlapping.")
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
			Polygon p(octagon);
			Coordinate2D pos1(0, 0);
			std::vector<Coordinate2D> inside;
			for (std::size_t i = 0; i < octagon.size(); ++i) inside.push_back(octagon[i] * 0.5f); // Half-size octagon.
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
}

SCENARIO("Two polygons are overlapping, and need to be separated (by the minimum translation vector).", "[poly][SAT]") {
	GIVEN("The polygon to be moved is a rectangle, and the stationary one is a triangle.") {
		Polygon p(Rectangle(0, 0, 1, 1).toPoly());
		Polygon o(rightTri);
		Coordinate2D pos1(0, 0), pos2(0, 0), out_norm;
		Coordinate out_dist;
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
		Polygon p(Rectangle(0, 0, 100, 100).toPoly());
		Polygon o(rightTri);
		Coordinate2D pos1(0, 0), pos2(0, 0), out_norm;
		Coordinate out_dist;
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
		Polygon p(rightTri);
		std::vector<Coordinate2D> large;
		for (std::size_t i = 0; i < rightTri.size(); ++i) large.push_back(rightTri[i] * 100.0f);
		Polygon o(large);
		Coordinate2D pos1(0, 0), pos2(0, 0), out_norm;
		Coordinate out_dist;
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
	GIVEN("An octagon and arbitrary convex polygon.") {
		Polygon p(octagon);
		Polygon o(arb);
		Coordinate2D pos1(0, 0), pos2(0, 0), out_norm;
		Coordinate out_dist;
		WHEN("They octagon overlaps the arbitrary polygon's left side.") {
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
			THEN("Touching polygons are not overlapping.")
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

SCENARIO("One polygon is moving to collide with a stationary one, detected with hybrid SAT.", "[poly][SAT][hybridSAT]") {
	Fraction out_t;
	Coordinate2D out_norm;
	GIVEN("The polygons will collide vertex to vertex.") {
		Polygon collider(rightTri);
		// What normal to use is not particularily clear with these cases. Should the collider stop, or deflect off a side?
		WHEN("The collider moves towards a polygon it is already touching.") {
			GIVEN("The stationary polygon is a triangle.") {
				Polygon stationary(tri);
				THEN("It should collide immediately and not move at all.") {
					REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), Coordinate2D(0, -10), stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
					REQUIRE(out_t == ApproxEps(0));
				}
			}
			GIVEN("The stationary polygon is a rectangle.") {
				Polygon stationary(Rectangle(-1, 0, 1, 1).toPoly());
				THEN("It should collide immediately and not move at all.") {
					REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), Coordinate2D(-10, 0), stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
					REQUIRE(out_t == ApproxEps(0));
				}
			}
		}
		WHEN("The collider moves towards a distant polygon.") {
			GIVEN("The stationary polygon is an octagon.") {
				Polygon stationary = Polygon(octagon);
				Coordinate2D stationaryPos(10, 0);
				THEN("The collider should move the distance between them.") {
					REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), Coordinate2D(10, 0), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
					REQUIRE(out_t == ApproxEps(0.7f));
				}
			}
			GIVEN("The stationary polygon is a triangle.") {
				Polygon stationary = Polygon(tri);
				Coordinate2D stationaryPos(3, -4);
				THEN("The collider should move the distance between them.") {
					REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), Coordinate2D(6, -8), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
					REQUIRE(out_t == ApproxEps(0.5f));
				}
			}
		}
	}
	GIVEN("The polygons will collide vertex to edge.") {
		Polygon collider(rightTri);
		WHEN("The collider moves towards a polygon it is already touching.") {
			GIVEN("A vertex on the collider is moving into a edge of a triangle immediately above and to the right of it.") {
				Polygon stationary(tri);
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
				Polygon stationary(Rectangle(0, 0, 1, 2).toPoly());
				Coordinate2D stationaryPos(-1, -1);
				Coordinate2D expected_norm((stationary[3] - stationary[2]).perpCCW().normalize());
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
		WHEN("The collider moves towards a distant polygon.") {
			GIVEN("The stationary polygon is a rectangle to the left of the collider.") {
				Polygon stationary(Rectangle(-10, -1, 1, 2).toPoly());
				WHEN("Moving left into the rectangle.") {
					Coordinate2D expected_norm((stationary[3] - stationary[2]).perpCCW().normalize());
					Coordinate2D delta(-10, 0);
					THEN("The collider should move the distance between them.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), delta, stationary, Coordinate2D(0, 0), out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0.9f));
					}
				}
			}
			GIVEN("The stationary polygon is a triangle above and to the right of the collider.") {
				Polygon stationary(tri);
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
	GIVEN("The polygons will collide edge to vertex.") {
		Polygon collider(rightTri);
		WHEN("The collider moves towards a polygon it is already touching.") {
			GIVEN("An edge on the collider is moving into a vertex of a triangle immediately above and to the left of it.") {
				Polygon stationary(tri);
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
				Polygon stationary(octagon);
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
		WHEN("The collider moves towards a distant polygon.") {
			GIVEN("The stationary polygon is a triangle down and to the left of the collider.") {
				Polygon stationary(tri);
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
	GIVEN("The polygons will collide edge to edge.") {
		WHEN("The collider moves towards a polygon it is already touching.") {
			GIVEN("An edge on the collider is moving into an edge of a rectangle immediately to the right of it.") {
				Polygon collider(rightTri);
				Polygon stationary(Rectangle(1, 0, 1, 1).toPoly());
				Coordinate2D expected_norm((stationary[1] - stationary[0]).perpCCW().normalize());
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
				Polygon collider(tri);
				Polygon stationary(edgetri);
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
		WHEN("The collider moves towards a distant polygon.") {
			GIVEN("The stationary polygon is a rectangle to the right of the collider.") {
				Polygon collider(rightTri);
				Polygon stationary(Rectangle(5, 0, 1, 10).toPoly());
				Coordinate2D expected_norm((stationary[1] - stationary[0]).perpCCW().normalize());
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
			GIVEN("The stationary polygon is a triangle down and to the left of the collider.") {
				Polygon collider(tri);
				Polygon stationary(edgetri);
				Coordinate2D stationaryPos(-4, 3);
				Coordinate2D expected_norm((stationary[0] - stationary[2]).perpCCW().normalize());
				WHEN("Moving down-left into the triangle.") {
					Coordinate2D delta(-8, 6);
					THEN("The collider should move the distance between them.") {
						REQUIRE(sat::performHybridSAT(collider, Coordinate2D(0, 0), Coordinate2D(-8, 6), stationary, stationaryPos, out_norm, out_t) == sat::HybridResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						REQUIRE(out_t == ApproxEps(0.5f));
					}
				}
			}
		}
	}
	GIVEN("Two polygons with fine edges.") {
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
	GIVEN("Both the collider and stationary polygon are given a position.") {
		GIVEN("The collider is a triangle, and the stationary polygon an octagon.") {
			Polygon collider(rightTri);
			Polygon stationary(octagon);
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
SCENARIO("Two polygons are currently overlapping, detected with hybrid SAT.", "[poly][SAT][hybridSAT]") {
	Fraction out_t;
	Coordinate2D out_norm;
	GIVEN("Two rectangles.") {
		Polygon collider(Rectangle(0, 0, 1, 1).toPoly());
		Polygon stationary(Rectangle(0, 0, 1, 1).toPoly());
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
	GIVEN("An arbitrary polygon and an octagon.") {
		Polygon collider(arb);
		Polygon stationary(octagon);
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
SCENARIO("Two polygons are separated and will not collide, detected with hybrid SAT.", "[poly][SAT][hybridSAT]") {
	Fraction out_t;
	Coordinate2D out_norm;
	Polygon collider(tri);
	Polygon stationary(edgetri);
	GIVEN("Two touching polygons.") {
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
	GIVEN("Two polygons are not touching.") {
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
SCENARIO("Hybrid SAT with both polygons moving.", "[poly][SAT][hybridSAT]") {
	Fraction out_t;
	Coordinate2D out_norm;
	GIVEN("Two touching polygons.") {
		Polygon p1(tri);
		Polygon p2(edgetri);
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
	GIVEN("Two polygons a distance apart.") {
		Polygon p1(octagon), p2(Rectangle(0, 0, 1, 1).toPoly());
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
	GIVEN("Two overlapping polygons.") {
		Polygon p1(Rectangle(0, 0, 10, 10).toPoly()), p2(Rectangle(0, 0, 1, 1).toPoly());
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