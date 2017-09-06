#include "catch.hpp"

#include <vector>

#include "../Geometry/CollisionMath.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Rectangle.h"
#include "../Units.h"
#include "../Constants.h"

using namespace units;

SCENARIO("A moving polygon may collide with a stationary one.", "[poly][collision]") {
	GIVEN("The collider is a triangle.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1,1), Coordinate2D(1,0) };
		Polygon collider(points);
		GIVEN("The stationary polygon is a triangle.") {
			std::vector<Coordinate2D> tri = { Coordinate2D(-1, -2), Coordinate2D(1, 0), Coordinate2D(3, -1) };
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
		GIVEN("The stationary polygon is an octogon.") {
			std::vector<Coordinate2D> oct = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
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
			std::vector<Coordinate2D> arb = { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
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
			std::vector<Coordinate2D> tri = { Coordinate2D(-1, -2), Coordinate2D(1, 0), Coordinate2D(3, -1) };
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
		GIVEN("The stationary polygon is an octogon.") {
			std::vector<Coordinate2D> oct = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
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
			std::vector<Coordinate2D> arb = { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
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
	GIVEN("The collider is an octogon.") {
		std::vector<Coordinate2D> points = { Coordinate2D(0,0.5f), Coordinate2D(0.2f,0.8f), Coordinate2D(0.5f,1), Coordinate2D(0.8f,0.8f),
			Coordinate2D(1,0.5f), Coordinate2D(0.8f,0.2f), Coordinate2D(0.5f,0), Coordinate2D(0.2f,0.2f) };
		Polygon collider(points);
		GIVEN("The stationary polygon is a triangle.") {
			std::vector<Coordinate2D> tri = { Coordinate2D(-1, -2), Coordinate2D(0.5f, 0), Coordinate2D(3, -1) };
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
		GIVEN("The stationary polygon is an octogon.") {
			std::vector<Coordinate2D> oct = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
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
			std::vector<Coordinate2D> arb = { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
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
		std::vector<Coordinate2D> points = { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
		Polygon collider(points);
		GIVEN("The stationary polygon is a triangle.") {
			std::vector<Coordinate2D> tri = { Coordinate2D(1, -2), Coordinate2D(2, -3), Coordinate2D(1, -3) };
			Polygon stationary(tri);
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
		GIVEN("The stationary polygon is an octogon.") {
			std::vector<Coordinate2D> oct = { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
				Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
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
			std::vector<Coordinate2D> arb = { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
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
