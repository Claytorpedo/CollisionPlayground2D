#include "catch.hpp"
#include "definitions.h"

#include <vector>

#include "../Geometry/Collidable.h"
#include "../Geometry/CollisionMap.h"
#include "../Units.h"
#include "../Constants.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Rectangle.h"

using namespace units;

struct CollidableTest : public Collidable {
	CollidableTest(Collidable::CollisionType type) : Collidable(type) {}
	~CollidableTest() {}
};
struct CollisionMapTest : public CollisionMap {
	std::vector<Polygon> polys;
	CollisionMapTest(std::vector<Polygon> polys) : polys(polys) {}
	~CollisionMapTest() {}
	virtual std::vector<Polygon> getColliding(const Shape& collider, const units::Coordinate2D& position, const units::Coordinate2D& delta) const {
		return polys;
	}
};

SCENARIO("A collider deflects off a stationary polygon.", "[collidable][deflection]") {
	CollidableTest tester(Collidable::CollisionType::DEFLECTION);
	GIVEN("The collider is a right triangle.") {
		Polygon collider(shapes::rightTri);
		GIVEN("The stationary polygon is a rectangle.") {
			std::vector<Polygon> polys;
			polys.push_back(Rectangle(0, 0, 1, 1).toPoly());
			CollisionMapTest map(polys);
			WHEN("The collider misses the rectangle.") {
				Coordinate2D origin(0, -10);
				Coordinate2D delta(10, 0);
				Coordinate2D position = tester.move(origin, collider, delta, map);
				THEN("It moves the full delta vector.") {
					Coordinate2D expected_pos = origin + delta;
					CHECK(position.x == ApproxEps(expected_pos.x));
					CHECK(position.y == ApproxEps(expected_pos.y));
				}
			}
			WHEN("The collider hits the rectangle straight on.") {
				Coordinate2D origin(-0.5f, -5);
				Coordinate2D dir(0, 1);
				Coordinate dist(10);
				Coordinate2D position = tester.move(origin, collider, dir*dist, map);
				THEN("It moves to the rectangle and stops.") {
					Coordinate2D expected_pos(-0.5f, -1 - Collidable::getPushoutDistance(dir, Coordinate2D(0, -1)));
					CHECK(position.x == ApproxEps(expected_pos.x));
					CHECK(position.y == ApproxEps(expected_pos.y));
				}
			}
			WHEN("The collider hits the rectangle on the collider's edge.") {
				Coordinate2D origin(0.5f, -5);
				Coordinate2D dir(0, 1);
				Coordinate dist(10);
				Coordinate2D position = tester.move(origin, collider, dir*dist, map);
				THEN("It deflects off the rectangle using the collider's edge.") {
					Coordinate2D norm(Coordinate2D(1, -1).normalize());
					Coordinate moveDist = 4.5f - Collidable::getPushoutDistance(dir, norm);
					Coordinate2D projection = dir.project(norm.perpCCW(), dist - moveDist);
					Coordinate2D expected_pos = origin + dir*moveDist + projection;
					CHECK(position.x == ApproxEps(expected_pos.x));
					CHECK(position.y == ApproxEps(expected_pos.y));
				}
			}
		}
	}
	GIVEN("The collider is a rectangle.") {
		Polygon collider(Rectangle(0, 0, 1, 1).toPoly());
		GIVEN("The stationary polygon is an octagon.") {
			std::vector<Polygon> polys;
			polys.push_back(shapes::octagon);
			CollisionMapTest map(polys);
			WHEN("The rectangle hits the octagon on the octagon's edge.") {
				Coordinate2D origin(3, 0);
				Coordinate2D dir(Coordinate2D(-1, 0.5f).normalize());
				Coordinate dist(10);
				Coordinate2D position = tester.move(origin, collider, dir*dist, map);
				THEN("It deflects off the octagon's edge.") {
					Coordinate2D norm(Coordinate2D(3, 1).normalize());
					Coordinate moveDist(std::sqrt(1.2f*1.2f + 0.6f * 0.6f) - Collidable::getPushoutDistance(dir, norm)); // Intersect at (1.8, 0.6).
					Coordinate2D projection = dir.project(norm.perpCCW(), dist - moveDist);
					Coordinate2D expected_pos = origin + dir*moveDist + projection;
					CHECK(position.x == ApproxEps(expected_pos.x));
					CHECK(position.y == ApproxEps(expected_pos.y));
				}
			}
		}
	}
}