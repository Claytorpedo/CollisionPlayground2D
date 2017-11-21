#include "catch.hpp"
#include "definitions.h"

#include <vector>

#include "../Geometry/Movable.h"
#include "../Geometry/Collidable.h"
#include "../Geometry/Wall.h"
#include "../Geometry/CollisionMap.h"
#include "../Units.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Rectangle.h"

using namespace units;


struct MovableTest : public Movable {
	Shape* collider;
	Coordinate2D position;

	MovableTest(Movable::CollisionType type, Shape* collider) : Movable(type), collider(collider) {}
	MovableTest(Movable::CollisionType type, Shape* collider, Coordinate2D position) : Movable(type), collider(collider), position(position) {}
	~MovableTest() {
		delete collider;
	}
	const Coordinate2D& getPosition() const { return position;  }
	const Shape* const getCollider() const { return collider; }
	void move(const Coordinate2D& delta, const CollisionMap& map) {
		position = Movable::move(position, collider, delta, &map);
	}
};

class CollisionMapTest : public CollisionMap {
public:
	CollisionMapTest() {}
	CollisionMapTest(std::vector<Collidable*> obstacles) : obstacles_(obstacles) {}
	~CollisionMapTest() {
		clear();
	}
	virtual const std::vector<Collidable*> getColliding(const Collidable& collider, const units::Coordinate2D& delta) const {
		return std::vector<Collidable*>(obstacles_.begin(), obstacles_.end());
	}
	void add(Shape* obstacle) {
		obstacles_.push_back(new Wall(obstacle));
	}
	void add(Shape* obstacle, const Coordinate2D& position) {
		obstacles_.push_back(new Wall(obstacle, position));
	}
	void clear() {
		for (std::size_t i = 0; i < obstacles_.size(); ++i)
			delete obstacles_[i];
		obstacles_.clear();
	}
private:
	std::vector<Collidable*> obstacles_;
};

SCENARIO("A movable deflects off a wall.", "[movable][deflection]") {
	CollisionMapTest map;
	GIVEN("The movable is a right triangle.") {
		MovableTest mover(Movable::CollisionType::DEFLECTION, new Polygon(shapes::rightTri));
		GIVEN("The stationary polygon is a rectangle.") {
			map.add(new Rectangle(0, 0, 1, 1));
			WHEN("The collider misses the rectangle.") {
				Coordinate2D origin(0, -10);
				Coordinate2D delta(10, 0);
				mover.position = origin;
				mover.move(delta, map);
				THEN("It moves the full delta vector.") {
					Coordinate2D expected_pos = origin + delta;
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
			WHEN("The movable hits the rectangle straight on.") {
				Coordinate2D origin(-0.5f, -5);
				Coordinate2D dir(0, 1);
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves to the rectangle and stops.") {
					Coordinate2D expected_pos(-0.5f, -1 - Movable::getPushoutDistance(dir, Coordinate2D(0, -1)));
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
			WHEN("The movable hits the rectangle on the movable's edge.") {
				Coordinate2D origin(0.5f, -5);
				Coordinate2D dir(0, 1);
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It deflects off the rectangle using the movable's edge.") {
					Coordinate2D norm(Coordinate2D(1, -1).normalize());
					Coordinate moveDist = 4.5f - Movable::getPushoutDistance(dir, norm);
					Coordinate2D projection = dir.project(norm.perpCCW(), dist - moveDist);
					Coordinate2D expected_pos = origin + dir*moveDist + projection;
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
		GIVEN("The stationary collidable is a triangle with a matching edge.") {
			map.add(new Polygon(shapes::edgeTriR), Coordinate2D(0, 5));
			WHEN("The movable collides with the matching edge.") {
				Coordinate2D origin(0.5f, 0);
				Coordinate2D dir(0, 1);
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It deflects along it.") {
					Coordinate2D norm(Coordinate2D(1, -1).normalize());
					Coordinate moveDist = 5.5f - Movable::getPushoutDistance(dir, norm);
					Coordinate2D projection = dir.project(norm.perpCCW(), dist - moveDist);
					Coordinate2D expected_pos = origin + dir*moveDist + projection;
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
	}
	GIVEN("The movable is a rectangle.") {
		MovableTest mover(Movable::CollisionType::DEFLECTION, new Rectangle(0, 0, 1, 1));
		GIVEN("The stationary collidable is an octagon.") {
			map.add(new Polygon(shapes::octagon));
			WHEN("The rectangle hits the octagon on the octagon's edge.") {
				Coordinate2D origin(3, 0);
				Coordinate2D dir(Coordinate2D(-1, 0.5f).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It deflects off the octagon's edge.") {
					Coordinate2D norm(Coordinate2D(3, 1).normalize());
					Coordinate moveDist(std::sqrt(1.2f*1.2f + 0.6f * 0.6f) - Movable::getPushoutDistance(dir, norm)); // Intersect at (1.8, 0.6).
					Coordinate2D projection = dir.project(norm.perpCCW(), dist - moveDist);
					Coordinate2D expected_pos = origin + dir*moveDist + projection;
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
	}
}
