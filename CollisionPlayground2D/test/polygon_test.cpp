#include "catch.hpp"
#include "definitions.h"

#include <vector>

#include "../Geometry/Units.h"
#include "../Geometry/Util.h"
#include "../Geometry/Constants.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/Polygon.h"

using namespace units;

SCENARIO("Translate a polygon.", "[poly]") {
	GIVEN("An octogon.") {
		Polygon oct(shapes::octagon);
		std::vector<Coordinate2D> points(shapes::octagon);
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
					CHECK(oct[i].x == ApproxEps(points[i].x));
					CHECK(oct[i].y == ApproxEps(points[i].y));
				}
				AND_WHEN("The polygon is now translated 5 units down.") {
					oct.translate(0, 5);
					THEN("Its boundinb box and all its points are also translated 5 units down.") {
						for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coordinate2D(0, 5);
						for (std::size_t i = 0; i < oct.size(); ++i) {
							CHECK(oct[i].x == ApproxEps(points[i].x));
							CHECK(oct[i].y == ApproxEps(points[i].y));
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
					CHECK(oct[i].x == ApproxEps(points[i].x));
					CHECK(oct[i].y == ApproxEps(points[i].y));
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
		Polygon oct(shapes::octagon);
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

// Give a warning and some information for miss-matching polygons.
void warnPoly(Polygon p, std::size_t index) {
	WARN("Error: Polygon o does not contain the vertex (" << p[index].x << ", " << p[index].y << ").");
	printf("Polygon p:\n");
	for (std::size_t j = 0; j < p.size(); ++j) {
		printf("P%ld: (%f, %f)\n", j, p[j].x, p[j].y);
	}
}

// The order of vertices in extended polygons isn't necessarily preserved.
// This tests to see if two polygons contain equivalent vertices, and assumes
// they are both wound counter-clockwise (as they should be).
bool _polygons_equal(Polygon p, Polygon o) {
	if (p.size() != o.size()) {
		WARN("Error: Polygon p and Polygon o have different numbers of vertices (" << p.size() << " vs " << o.size() << ").");
		return false;
	}
	std::size_t offset;
	bool found = false;
	for (std::size_t i = 0; i < o.size(); ++i) {
		if (util::almostEquals(p[0], o[i])) {
			found = true;
			offset = i;
		}
	}
	if (!found) {
		warnPoly(p, 0);
		return false;
	}
	// Found our offset to use. Ordering should be the same with that offset.
	std::size_t second;
	for (std::size_t i = 1; i < p.size(); ++i) {
		second = i + offset >= o.size() ? (i + offset) - o.size() : i + offset;
		if ( !util::almostEquals(p[i], o[second]) ) {
			warnPoly(p, i);
			return false;
		}
	}
	// Test normals.
	for (std::size_t i = 0; i < p.size(); ++i) {
		second = i + offset >= o.size() ? (i + offset) - o.size() : i + offset;
		if (!util::almostEquals(p.getEdgeNorm(i), o.getEdgeNorm(second))) {
			WARN("Error: Polygon o does not contain the edge normal (" << p.getEdgeNorm(i).x << ", " << p.getEdgeNorm(i).y << ").");
			printf("Got (%f, %f)\n", o.getEdgeNorm(i).x, o.getEdgeNorm(i).y);
			printf("Polygon p:\n");
			for (std::size_t j = 0; j < p.size(); ++j)
				printf("P_norm%ld: (%f, %f)\n", j, p.getEdgeNorm(j).x, p.getEdgeNorm(j).y);
			printf("\nPolygon o with offset %zu:\n", offset);
			for (std::size_t j = 0; j < p.size(); ++j)
				printf("O_norm%ld: (%f, %f)\n", j, o.getEdgeNorm(j).x, o.getEdgeNorm(j).y);
			return false;
		}
	}
	return true;
}
std::vector<Coordinate2D> _get_normals(std::vector<Coordinate2D> extendSet) {
	std::vector<Coordinate2D> normals;
	normals.reserve(extendSet.size());
	for (std::size_t i = 0; i < extendSet.size(); ++i) {
		const units::Coordinate2D first = extendSet[i];
		const units::Coordinate2D second = extendSet[i + 1 >= extendSet.size() ? 0 : i + 1];
		normals.push_back(units::Coordinate2D(first.y - second.y, second.x - first.x).normalize());
	}
	return normals;
}

SCENARIO("A polygon computes its normals." "[poly]") {
	GIVEN("An octogon.") {
		Polygon oct(shapes::octagon);
		std::vector<Coordinate2D> normals(_get_normals(shapes::octagon));
		WHEN("We pre-compute the normals.") {
			oct.computeNormals();
			THEN("All of the edge normals are computed for a counter-clockwise wound polygon.") {
				for (std::size_t i = 0; i < oct.size(); ++i) {
					CHECK(oct.getEdgeNorm(i).x == ApproxEps(normals[i].x));
					CHECK(oct.getEdgeNorm(i).y == ApproxEps(normals[i].y));
				}
			}
		}
		WHEN("We ask for edge normals without pre-computing them.") {
			THEN("All of the edge normals are computed on the fly for a counter-clockwise wound polygon.") {
				for (std::size_t i = 0; i < oct.size(); ++i) {
					CHECK(oct.getEdgeNorm(i).x == ApproxEps(normals[i].x));
					CHECK(oct.getEdgeNorm(i).y == ApproxEps(normals[i].y));
				}
			}
		}
	}
}

SCENARIO("A polygon is expanded.", "[poly") {
	GIVEN("A rectangle.") {
		Polygon p(Rectangle(0, 0, 1, 1).toPoly());
		WHEN("It is scaled 2.5 units larger.") {
			units::Coordinate amt = 2.5f;
			units::Coordinate2D topLeft(p[0]     + units::Coordinate2D(-1,-1).normalize() * amt);
			units::Coordinate2D bottomLeft(p[1]  + units::Coordinate2D(-1, 1).normalize() * amt);
			units::Coordinate2D bottomRight(p[2] + units::Coordinate2D( 1, 1).normalize() * amt);
			units::Coordinate2D topRight(p[3]    + units::Coordinate2D( 1,-1).normalize() * amt);
			p.expand(amt);
			THEN("It becomes 2.5 units larger on all sides by its vertices.") {
				CHECK(p.left()   == ApproxEps(topLeft.x));
				CHECK(p.right()  == ApproxEps(topRight.x));
				CHECK(p.top()    == ApproxEps(topLeft.y));
				CHECK(p.bottom() == ApproxEps(bottomLeft.y));
				CHECK(p[0].x == ApproxEps(topLeft.x));
				CHECK(p[0].y == ApproxEps(topLeft.y));
				CHECK(p[1].x == ApproxEps(bottomLeft.x));
				CHECK(p[1].y == ApproxEps(bottomLeft.y));
				CHECK(p[2].x == ApproxEps(bottomRight.x));
				CHECK(p[2].y == ApproxEps(bottomRight.y));
				CHECK(p[3].x == ApproxEps(topRight.x));
				CHECK(p[3].y == ApproxEps(topRight.y));
			}
		}
	}
	GIVEN("An octagon.") {
		Polygon p(shapes::octagon);
		WHEN("It is expanded 3 units larger.") {
			units::Coordinate amt = 3;
			p.expand(amt);
			units::Coordinate2D left(-5,0);
			units::Coordinate2D right(5,0);
			units::Coordinate2D top(0,-5);
			units::Coordinate2D bottom(0,5);
			units::Coordinate2D bottomLeft( shapes::octagon[7] + units::Coordinate2D(-1, 1).normalize() * amt);
			units::Coordinate2D bottomRight(shapes::octagon[1] + units::Coordinate2D( 1, 1).normalize() * amt);
			units::Coordinate2D topRight(   shapes::octagon[3] + units::Coordinate2D( 1,-1).normalize() * amt);
			units::Coordinate2D topLeft(    shapes::octagon[5] + units::Coordinate2D(-1,-1).normalize() * amt);
			THEN("It becomes 3 units larger on all sides by its vertices.") {
				CHECK(p.left() == ApproxEps(-5));
				CHECK(p.right() == ApproxEps(5));
				CHECK(p.top() == ApproxEps(-5));
				CHECK(p.bottom() == ApproxEps(5));
				CHECK(p[0].x == ApproxEps(bottom.x));
				CHECK(p[0].y == ApproxEps(bottom.y));
				CHECK(p[1].x == ApproxEps(bottomRight.x));
				CHECK(p[1].y == ApproxEps(bottomRight.y));
				CHECK(p[2].x == ApproxEps(right.x));
				CHECK(p[2].y == ApproxEps(right.y));
				CHECK(p[3].x == ApproxEps(topRight.x));
				CHECK(p[3].y == ApproxEps(topRight.y));
				CHECK(p[4].x == ApproxEps(top.x));
				CHECK(p[4].y == ApproxEps(top.y));
				CHECK(p[5].x == ApproxEps(topLeft.x));
				CHECK(p[5].y == ApproxEps(topLeft.y));
				CHECK(p[6].x == ApproxEps(left.x));
				CHECK(p[6].y == ApproxEps(left.y));
				CHECK(p[7].x == ApproxEps(bottomLeft.x));
				CHECK(p[7].y == ApproxEps(bottomLeft.y));
			}
		}
	}
}

SCENARIO("Extending a polygon.", "[poly]") {
	GIVEN("A triangle.") {
		Polygon tri(shapes::isoTri);
		tri.computeNormals();
		WHEN("Extended downwards.") {
			Polygon t = tri.extend(Coordinate2D(0, 1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(0, 5), Coordinate2D(1, 6), Coordinate2D(2, 5), Coordinate2D(2, 0) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended upwards.") {
			Polygon t = tri.extend(Coordinate2D(0, -1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0), Coordinate2D(2, -5), Coordinate2D(0, -5) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended rightwards, parallel to one side of the triangle.") {
			Polygon t = tri.extend(Coordinate2D(1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(11, 1), Coordinate2D(12, 0) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended leftwards, parallel to one side of the triangle.") {
			Polygon t = tri.extend(Coordinate2D(-1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(-10,0), Coordinate2D(-9, 1), Coordinate2D(1, 1), Coordinate2D(2, 0) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended diagonally, parallel to one side of the triangle.") {
			Polygon t = tri.extend(Coordinate2D(1, -1).normalize(), 10);
			const Coordinate2D delta = Coordinate2D(1, -1).normalize() * 10;
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0) + delta, Coordinate2D(0, 0) + delta };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
	}
	GIVEN("An octagon.") {
		Polygon oct(shapes::octagon);
		oct.computeNormals();
		WHEN("Extended downwards.") {
			Polygon t = oct.extend(Coordinate2D(0, 1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,7), Coordinate2D(1.5f,6.5f), Coordinate2D(2,5), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				                                    Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-2,5), Coordinate2D(-1.5f,6.5f) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended upwards.") {
			Polygon t = oct.extend(Coordinate2D(0, -1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(2,-5), Coordinate2D(1.5f,-6.5f),
				                                    Coordinate2D(0,-7), Coordinate2D(-1.5f,-6.5f), Coordinate2D(-2,-5), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended leftwards.") {
			Polygon t = oct.extend(Coordinate2D(1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2), Coordinate2D(10,2), Coordinate2D(11.5f,1.5f), Coordinate2D(12,0), Coordinate2D(11.5f,-1.5f),
				                                    Coordinate2D(10,-2), Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended rightwards.") {
			Polygon t = oct.extend(Coordinate2D(-1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(-10, 2), Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				                                    Coordinate2D(0,-2), Coordinate2D(-10, -2), Coordinate2D(-11.5f,-1.5f), Coordinate2D(-12,0), Coordinate2D(-11.5f,1.5f) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended diagonally, parallel to two sides of the octogon.") {
			Polygon t = oct.extend(Coordinate2D(-1.5f, 0.5f).normalize(), 10);
			const Coordinate2D delta = Coordinate2D(-1.5f, 0.5f).normalize() * 10;
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2) + delta, Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				                                    Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f) + delta, Coordinate2D(-2,0) + delta , Coordinate2D(-1.5f,1.5f) + delta };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
	}
}

SCENARIO("A polygon is clip-extended: extended in a direction, and only the extended part is kept.", "[poly]") {
	GIVEN("A triangle.") {
		Polygon tri(shapes::isoTri);
		tri.computeNormals();
		WHEN("Clip-extended downwards.") {
			Polygon t = tri.clipExtend(Coordinate2D(0, 1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(0, 5), Coordinate2D(1, 6), Coordinate2D(2, 5), Coordinate2D(2, 0) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extended upwards.") {
			Polygon t = tri.clipExtend(Coordinate2D(0, -1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(2, 0), Coordinate2D(2, -5), Coordinate2D(0, -5) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extended rightwards.") {
			Polygon t = tri.clipExtend(Coordinate2D(1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(1, 1), Coordinate2D(11, 1), Coordinate2D(12, 0), Coordinate2D(2,0) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extended leftwards.") {
			Polygon t = tri.clipExtend(Coordinate2D(-1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(-10,0), Coordinate2D(-9, 1), Coordinate2D(1, 1), Coordinate2D(0, 0) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extended diagonally.") {
			Polygon t = tri.clipExtend(Coordinate2D(1, -1).normalize(), 10);
			const Coordinate2D delta = Coordinate2D(1, -1).normalize() * 10;
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(2, 0), Coordinate2D(2, 0) + delta, Coordinate2D(0, 0) + delta };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
	}
	GIVEN("An octagon.") {
		Polygon oct(shapes::octagon);
		oct.computeNormals();
		WHEN("Clip-extneded downwards.") {
			Polygon t = oct.clipExtend(Coordinate2D(0, 1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,7), Coordinate2D(1.5f,6.5f), Coordinate2D(2,5), Coordinate2D(2,0),
				                                    Coordinate2D(-2,0), Coordinate2D(-2,5), Coordinate2D(-1.5f,6.5f) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extneded upwards.") {
			Polygon t = oct.clipExtend(Coordinate2D(0, -1), 5);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(2,0), Coordinate2D(2,-5), Coordinate2D(1.5f,-6.5f),
				                                    Coordinate2D(0,-7), Coordinate2D(-1.5f,-6.5f), Coordinate2D(-2,-5), Coordinate2D(-2,0) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extneded rightwards.") {
			Polygon t = oct.clipExtend(Coordinate2D(1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2), Coordinate2D(10,2), Coordinate2D(11.5f,1.5f), Coordinate2D(12,0), Coordinate2D(11.5f,-1.5f),
				                                    Coordinate2D(10,-2), Coordinate2D(0,-2) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extneded leftwards.") {
			Polygon t = oct.clipExtend(Coordinate2D(-1, 0), 10);
			std::vector<Coordinate2D> extendSet = { Coordinate2D(-10, 2), Coordinate2D(0,2), Coordinate2D(0,-2),
				                                    Coordinate2D(-10, -2), Coordinate2D(-11.5f,-1.5f), Coordinate2D(-12,0), Coordinate2D(-11.5f,1.5f) };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extneded diagonally.") {
			Polygon t = oct.clipExtend(Coordinate2D(-1.5f, 0.5f).normalize(), 10);
			Coordinate2D delta = Coordinate2D(-1.5f, 0.5f).normalize() * 10;
			std::vector<Coordinate2D> extendSet = { Coordinate2D(0,2) + delta, Coordinate2D(0,2), Coordinate2D(-1.5f,-1.5f),
				                                    Coordinate2D(-1.5f,-1.5f) + delta, Coordinate2D(-2,0) + delta , Coordinate2D(-1.5f,1.5f) + delta };
			std::vector <Coordinate2D> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
	}
}