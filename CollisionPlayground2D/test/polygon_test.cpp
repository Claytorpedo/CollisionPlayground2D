#include "catch.hpp"

#include <vector>

#include "../Units.h"
#include "../Util.h"
#include "../Constants.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/Polygon.h"

using namespace units;

SCENARIO("Translate a polygon.", "[poly]") {
	GIVEN("An octogon.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                 Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		Polygon oct(points);
		THEN("Its bounding box is created from the min/max x and y coordinates.") {
			CHECK(oct.left()   == -2);
			CHECK(oct.right()  ==  2);
			CHECK(oct.top()    == -2);
			CHECK(oct.bottom() ==  2);
		}
		WHEN("The polygon is translated 5 units left.") {
			oct.translate(5, 0);
			THEN("Its bounding box and all its points are translated 5 units left.") {
				CHECK(oct.left()   ==  3);
				CHECK(oct.right()  ==  7);
				CHECK(oct.top()    == -2);
				CHECK(oct.bottom() ==  2);
				for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coordinate2D(5, 0);
				for (std::size_t i = 0; i < oct.size(); ++i) {
					CHECK(oct[i].x == Approx(points[i].x).margin(constants::EPSILON));
					CHECK(oct[i].y == Approx(points[i].y).margin(constants::EPSILON));
				}
				AND_WHEN("The polygon is now translated 5 units down.") {
					oct.translate(0, 5);
					THEN("Its boundinb box and all its points are also translated 5 units down.") {
						for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coordinate2D(0, 5);
						for (std::size_t i = 0; i < oct.size(); ++i) {
							CHECK(oct[i].x == Approx(points[i].x).margin(constants::EPSILON));
							CHECK(oct[i].y == Approx(points[i].y).margin(constants::EPSILON));
						}
						CHECK(oct.left()   == 3);
						CHECK(oct.right()  == 7);
						CHECK(oct.top()    == 3);
						CHECK(oct.bottom() == 7);
					}
				}
			}
		}
		WHEN("The polygon is translated diagonally by floating point numbers.") {
			oct.translate(Coordinate2D(-10.5f, -12.5f));
			THEN("Its bounding box and all its points are translated diagonally by the same amount.") {
				for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coordinate2D(-10.5f, -12.5f);
				for (std::size_t i = 0; i < oct.size(); ++i) {
					CHECK(oct[i].x == Approx(points[i].x).margin(constants::EPSILON));
					CHECK(oct[i].y == Approx(points[i].y).margin(constants::EPSILON));
				}
				CHECK(oct.left()   == -12.5f);
				CHECK(oct.right()  == -8.5f);
				CHECK(oct.top()    == -14.5f);
				CHECK(oct.bottom() == -10.5f);
			}
		}
	}
}

SCENARIO("Finding the points to extend a polygon from in a given direction.", "[poly]") {
	std::size_t first, last;
	bool should_dupe_first, should_dupe_last;
	GIVEN("A triangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0) };
		Polygon tri(points);
		WHEN("Extending down.") {
			tri.findExtendRange(Coordinate2D(0, 1), first, last, should_dupe_first, should_dupe_last);
			CHECK(first == 0);
			CHECK(last == 2);
			CHECK(should_dupe_first);
			REQUIRE(should_dupe_last);
		}
		WHEN("Extending up.") {
			tri.findExtendRange(Coordinate2D(0, -1), first, last, should_dupe_first, should_dupe_last);
			CHECK(first == 2);
			CHECK(last == 0);
			CHECK(should_dupe_first);
			REQUIRE(should_dupe_last);
		}
		WHEN("Extending right.") {
			tri.findExtendRange(Coordinate2D(1, 0), first, last, should_dupe_first, should_dupe_last);
			CHECK(first == 1);
			CHECK(last == 2);
			CHECK(should_dupe_first);
			REQUIRE_FALSE(should_dupe_last);
		}
		WHEN("Extending left.") {
			tri.findExtendRange(Coordinate2D(-1, 0), first, last, should_dupe_first, should_dupe_last);
			CHECK(first == 0);
			CHECK(last == 1);
			CHECK_FALSE(should_dupe_first);
			REQUIRE(should_dupe_last);
		}
		WHEN("Extending diagonally.") {
			tri.findExtendRange(Coordinate2D(1, -1).normalize(), first, last, should_dupe_first, should_dupe_last);
			CHECK(first == 2);
			CHECK(last == 0);
			CHECK_FALSE(should_dupe_first);
			REQUIRE(should_dupe_last);
		}
	}
	GIVEN("An octagon.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                 Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		Polygon oct(points);
		WHEN("Extending down.") {
			oct.findExtendRange(Coordinate2D(0, 1), first, last, should_dupe_first, should_dupe_last);
			CHECK(first == 6);
			CHECK(last == 2);
			CHECK(should_dupe_first);
			REQUIRE(should_dupe_last);
		}
		WHEN("Extending up.") {
			oct.findExtendRange(Coordinate2D(0, -1), first, last, should_dupe_first, should_dupe_last);
			CHECK(first == 2);
			CHECK(last == 6);
			CHECK(should_dupe_first);
			REQUIRE(should_dupe_last);
		}
		WHEN("Extending right.") {
			oct.findExtendRange(Coordinate2D(1, 0), first, last, should_dupe_first, should_dupe_last);
			CHECK(first == 0);
			CHECK(last == 4);
			CHECK(should_dupe_first);
			REQUIRE(should_dupe_last);
		}
		WHEN("Extending left.") {
			oct.findExtendRange(Coordinate2D(-1, 0), first, last, should_dupe_first, should_dupe_last);
			CHECK(first == 4);
			CHECK(last == 0);
			CHECK(should_dupe_first);
			REQUIRE(should_dupe_last);
		}
		WHEN("Extending diagonally.") {
			THEN("Extending non-parallel to edges requires vertex duplication.") {
				oct.findExtendRange(Coordinate2D(1, 1).normalize(), first, last, should_dupe_first, should_dupe_last);
				CHECK(first == 7);
				CHECK(last == 3);
				CHECK(should_dupe_first);
				REQUIRE(should_dupe_last);
			}
			THEN("Extending parallel to edges avoids vertex duplication.") {
				oct.findExtendRange(Coordinate2D(-1.5f, 0.5f).normalize(), first, last, should_dupe_first, should_dupe_last);
				CHECK(first == 5);
				CHECK(last == 0);
				CHECK_FALSE(should_dupe_first);
				REQUIRE_FALSE(should_dupe_last);
			}
		}
	}
}

// The order of vertices in extended polygons isn't necessarily preserved.
// This tests to see if two polygons contain equivalent vertices, and assumes
// they are both wound counter-clockwise.
bool _polygons_equal(Polygon p, Polygon o) {
	if (p.size() != o.size()) {
		WARN("Error: Polygon p and Polygon o have different numbers of vertices (" << p.size() << " vs " << o.size() << ").");
		return false;
	}
	for (std::size_t i = 0; i < p.size(); ++i) {
		bool found = false;
		for (std::size_t k = 0; k < o.size(); ++k) {
			if (util::almostEquals(p[i], o[k])) {
				found = true;
				break;
			}
		}
		if (!found) {
			WARN("Error: Polygon o does not contain the vertex (" << p[i].x << ", " << p[i].y << ").");
			printf("Polygon p:\n");
			for (std::size_t j = 0; j < p.size(); ++j) {
				printf("P%ld: (%f, %f)\n", j, p[j].x, p[j].y);
			}
			return false;
		}
	}
	return true;
}

SCENARIO("Extending a polygon.", "[poly]") {
	GIVEN("A triangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0) };
		Polygon tri(points);
		WHEN("Extended downwards.") {
			Polygon t = tri.extend(Coordinate2D(0, 1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(0, 5), Coordinate2D(1, 6), Coordinate2D(2, 5), Coordinate2D(2, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Extended upwards.") {
			Polygon t = tri.extend(Coordinate2D(0, -1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0), Coordinate2D(2, -5), Coordinate2D(0, -5) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Extended rightwards, parallel to one side of the triangle.") {
			Polygon t = tri.extend(Coordinate2D(1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(11, 1), Coordinate2D(12, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Extended leftwards, parallel to one side of the triangle.") {
			Polygon t = tri.extend(Coordinate2D(-1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(-10,0), Coordinate2D(-9, 1), Coordinate2D(1, 1), Coordinate2D(2, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Extended diagonally, parallel to one side of the triangle.") {
			Polygon t = tri.extend(Coordinate2D(1, -1).normalize(), 10);
			const Coordinate2D delta = Coordinate2D(1, -1).normalize() * 10;
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0) + delta, Coordinate2D(0, 0) + delta };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
	}
	GIVEN("An octagon.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                 Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		Polygon oct(points);
		WHEN("Extended downwards.") {
			Polygon t = oct.extend(Coordinate2D(0, 1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,7), Coordinate2D(1.5f,6.5f), Coordinate2D(2,5), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				                                    Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-2,5), Coordinate2D(-1.5f,6.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Extended upwards.") {
			Polygon t = oct.extend(Coordinate2D(0, -1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(2,-5), Coordinate2D(1.5f,-6.5f),
				                                    Coordinate2D(0,-7), Coordinate2D(-1.5f,-6.5f), Coordinate2D(-2,-5), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Extended leftwards.") {
			Polygon t = oct.extend(Coordinate2D(1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2), Coordinate2D(10,2), Coordinate2D(11.5f,1.5f), Coordinate2D(12,0), Coordinate2D(11.5f,-1.5f),
				                                    Coordinate2D(10,-2), Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Extended rightwards.") {
			Polygon t = oct.extend(Coordinate2D(-1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(-10, 2), Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				                                    Coordinate2D(0,-2), Coordinate2D(-10, -2), Coordinate2D(-11.5f,-1.5f), Coordinate2D(-12,0), Coordinate2D(-11.5f,1.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Extended diagonally, parallel to two sides of the octogon.") {
			Polygon t = oct.extend(Coordinate2D(-1.5f, 0.5f).normalize(), 10);
			const Coordinate2D delta = Coordinate2D(-1.5f, 0.5f).normalize() * 10;
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2) + delta, Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				                                    Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f) + delta, Coordinate2D(-2,0) + delta , Coordinate2D(-1.5f,1.5f) + delta };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
	}
}

SCENARIO("A polygon is clip-extended: extended in a direction, and only the extended part is kept.", "[poly]") {
	GIVEN("A triangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0) };
		Polygon tri(points);
		WHEN("Clip-extended downwards.") {
			Polygon t = tri.clipExtend(Coordinate2D(0, 1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(0, 5), Coordinate2D(1, 6), Coordinate2D(2, 5), Coordinate2D(2, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Clip-extended upwards.") {
			Polygon t = tri.clipExtend(Coordinate2D(0, -1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(2, 0), Coordinate2D(2, -5), Coordinate2D(0, -5) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Clip-extended rightwards.") {
			Polygon t = tri.clipExtend(Coordinate2D(1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(1, 1), Coordinate2D(11, 1), Coordinate2D(12, 0), Coordinate2D(2,0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Clip-extended leftwards.") {
			Polygon t = tri.clipExtend(Coordinate2D(-1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(-10,0), Coordinate2D(-9, 1), Coordinate2D(1, 1), Coordinate2D(0, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Clip-extended diagonally.") {
			Polygon t = tri.clipExtend(Coordinate2D(1, -1).normalize(), 10);
			const Coordinate2D delta = Coordinate2D(1, -1).normalize() * 10;
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(2, 0), Coordinate2D(2, 0) + delta, Coordinate2D(0, 0) + delta };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
	}
	GIVEN("An octagon.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                 Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		Polygon oct(points);
		WHEN("Clip-extneded downwards.") {
			Polygon t = oct.clipExtend(Coordinate2D(0, 1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,7), Coordinate2D(1.5f,6.5f), Coordinate2D(2,5), Coordinate2D(2,0),
				                                    Coordinate2D(-2,0), Coordinate2D(-2,5), Coordinate2D(-1.5f,6.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Clip-extneded upwards.") {
			Polygon t = oct.clipExtend(Coordinate2D(0, -1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(2,0), Coordinate2D(2,-5), Coordinate2D(1.5f,-6.5f),
				                                    Coordinate2D(0,-7), Coordinate2D(-1.5f,-6.5f), Coordinate2D(-2,-5), Coordinate2D(-2,0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Clip-extneded rightwards.") {
			Polygon t = oct.clipExtend(Coordinate2D(1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2), Coordinate2D(10,2), Coordinate2D(11.5f,1.5f), Coordinate2D(12,0), Coordinate2D(11.5f,-1.5f),
				                                    Coordinate2D(10,-2), Coordinate2D(0,-2) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Clip-extneded leftwards.") {
			Polygon t = oct.clipExtend(Coordinate2D(-1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(-10, 2), Coordinate2D(0,2), Coordinate2D(0,-2),
				                                    Coordinate2D(-10, -2), Coordinate2D(-11.5f,-1.5f), Coordinate2D(-12,0), Coordinate2D(-11.5f,1.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
		WHEN("Clip-extneded diagonally.") {
			Polygon t = oct.clipExtend(Coordinate2D(-1.5f, 0.5f).normalize(), 10);
			Coordinate2D delta = Coordinate2D(-1.5f, 0.5f).normalize() * 10;
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2) + delta, Coordinate2D(0,2), Coordinate2D(-1.5f,-1.5f),
				                                    Coordinate2D(-1.5f,-1.5f) + delta, Coordinate2D(-2,0) + delta , Coordinate2D(-1.5f,1.5f) + delta };
			REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		}
	}
}