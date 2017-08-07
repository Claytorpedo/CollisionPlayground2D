#include "catch.hpp"

#include <vector>

#include "../Units.h"
#include "../Util.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/Polygon.h"

using namespace units;

TEST_CASE("Polygon translation.", "[poly]") {
	// Octagon.
	std::vector<Coordinate2D> points = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
		                                 Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
	Polygon p(points);
	CHECK(p.left()   == -2);
	CHECK(p.right()  ==  2);
	CHECK(p.top()    == -2);
	REQUIRE(p.bottom() == 2);
	p.translate(5, 0);
	for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coordinate2D(5, 0);
	for (std::size_t i = 0; i < p.size(); ++i)
		CHECK(util::almostEquals(p[i], points[i]));
	CHECK(p.left()   ==  3);
	CHECK(p.right()  ==  7);
	CHECK(p.top()    == -2);
	REQUIRE(p.bottom() == 2);
	p.translate(0, 5);
	for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coordinate2D(0, 5);
	for (std::size_t i = 0; i < p.size(); ++i)
		CHECK(util::almostEquals(p[i], points[i]));
	CHECK(p.left()   == 3);
	CHECK(p.right()  == 7);
	CHECK(p.top()    == 3);
	REQUIRE(p.bottom() == 7);
	p.translate(Coordinate2D(-10.5f, -12.5f));
	for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coordinate2D(-10.5f, -12.5f);
	for (std::size_t i = 0; i < p.size(); ++i)
		CHECK(util::almostEquals(p[i], points[i]));
	CHECK(p.left()   == -7.5f);
	CHECK(p.right()  == -3.5f);
	CHECK(p.top()    == -9.5f);
	REQUIRE(p.bottom() == -5.5f);
}

TEST_CASE("Polygon findExtendRange.", "[poly]") {
	SECTION("Extending a triangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0) };
		Polygon p(points);
		std::size_t first, last;
		bool should_dupe_first, should_dupe_last;
		p.findExtendRange(Coordinate2D(0, 1), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 0);
		CHECK(last == 2);
		CHECK(should_dupe_first);
		REQUIRE(should_dupe_last);
		p.findExtendRange(Coordinate2D(0, -1), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 2);
		CHECK(last == 0);
		CHECK(should_dupe_first);
		REQUIRE(should_dupe_last);
		p.findExtendRange(Coordinate2D(1, 0), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 1);
		CHECK(last == 2);
		CHECK(should_dupe_first);
		REQUIRE_FALSE(should_dupe_last);
		p.findExtendRange(Coordinate2D(-1, 0), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 0);
		CHECK(last == 1);
		CHECK_FALSE(should_dupe_first);
		REQUIRE(should_dupe_last);
		p.findExtendRange(Coordinate2D(1, -1).normalize(), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 2);
		CHECK(last == 0);
		CHECK_FALSE(should_dupe_first);
		REQUIRE(should_dupe_last);
	}
	SECTION("Extending an octagon.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                 Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		Polygon p(points);
		std::size_t first, last;
		bool should_dupe_first, should_dupe_last;
		p.findExtendRange(Coordinate2D(0, 1), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 6);
		CHECK(last == 2);
		CHECK(should_dupe_first);
		REQUIRE(should_dupe_last);
		p.findExtendRange(Coordinate2D(0, -1), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 2);
		CHECK(last == 6);
		CHECK(should_dupe_first);
		REQUIRE(should_dupe_last);
		p.findExtendRange(Coordinate2D(1, 0), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 0);
		CHECK(last == 4);
		CHECK(should_dupe_first);
		REQUIRE(should_dupe_last);
		p.findExtendRange(Coordinate2D(-1, 0), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 4);
		CHECK(last == 0);
		CHECK(should_dupe_first);
		REQUIRE(should_dupe_last);
		p.findExtendRange(Coordinate2D(1, 1).normalize(), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 7);
		CHECK(last == 3);
		CHECK(should_dupe_first);
		REQUIRE(should_dupe_last);
		p.findExtendRange(Coordinate2D(-1.5f, 0.5f).normalize(), first, last, should_dupe_first, should_dupe_last);
		CHECK(first == 5);
		CHECK(last == 0);
		CHECK_FALSE(should_dupe_first);
		REQUIRE_FALSE(should_dupe_last);
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

TEST_CASE("Polygon extend.", "[poly]") {
	SECTION("Extending a triangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0) };
		Polygon p(points);
		Polygon t = p.extend(Coordinate2D(0, 1), 5);
		std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(0, 5), Coordinate2D(1, 6), Coordinate2D(2, 5), Coordinate2D(2, 0) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.extend(Coordinate2D(0, -1), 5); 
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0), Coordinate2D(2, -5), Coordinate2D(0, -5) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.extend(Coordinate2D(1, 0), 10);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(11, 1), Coordinate2D(12, 0) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.extend(Coordinate2D(-1, 0), 10);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(-10,0), Coordinate2D(-9, 1), Coordinate2D(1, 1), Coordinate2D(2, 0) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.extend(Coordinate2D(1, -1).normalize(), 10);
		Coordinate2D delta = Coordinate2D(1, -1).normalize() * 10;
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0) + delta, Coordinate2D(0, 0) + delta };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
	}
	SECTION("Extending an octagon.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                 Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		Polygon p(points);
		Polygon t = p.extend(Coordinate2D(0, 1), 5);
		std::vector<Coordinate2D> extendSet = { Coordinate2D(0,7), Coordinate2D(1.5f,6.5f), Coordinate2D(2,5), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                    Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-2,5), Coordinate2D(-1.5f,6.5f) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.extend(Coordinate2D(0, -1), 5);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(2,-5), Coordinate2D(1.5f,-6.5f),
			                                   Coordinate2D(0,-7), Coordinate2D(-1.5f,-6.5f), Coordinate2D(-2,-5), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.extend(Coordinate2D(1, 0), 10);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,2), Coordinate2D(10,2), Coordinate2D(11.5f,1.5f), Coordinate2D(12,0), Coordinate2D(11.5f,-1.5f),
			                                   Coordinate2D(10,-2), Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.extend(Coordinate2D(-1, 0), 10);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(-10, 2), Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			                                   Coordinate2D(0,-2), Coordinate2D(-10, -2), Coordinate2D(-11.5f,-1.5f), Coordinate2D(-12,0), Coordinate2D(-11.5f,1.5f) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.extend(Coordinate2D(-1.5f, 0.5f).normalize(), 10);
		Coordinate2D delta = Coordinate2D(-1.5f, 0.5f).normalize() * 10;
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,2) + delta, Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
											   Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f) + delta, Coordinate2D(-2,0) + delta , Coordinate2D(-1.5f,1.5f) + delta };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
	}
}

TEST_CASE("Polygon clipExtend.", "[poly]") {
	SECTION("Clip-extending a triangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1, 1), Coordinate2D(2, 0) };
		Polygon p(points);
		Polygon t = p.clipExtend(Coordinate2D(0, 1), 5);
		std::vector<Coordinate2D> extendSet = { Coordinate2D(0,0), Coordinate2D(0, 5), Coordinate2D(1, 6), Coordinate2D(2, 5), Coordinate2D(2, 0) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.clipExtend(Coordinate2D(0, -1), 5);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,0), Coordinate2D(2, 0), Coordinate2D(2, -5), Coordinate2D(0, -5) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.clipExtend(Coordinate2D(1, 0), 10);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(1, 1), Coordinate2D(11, 1), Coordinate2D(12, 0), Coordinate2D(2,0) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.clipExtend(Coordinate2D(-1, 0), 10);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(-10,0), Coordinate2D(-9, 1), Coordinate2D(1, 1), Coordinate2D(0, 0) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.clipExtend(Coordinate2D(1, -1).normalize(), 10);
		Coordinate2D delta = Coordinate2D(1, -1).normalize() * 10;
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,0), Coordinate2D(2, 0), Coordinate2D(2, 0) + delta, Coordinate2D(0, 0) + delta };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
	}
	SECTION("Clip-extending an octagon.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
			Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
		Polygon p(points);
		Polygon t = p.clipExtend(Coordinate2D(0, 1), 5);
		std::vector<Coordinate2D> extendSet = { Coordinate2D(0,7), Coordinate2D(1.5f,6.5f), Coordinate2D(2,5), Coordinate2D(2,0), 
			                                    Coordinate2D(-2,0), Coordinate2D(-2,5), Coordinate2D(-1.5f,6.5f) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.clipExtend(Coordinate2D(0, -1), 5);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(2,0), Coordinate2D(2,-5), Coordinate2D(1.5f,-6.5f),
			                                   Coordinate2D(0,-7), Coordinate2D(-1.5f,-6.5f), Coordinate2D(-2,-5), Coordinate2D(-2,0) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.clipExtend(Coordinate2D(1, 0), 10);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,2), Coordinate2D(10,2), Coordinate2D(11.5f,1.5f), Coordinate2D(12,0), Coordinate2D(11.5f,-1.5f),
			                                   Coordinate2D(10,-2), Coordinate2D(0,-2) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.clipExtend(Coordinate2D(-1, 0), 10);
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(-10, 2), Coordinate2D(0,2),
			                                   Coordinate2D(0,-2), Coordinate2D(-10, -2), Coordinate2D(-11.5f,-1.5f), Coordinate2D(-12,0), Coordinate2D(-11.5f,1.5f) };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
		t = p.clipExtend(Coordinate2D(-1.5f, 0.5f).normalize(), 10);
		Coordinate2D delta = Coordinate2D(-1.5f, 0.5f).normalize() * 10;
		extendSet = std::vector<Coordinate2D>{ Coordinate2D(0,2) + delta, Coordinate2D(0,2),
			                                   Coordinate2D(-1.5f,-1.5f), Coordinate2D(-1.5f,-1.5f) + delta, Coordinate2D(-2,0) + delta , Coordinate2D(-1.5f,1.5f) + delta };
		REQUIRE(_polygons_equal(t, Polygon(extendSet)));
	}
}