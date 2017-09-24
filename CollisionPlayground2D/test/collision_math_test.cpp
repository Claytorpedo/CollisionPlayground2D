#include "catch.hpp"

#include <vector>

#include "../Geometry/CollisionMath.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Rectangle.h"
#include "../Units.h"
#include "../Constants.h"

using namespace units;

namespace { // Some shapes we'll use.
	const std::vector<Coordinate2D> rightTri = { Coordinate2D(0,0), Coordinate2D(1,1), Coordinate2D(1,0) };
	const std::vector<Coordinate2D> tri = { Coordinate2D(-1, -2), Coordinate2D(1, 0), Coordinate2D(3, -1) };
	const std::vector<Coordinate2D> oct = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
	                                        Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
	const std::vector<Coordinate2D> arb = { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
	const std::vector<Coordinate2D> smallOct = { Coordinate2D(0,0.5f), Coordinate2D(0.2f,0.8f), Coordinate2D(0.5f,1), Coordinate2D(0.8f,0.8f),
	                                             Coordinate2D(1,0.5f), Coordinate2D(0.8f,0.2f), Coordinate2D(0.5f,0), Coordinate2D(0.2f,0.2f) };
}

SCENARIO("A moving polygon may collide with a stationary one.", "[poly][collision]") {
	GIVEN("The collider is a triangle.") {
		Polygon collider(rightTri);
		GIVEN("The stationary polygon is a triangle.") {
			Polygon stationary(tri);
			WHEN("The collider and stationary polygons are touching.") {
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, -1), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units below the stationary polygon.") {
				stationary.translate(0, -10);
				THEN("Moving the collider more than 10 units up causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units up avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is a rectangle.") {
			Polygon stationary(Rectangle(1, 0, 1, 1).toPoly());
			WHEN("The collider and stationary polygons are touching.") {
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, -1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units left of the stationary polygon.") {
				stationary.translate(10, 0);
				THEN("Moving the collider more than 10 units right causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units right avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is an octagon.") {
			Polygon stationary(oct);
			WHEN("The collider and stationary polygons are touching.") {
				stationary.translate(-2, 0);
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units right of the stationary polygon.") {
				stationary.translate(-12, 0);
				THEN("Moving the collider more than 10 units left causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units left avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is an arbitrary polygon.") {
			Polygon stationary(arb);
			WHEN("The collider and stationary polygons are touching.") {
				stationary.translate(1, 0);
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 2).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units right of the stationary polygon.") {
				stationary.translate(11, 0);
				THEN("Moving the collider more than 10 units left causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units left avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
			}
		}
	}
	GIVEN("The collider is a rectangle.") {
		Polygon collider(Rectangle(0, 0, 1, 1).toPoly());
		GIVEN("The stationary polygon is a triangle.") {
			Polygon stationary(tri);
			WHEN("The collider and stationary polygons are touching.") {
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, -1), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units below the stationary polygon.") {
				stationary.translate(0, -10);
				THEN("Moving the collider more than 10 units up causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units up avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is a rectangle.") {
			Polygon stationary(Rectangle(1, 0, 1, 1).toPoly());
			WHEN("The collider and stationary polygons are touching.") {
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, -1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units left of the stationary polygon.") {
				stationary.translate(10, 0);
				THEN("Moving the collider more than 10 units right causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units right avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is an octagon.") {
			Polygon stationary(oct);
			WHEN("The collider and stationary polygons are touching.") {
				stationary.translate(-2, 0);
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units right of the stationary polygon.") {
				stationary.translate(-12, 0);
				THEN("Moving the collider more than 10 units left causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units left avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is an arbitrary polygon.") {
			Polygon stationary(arb);
			WHEN("The collider and stationary polygons are touching.") {
				stationary.translate(1, 0);
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 2).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units right of the stationary polygon.") {
				stationary.translate(11, 0);
				THEN("Moving the collider more than 10 units left causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units left avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
			}
		}
	}
	GIVEN("The collider is an octagon.") {
		Polygon collider(smallOct);
		GIVEN("The stationary polygon is a triangle.") {
			std::vector<Coordinate2D> newtri = { Coordinate2D(-1, -2), Coordinate2D(0.5f, 0), Coordinate2D(3, -1) };
			Polygon stationary(newtri);
			WHEN("The collider and stationary polygons are touching.") {
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, -1), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units below the stationary polygon.") {
				stationary.translate(0, -10);
				THEN("Moving the collider more than 10 units up causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units up avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is a rectangle.") {
			Polygon stationary(Rectangle(1, 0, 1, 1).toPoly());
			WHEN("The collider and stationary polygons are touching.") {
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, -1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units left of the stationary polygon.") {
				stationary.translate(10, 0);
				THEN("Moving the collider more than 10 units right causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units right avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is an octagon.") {
			Polygon stationary(oct);
			WHEN("The collider and stationary polygons are touching.") {
				stationary.translate(-2, 0.5f);
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units right of the stationary polygon.") {
				stationary.translate(-12, 0.5f);
				THEN("Moving the collider more than 10 units left causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units left avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is an arbitrary convex polygon.") {
			Polygon stationary(arb);
			WHEN("The collider and stationary polygons are touching.") {
				stationary.translate(1, 0.5f);
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, -2).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units right of the stationary polygon.") {
				stationary.translate(11, 0.5f);
				THEN("Moving the collider more than 10 units left causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units left avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
			}
		}
	}
	GIVEN("The collider is an arbitrary convex polygon.") {
		Polygon collider(arb);
		GIVEN("The stationary polygon is a triangle.") {
			std::vector<Coordinate2D> newtri = { Coordinate2D(1, -2), Coordinate2D(2, -3), Coordinate2D(1, -3) };
			Polygon stationary(newtri);
			WHEN("The collider and stationary polygons are touching.") {
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -2).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, -1), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units below the stationary polygon.") {
				stationary.translate(0, -10);
				THEN("Moving the collider more than 10 units up causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units up avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is a rectangle.") {
			Polygon stationary(Rectangle(3, 0, 1, 1).toPoly());
			WHEN("The collider and stationary polygons are touching.") {
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, -1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units left of the stationary polygon.") {
				stationary.translate(10, 0);
				THEN("Moving the collider more than 10 units right causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units right avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is an octagon.") {
			Polygon stationary(oct);
			WHEN("The collider and stationary polygons are touching.") {
				stationary.translate(-2, 0);
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 1).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units right of the stationary polygon.") {
				stationary.translate(-12, 0);
				THEN("Moving the collider more than 10 units left causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units left avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary));
			}
		}
		GIVEN("The stationary polygon is an arbitrary convex polygon.") {
			Polygon stationary(arb);
			WHEN("The collider and stationary polygons are touching.") {
				stationary.translate(3, 1);
				THEN("Moving the collider towards the stationary polygon causes a collision.") {
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 0.1f, stationary));
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary));
				}
				THEN("Moving the collider away from the stationary polygon avoids a collision.") {
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 0, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary));
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, -2).normalize(), 10, stationary));
				}
			}
			WHEN("The collider is 10 units right of the stationary polygon.") {
				stationary.translate(13, 1);
				THEN("Moving the collider more than 10 units left causes a collision.")
					REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10.1f, stationary));
				THEN("Moving the collider 10 or fewer units left avoids a collision.")
					REQUIRE_FALSE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary));
			}
		}
	}
}

#define ApproxEps(x) Approx(x).margin(constants::EPSILON)
// Approx for collision resolution. Only use if the distance to move is >= collision_math::COLLISION_PUSHOUT_DISTANCE.
#define ApproxCollide(x) ApproxEps(x - collision_math::COLLISION_PUSHOUT_DISTANCE)

// Test the results of a collision, checking both the collision normal and the movement distance.
inline void _test_collision_output(std::string testName, bool was_collision, Coordinate2D expected_norm,
	                               Coordinate2D out_norm, Approx expected_dist, Coordinate out_dist) {
	SECTION(testName) {
		REQUIRE(was_collision);
		out_norm = out_norm.normalize();
		expected_norm = expected_norm.normalize();
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
		REQUIRE(out_dist == expected_dist);
	}
}

TEST_CASE("Collision detection and resolution for two polygons; one moving and one stationary.", "[poly][collision]") {
	Coordinate out_dist;
	Coordinate2D out_norm, expected_normal;
	SECTION("Vertex to vertex collisions.") {
		Polygon collider(rightTri);
		// What normal to use is not particularily clear with these cases. Should the collider stop, or deflect off a side?
		SECTION("When moving the collider towards a polygon it is already touching, it should collide immediately and not move at all.") {
			Polygon stationary(tri);
			REQUIRE(collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary, out_dist, out_norm));
			REQUIRE(out_dist == 0);
			stationary = Rectangle(-1, 0, 1, 1).toPoly();
			REQUIRE(collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary, out_dist, out_norm));
			REQUIRE(out_dist == 0);
		}
		SECTION("When moving the collider towards a distant polygon, it should move the distance between them.") {
			Polygon stationary = Polygon(oct);
			stationary.translate(10, 0);
			REQUIRE(collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary, out_dist, out_norm));
			REQUIRE(out_dist == ApproxCollide(7));
			stationary = Polygon(tri);
			stationary.translate(3, -4);
			REQUIRE(collision_math::collides(collider, Coordinate2D(3, -4).normalize(), 10, stationary, out_dist, out_norm));
			REQUIRE(out_dist == ApproxCollide(5));
		}
	}
	SECTION("Vertex to edge collisions.") {
		Polygon collider(rightTri);
		SECTION("When already touching, they should collide immediately and not move at all.") {
			SECTION("Moving collider into the edge of a triangle immediately above and to the right of it.") {
				Polygon stationary(tri);
				stationary.translate(1, 1);
				bool r = collision_math::collides(collider, Coordinate2D(0, -1), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving up into the triangle.", r, (stationary[1] - stationary[0]).perpCCW(), out_norm, ApproxEps(0), out_dist);
				r = collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving right into the triangle.", r, (stationary[1] - stationary[0]).perpCCW(), out_norm, ApproxEps(0), out_dist);
				r = collision_math::collides(collider, Coordinate2D(1, -1).normalize(), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving up-right into the triangle.", r, (stationary[1] - stationary[0]).perpCCW(), out_norm, ApproxEps(0), out_dist);
			}
			SECTION("Moving collider into a rectangle immediately to the left of it.") {
				Polygon stationary(Rectangle(-1, -1, 1, 2).toPoly());
				bool r = collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving left into the rectangle.", r, (stationary[3] - stationary[2]).perpCCW(), out_norm, ApproxEps(0), out_dist);
				r = collision_math::collides(collider, Coordinate2D(-1, 1).normalize(), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving down-left into the rectangle.", r, (stationary[3] - stationary[2]).perpCCW(), out_norm, ApproxEps(0), out_dist);
				r = collision_math::collides(collider, Coordinate2D(-1, -100).normalize(), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving up-left into the rectangle.", r, (stationary[3] - stationary[2]).perpCCW(), out_norm, ApproxEps(0), out_dist);
			}
		}
		SECTION("When moving the collider towards a distant polygon, it should move the distance between them.") {
			Polygon stationary(Rectangle(-10, -1, 1, 2).toPoly());
			bool r = collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving left into the rectangle.", r, (stationary[3] - stationary[2]).perpCCW(), out_norm, ApproxCollide(9), out_dist);
			stationary = Polygon(tri);
			stationary.translate(4, -3);
			r = collision_math::collides(collider, Coordinate2D(3, -4).normalize(), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving up-right into the triangle.", r, (stationary[1] - stationary[0]).perpCCW(), out_norm, ApproxCollide(5), out_dist);
		}
	}
	SECTION("Edge to vertex collisions.") {
		Polygon collider(rightTri);
		SECTION("When already touching, they should collide immediately and not move at all.") {
			SECTION("Moving collider's edge into the vertex of a triangle immediately above and to the left of it.") {
				Polygon stationary(tri);
				stationary.translate(-2.5f, 1.5f);
				bool r = collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving left into the triangle.", r, (collider[1] - collider[0]).perpCW(), out_norm, ApproxEps(0), out_dist);
				r = collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving down into the triangle.", r, (collider[1] - collider[0]).perpCW(), out_norm, ApproxEps(0), out_dist);
				r = collision_math::collides(collider, Coordinate2D(-1, 1).normalize(), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving down-left into the triangle.", r, (collider[1] - collider[0]).perpCW(), out_norm, ApproxEps(0), out_dist);
			}
			SECTION("Moving collider into an octagon immediately to the right of it.") {
				Polygon stationary(oct);
				stationary.translate(3, 0.5f);
				bool r = collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving right into the octagon.", r, (collider[2] - collider[1]).perpCW(), out_norm, ApproxEps(0), out_dist);
				r = collision_math::collides(collider, Coordinate2D(1, 1).normalize(), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving down-right into the octagon.", r, (collider[2] - collider[1]).perpCW(), out_norm, ApproxEps(0), out_dist);
				r = collision_math::collides(collider, Coordinate2D(1, -100).normalize(), 10, stationary, out_dist, out_norm);
				_test_collision_output("Moving up-right into the octagon.", r, (collider[2] - collider[1]).perpCW(), out_norm, ApproxEps(0), out_dist);
			}
		}
		SECTION("When moving the collider towards a distant polygon, it should move the distance between them.") {
			Polygon stationary(tri);
			stationary.translate(-5.5f, 5.5f);
			bool r = collision_math::collides(collider, Coordinate2D(-3, 4).normalize(), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving down-left into the triangle.", r, (collider[1] - collider[0]).perpCW(), out_norm, ApproxCollide(5), out_dist);
		}
	}
	SECTION("Matching edges collisions / edge to edge.") {
		std::vector<Coordinate2D> newtri = { Coordinate2D(-1, -2), Coordinate2D(-1, 0), Coordinate2D(1, 0) };
		SECTION("When already touching, they should collide immediately and not move at all.") {
			Polygon collider(rightTri);
			Polygon stationary(Rectangle(1, 0, 1, 1).toPoly());
			bool r = collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving right into the rectangle.", r, (stationary[1] - stationary[0]).perpCCW(), out_norm, ApproxEps(0), out_dist);
			r = collision_math::collides(collider, Coordinate2D(1, 1).normalize(), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving down-right into the rectangle.", r, (stationary[1] - stationary[0]).perpCCW(), out_norm, ApproxEps(0), out_dist);
			r = collision_math::collides(collider, Coordinate2D(1, -100).normalize(), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving up-right into the rectangle.", r, (stationary[1] - stationary[0]).perpCCW(), out_norm, ApproxEps(0), out_dist);
			collider = Polygon(tri);
			stationary = Polygon(newtri);
			r = collision_math::collides(collider, Coordinate2D(-1, 0), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving left into the triangle.", r, (stationary[0] - stationary[2]).perpCCW(), out_norm, ApproxEps(0), out_dist);
			r = collision_math::collides(collider, Coordinate2D(0, 1), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving down into the triangle.", r, (stationary[0] - stationary[2]).perpCCW(), out_norm, ApproxEps(0), out_dist);
			r = collision_math::collides(collider, Coordinate2D(-1, 100).normalize(), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving down-left into the triangle.", r, (stationary[0] - stationary[2]).perpCCW(), out_norm, ApproxEps(0), out_dist);
		}
		SECTION("When moving the collider towards a distant polygon, it should move the distance between them.") {
			Polygon collider(rightTri);
			Polygon stationary(Rectangle(5, 0, 1, 10).toPoly());
			bool r = collision_math::collides(collider, Coordinate2D(1, 0), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving right into the rectangle.", r, (stationary[1] - stationary[0]).perpCCW(), out_norm, ApproxCollide(4), out_dist);
			r = collision_math::collides(collider, Coordinate2D(4, 3).normalize(), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving down-right into the rectangle.", r, (stationary[1] - stationary[0]).perpCCW(), out_norm, ApproxCollide(5), out_dist);
			collider = Polygon(tri);
			stationary = Polygon(newtri);
			stationary.translate(-4, 3);
			r = collision_math::collides(collider, Coordinate2D(-4, 3).normalize(), 10, stationary, out_dist, out_norm);
			_test_collision_output("Moving down-left into the triangle.", r, (stationary[0] - stationary[2]).perpCCW(), out_norm, ApproxCollide(5), out_dist);
		}
	}
}