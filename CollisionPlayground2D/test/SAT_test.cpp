#include "catch.hpp"

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
	const std::vector<Coordinate2D> octagon = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
	                                            Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
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

#define ApproxEps(x) Approx(x).margin(constants::EPSILON)

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