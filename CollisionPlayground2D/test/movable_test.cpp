#include "catch.hpp"
#include "definitions.h"

#include <vector>

#include "../Geometry/Movable.h"
#include "../Geometry/Collidable.h"
#include "../Geometry/Wall.h"
#include "../Geometry/CollisionMap.h"
#include "../Units.h"
#include "../Geometry/ShapeContainer.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Rectangle.h"

using namespace units;


struct MovableTest : public Movable {
	ShapeContainer collider;
	Coordinate2D position;

	MovableTest(Movable::CollisionType type, const ShapeContainer& collider) : Movable(type), collider(collider) {}
	MovableTest(Movable::CollisionType type, const ShapeContainer& collider, Coordinate2D position) : Movable(type), collider(collider), position(position) {}
	~MovableTest() {}
	const Coordinate2D& getPosition() const { return position;  }
	const ShapeContainer& getCollider() const { return collider; }
	void move(const Coordinate2D& delta, const CollisionMap& map) {
		position = Movable::move(collider, position, delta, &map);
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
	void add(const ShapeContainer& obstacle) {
		obstacles_.push_back(new Wall(obstacle));
	}
	void add(const ShapeContainer& obstacle, const Coordinate2D& position) {
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

SCENARIO("A movable deflects off a stationary collidable.", "[movable][deflection]") {
	CollisionMapTest map;
	GIVEN("The movable is a right triangle.") {
		MovableTest mover(Movable::CollisionType::DEFLECTION, ShapeContainer(new Polygon(shapes::rightTri)));
		GIVEN("The stationary collidable is a rectangle.") {
			map.add(Rectangle(0, 0, 1, 1));
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
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(0, 5));
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
		MovableTest mover(Movable::CollisionType::DEFLECTION, ShapeContainer(new Rectangle(0, 0, 1, 1)));
		GIVEN("The stationary collidable is an octagon.") {
			map.add(Polygon(shapes::octagon));
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

SCENARIO("A movable deflects off multiple stationary collidables.", "[movable][deflection]") {
	CollisionMapTest map;
	GIVEN("The movable is a rectangle.") {
		MovableTest mover(Movable::CollisionType::DEFLECTION, ShapeContainer(new Rectangle(0, 0, 1, 1)));
		GIVEN("Two stationary rectangles form a wall in mover's path.") {
			map.add(Rectangle(-3, 0.5f, 1, 1));
			map.add(Rectangle(-3, -0.5f, 1, 1));
			WHEN("The mover moves left into them.") {
				Coordinate2D origin(0, 0);
				Coordinate2D dir(-1, 0);
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It will hit the wall and stop.") {
					Coordinate2D norm(1, 0);
					Coordinate2D expected_pos(-2 + Movable::getPushoutDistance(dir, norm), 0);
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
		GIVEN("Two stationary rectangles, one to the left and one below the mover.") {
			map.add(Rectangle(-2, 2, 1, 2));
			map.add(Rectangle(-1, 4, 1, 1));
			WHEN("The mover moves down-left.") {
				Coordinate2D origin(0, 0);
				Coordinate2D dir(Coordinate2D(-1, 1).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It will deflect off of the first, and stop on the second.") {
					Coordinate diagDist = Movable::getPushoutDistance(dir, Coordinate2D(1, 0)); // Diagonal pushout distance for first collision.
					Coordinate x_pos(-1 + std::sqrt((diagDist * diagDist)/2.0f)); // Solve the triangle for the final x position.
					Coordinate y_pos(3 - Movable::getPushoutDistance(Coordinate2D(0, 1), Coordinate2D(0, -1))); // Deflects straight down for second collision.
					CHECK(mover.position.x == ApproxEps(x_pos));
					CHECK(mover.position.y == ApproxEps(y_pos));
				}
			}
		}
		GIVEN("A series of shapes.") {
			map.add(Rectangle(2, 2, 1, 1));
			map.add(Rectangle(2, 3, 1, 1));
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(0.5f, 5));
			map.add(Rectangle(4, 9, 1, 1));
			map.add(Rectangle(5, 9, 1, 1));
			map.add(Rectangle(6, 8, 1, 0.5f));
			WHEN("The mover moves down-right.") {
				Coordinate2D origin(0, 0);
				Coordinate2D dir(Coordinate2D(1, 1).normalize());
				Coordinate dist(100);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It deflects several times, and stop against some rectangles.") {
					Coordinate diagDist = Movable::getPushoutDistance(Coordinate2D(1, 1).normalize(), Coordinate2D(0, -1)); // Off edgeTriR, into rectangle.
					Coordinate y_pos(8 - std::sqrt((diagDist * diagDist) / 2.0f)); // Solve the triangle for the final y position.
					Coordinate x_pos(5 - Movable::getPushoutDistance(Coordinate2D(1, 0), Coordinate2D(-1, 0))); // Hits final rectangle straight on.
					CHECK(mover.position.x == ApproxEps(x_pos));
					CHECK(mover.position.y == ApproxEps(y_pos));
				}
			}
		}
	}
	GIVEN("The movable is an isosceles triangle.") {
		MovableTest mover(Movable::CollisionType::DEFLECTION, ShapeContainer(new Polygon(shapes::isoTri)));
		GIVEN("Two stationary rectangles form a wall in mover's path.") {
			map.add(Rectangle(-3, 0.5f, 1, 1));
			map.add(Rectangle(-3, -0.5f, 1, 1));
			WHEN("The mover moves left into them.") {
				Coordinate2D origin(0, 0);
				Coordinate2D dir(-1, 0);
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It will hit the wall and stop.") {
					Coordinate2D norm(1, 0);
					Coordinate2D expected_pos(-2 + Movable::getPushoutDistance(dir, norm), 0);
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
		GIVEN("A series of shapes.") {
			map.add(Rectangle(2, 2, 1, 1));
			map.add(Rectangle(2, 3, 1, 1));
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(0.5f, 5));
			map.add(Rectangle(4, 9, 1, 1));
			map.add(Rectangle(5, 9, 1, 1));
			map.add(Rectangle(6, 7.5f, 1, 0.5f));
			WHEN("The mover moves down-right.") {
				Coordinate2D origin(-1, 0);
				Coordinate2D dir(Coordinate2D(1, 2).normalize());
				Coordinate dist(100);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It deflects several times, and stop against some rectangles.") {
					Coordinate diagDist = Movable::getPushoutDistance(Coordinate2D(1, 1).normalize(), Coordinate2D(0, -1)); // Off edgeTriR, into rectangle.
					Coordinate y_pos(8 - std::sqrt((diagDist * diagDist) / 2.0f)); // Solve the triangle for the final y position.
					Coordinate x_pos(4 - Movable::getPushoutDistance(Coordinate2D(1, 0), Coordinate2D(-1, 0))); // Hits final rectangle straight on.
					CHECK(mover.position.x == ApproxEps(x_pos));
					CHECK(mover.position.y == ApproxEps(y_pos));
				}
			}
		}
	}
}

SCENARIO("A mover deflects into a wedge.", "[movable][deflection]") {
	CollisionMapTest map;
	GIVEN("The mover is a rectangle.") {
		MovableTest mover(Movable::CollisionType::DEFLECTION, ShapeContainer(new Rectangle(0, 0, 1, 1)));
		GIVEN("A wedge formed by two triangles, with no room for the mover.") {
			map.add(Polygon(std::vector<Coordinate2D>{Coordinate2D(-1, 0), Coordinate2D(0, 1), Coordinate2D(0.5f, 0)}));
			map.add(Polygon(std::vector<Coordinate2D>{Coordinate2D(0.5f, 0), Coordinate2D(1, 1), Coordinate2D(2, 0)}));
			WHEN("The mover moves up into the wedge.") {
				Coordinate2D origin(0, 5);
				Coordinate2D dir(Coordinate2D(0, -1).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It hits the top of the wedge and stops immediately.") {
					Coordinate2D expected(0, 1);
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
		GIVEN("A wedge formed by two triangles, that the mover can move halfway down between.") {
			map.add(Polygon(std::vector<Coordinate2D>{Coordinate2D(-2, 0), Coordinate2D(-0.5f, 1), Coordinate2D(0.5f, 0)}));
			map.add(Polygon(std::vector<Coordinate2D>{Coordinate2D(0.5f, 0), Coordinate2D(1.5f, 1), Coordinate2D(2, 0)}));
			WHEN("The mover moves up into the wedge.") {
				Coordinate2D origin(0, 5);
				Coordinate2D dir(Coordinate2D(0, -1).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves halfway down the wedge and stops.") {
					Coordinate2D expected(0, 0.5f + Movable::getPushoutDistance(Coordinate2D(0, -1), Coordinate2D(-1, 1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
			WHEN("The mover moves up into the wedge from off-center.") {
				Coordinate2D origin(0.4f, 5);
				Coordinate2D dir(Coordinate2D(0, -1).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves halfway down the wedge and stops.") {
					Coordinate2D expected(0, 0.5f + Movable::getPushoutDistance(Coordinate2D(0, -1), Coordinate2D(-1, 1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
			WHEN("The mover moves up-left into the wedge from off-center.") {
				Coordinate2D origin(0.8f, 5);
				Coordinate2D dir(Coordinate2D(-0.1f, -1).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves halfway down the wedge and stops.") {
					Coordinate2D expected(0, 0.5f + Movable::getPushoutDistance(Coordinate2D(0, -1), Coordinate2D(-1, 1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
	}
	GIVEN("The mover is an isosceles triangle.") {
		MovableTest mover(Movable::CollisionType::DEFLECTION, ShapeContainer(new Polygon(shapes::isoTri)));
		GIVEN("A wedge formed by two triangles, that fit the mover perfectly.") {
			map.add(Polygon(std::vector<Coordinate2D>{ Coordinate2D(0, 0), Coordinate2D(-1, -1), Coordinate2D(-2, 0) }));
			map.add(Polygon(std::vector<Coordinate2D>{ Coordinate2D(-2, 0), Coordinate2D(-3, -1), Coordinate2D(-4, 0) }));
			WHEN("The mover moves down into the wedge.") {
				Coordinate2D origin(-3, -5);
				Coordinate2D dir(Coordinate2D(0, 1).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves to the bottom of the wedge and stops.") {
					Coordinate2D expected(-3, -1 - Movable::getPushoutDistance(Coordinate2D(0, 1), Coordinate2D(1, -1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
		GIVEN("A wedge formed by two triangles, that are spaced slightly apart.") {
			map.add(Polygon(std::vector<Coordinate2D>{ Coordinate2D(0, 0), Coordinate2D(-1, -1), Coordinate2D(-2, 0) }));
			map.add(Polygon(std::vector<Coordinate2D>{ Coordinate2D(-2.5f, 0), Coordinate2D(-3.5f, -1), Coordinate2D(-4.5f, 0) }));
			WHEN("The mover moves down into the wedge.") {
				Coordinate2D origin(-3, -5);
				Coordinate2D dir(Coordinate2D(0, 1).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It bottoms out between them and stops.") {
					Coordinate2D expected(-3.25f, -0.75f - Movable::getPushoutDistance(Coordinate2D(0, 1), Coordinate2D(1, -1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
	}
	GIVEN("The mover is an octagon.") {
		MovableTest mover(Movable::CollisionType::DEFLECTION, ShapeContainer(new Polygon(shapes::octagon)));
		GIVEN("A wedge formed by several shapes, that fit the octagon perfectly.") {
			map.add(Polygon(std::vector<Coordinate2D>{Coordinate2D(-2, 0), Coordinate2D(-1.5f, -1.5f), Coordinate2D(-2, -1)}));
			map.add(Polygon(std::vector<Coordinate2D>{Coordinate2D(-1.5f, -1.5f), Coordinate2D(0, -2), Coordinate2D(-1, -2)}));
			map.add(Polygon(std::vector<Coordinate2D>{Coordinate2D(0, -2), Coordinate2D(1.5f, -1.5f), Coordinate2D(1, -2)}));
			map.add(Polygon(std::vector<Coordinate2D>{Coordinate2D(1.5f, -1.5f), Coordinate2D(2, 0), Coordinate2D(2, -2)}));
			WHEN("The mover moves up into the wedge.") {
				Coordinate2D origin(0, 5);
				Coordinate2D dir(Coordinate2D(0, -1).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves to the top of the wedge, and stops.") {
					Coordinate2D expected(0, 0 + Movable::getPushoutDistance(Coordinate2D(0, -1), Coordinate2D(-0.5f, 1.5f).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
			WHEN("The mover moves up into the wedge from off-center.") {
				Coordinate2D origin(1, 5);
				Coordinate2D dir(Coordinate2D(0, -1).normalize());
				Coordinate dist(100);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves to the top of the wedge, and stops.") {
					// Will deflect between the steep edges.
					Coordinate2D expected(0, Movable::getPushoutDistance(Coordinate2D(0, -1), Coordinate2D(1.5f, 0.5f).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
	}
}

SCENARIO("A mover deflects down a corridor.", "[movable][deflection]") {
	CollisionMapTest map;
	GIVEN("The mover is a rectangle.") {
		MovableTest mover(Movable::CollisionType::DEFLECTION, ShapeContainer(new Rectangle(0, 0, 1, 1)));
		GIVEN("Two rectangles form a corridr.") {
			map.add(Rectangle(-1, -2, 1, 12));
			map.add(Rectangle(1, 0, 1, 10));
			WHEN("The mover moves down into the corridor.") {
				Coordinate2D origin(0, -2);
				Coordinate2D dir(Coordinate2D(0, 1).normalize());
				Coordinate dist(20);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves through without collision.") {
					Coordinate2D expected(0, 18);
					CHECK(mover.position.x == ApproxEps(expected.x));
					CHECK(mover.position.y == ApproxEps(expected.y));
				}
			}
			WHEN("The mover moves into the corridor at an angle.") {
				Coordinate2D origin(0, -2);
				Coordinate2D dir(Coordinate2D(-1, 1).normalize());
				Coordinate dist(20);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves through without collision.") {
					Coordinate2D expected = origin +  dir.project(Coordinate2D(0, 1), dist);
					CHECK(mover.position.x == ApproxEps(expected.x));
					CHECK(mover.position.y == ApproxEps(expected.y));
				}
			}
		}
		GIVEN("Rectangles form a corridor with a stopper at the end.") {
			map.add(Rectangle(-1, 1, 1, 1));
			map.add(Rectangle(1, 1, 1, 1));
			map.add(Rectangle(-1, 2, 1, 5));
			map.add(Rectangle(1, 2, 1, 5));
			map.add(Rectangle(0, 7, 1, 1));
			WHEN("The mover moves down into the corridor.") {
				Coordinate2D origin(0, 0);
				Coordinate2D dir(Coordinate2D(0, 1).normalize());
				Coordinate dist(20);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves down to the stopper and stops.") {
					Coordinate2D expected(0, 6.0f - Movable::getPushoutDistance(Coordinate2D(0, 1), Coordinate2D(0, -1)));
					CHECK(mover.position.x == ApproxEps(expected.x));
					CHECK(mover.position.y == ApproxEps(expected.y));
				}
			}
		}
		GIVEN("Triangles form a diagonal corridor.") {
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(0, 1));
			map.add(Polygon(shapes::rightTri), Coordinate2D(1, 0));
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(1, 2));
			map.add(Polygon(shapes::rightTri), Coordinate2D(2, 1));
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(2, 3));
			map.add(Polygon(shapes::rightTri), Coordinate2D(3, 2));
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(3, 4));
			map.add(Polygon(shapes::rightTri), Coordinate2D(4, 3));
			WHEN("The mover moves diagonally through the corridor.") {
				Coordinate2D origin(0, 0);
				Coordinate2D dir(Coordinate2D(1, 1).normalize());
				Coordinate dist(10);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves through without collision.") {
					Coordinate expected(std::sqrt(50.0f));
					CHECK(mover.position.x == ApproxEps(expected));
					CHECK(mover.position.y == ApproxEps(expected));
				}
			}
		}
	}
	GIVEN("The mover is a rectangle, slightly smaller than a full unit.") {
		// Allow the mover to deflect down a tight corridor, by taking the collision buffer into account.
		MovableTest mover(Movable::CollisionType::DEFLECTION, ShapeContainer(new Rectangle(0, 0, 1 - Movable::COLLISION_BUFFER, 1 - Movable::COLLISION_BUFFER)));
		GIVEN("Several shapes form a capped corridor with a bend in the middle.") {
			map.add(Rectangle(-1, 1, 1, 10));
			map.add(Rectangle(1, 1, 1, 9));
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(0, 12));
			map.add(Polygon(shapes::rightTri), Coordinate2D(1, 11));
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(1, 13));
			map.add(Polygon(shapes::rightTri), Coordinate2D(2, 12));
			map.add(Polygon(shapes::edgeTriR), Coordinate2D(2, 14));
			map.add(Polygon(shapes::rightTri), Coordinate2D(3, 13));
			map.add(Rectangle(2, 15, 1, 3));
			map.add(Rectangle(4, 14, 1, 4));
			map.add(Rectangle(3, 19, 1, 1));
			WHEN("The mover moves down through the corridor.") {
				Coordinate2D origin(Movable::COLLISION_BUFFER * 0.5f, 0);
				Coordinate2D dir(Coordinate2D(0, 1).normalize());
				Coordinate dist(25);
				mover.position = origin;
				mover.move(dir*dist, map);
				THEN("It moves through, deflects off the bend, and reaches the end.") {
					Coordinate2D expected(3, 18); // The deflection buffer and rectangle's dimensions cancell out.
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
	}
}
