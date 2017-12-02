#include "catch.hpp"
#include "definitions.h"

#include "../Geometry/geom_math.h"

#include "../Geometry/Ray.h"
#include "../Geometry/Units.h"

SCENARIO("Getting the closest point on a line to a point, from a line made with two points.", "[geom_math]") {
	GIVEN("A horizontal line through origin.") {
		units::Coordinate2D l1(0, 0), l2(2, 0);
		GIVEN("A point on the line.") {
			units::Coordinate2D p(10, 0);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(10, 0);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			units::Coordinate2D p(10, 10);
			THEN("The closest point is the x-coordinate.") {
				units::Coordinate2D expected(10, 0);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			units::Coordinate2D p(10, -10);
			THEN("The closest point is the x-coordinate.") {
				units::Coordinate2D expected(10, 0);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line through orign.") {
		units::Coordinate2D l1(0, 0), l2(0, 10);
		GIVEN("A point on the line.") {
			units::Coordinate2D p(0, -10);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(0, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			units::Coordinate2D p(-10, -10);
			THEN("The closest point is the y-coordinate.") {
				units::Coordinate2D expected(0, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			units::Coordinate2D p(10, -10);
			THEN("The closest point is the y-coordinate.") {
				units::Coordinate2D expected(0, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line through origin.") {
		units::Coordinate2D l1(0, 0), l2(2, 2);
		GIVEN("A point on the line.") {
			units::Coordinate2D p(10, 10);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(10, 10);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			units::Coordinate2D p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				units::Coordinate2D expected(8, 8);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			units::Coordinate2D p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				units::Coordinate2D expected(13, 13);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A horizontal line not through origin.") {
		units::Coordinate2D l1(5, 5), l2(-2, 5);
		GIVEN("A point on the line.") {
			units::Coordinate2D p(10, 5);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(10, 5);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			units::Coordinate2D p(10, 10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				units::Coordinate2D expected(10, 5);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			units::Coordinate2D p(10, -10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				units::Coordinate2D expected(10, 5);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line not through orign.") {
		units::Coordinate2D l1(-1, 0), l2(-1, -10);
		GIVEN("A point on the line.") {
			units::Coordinate2D p(-1, -10);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(-1, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			units::Coordinate2D p(-10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				units::Coordinate2D expected(-1, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			units::Coordinate2D p(10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				units::Coordinate2D expected(-1, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line not through origin.") {
		units::Coordinate2D l1(5, 0), l2(0, -5);
		GIVEN("A point on the line.") {
			units::Coordinate2D p(2, -3);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(2, -3);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			units::Coordinate2D p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				units::Coordinate2D expected(10.5f, 5.5f);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			units::Coordinate2D p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				units::Coordinate2D expected(15.5f, 10.5f);
				units::Coordinate2D result = geom_math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
}

SCENARIO("Getting the closest point on a line to a point, from a line made by a ray.", "[geom_math]") {
	GIVEN("A horizontal line through origin.") {
		Ray r(units::Coordinate2D(0, 0), units::Coordinate2D(1, 0));
		GIVEN("A point on the line.") {
			units::Coordinate2D p(10, 0);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(10, 0);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			units::Coordinate2D p(10, 10);
			THEN("The closest point is the x-coordinate.") {
				units::Coordinate2D expected(10, 0);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			units::Coordinate2D p(10, -10);
			THEN("The closest point is the x-coordinate.") {
				units::Coordinate2D expected(10, 0);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line through orign.") {
		Ray r(units::Coordinate2D(0, 0), units::Coordinate2D(0, 1));
		GIVEN("A point on the line.") {
			units::Coordinate2D p(0, -10);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(0, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			units::Coordinate2D p(-10, -10);
			THEN("The closest point is the y-coordinate.") {
				units::Coordinate2D expected(0, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			units::Coordinate2D p(10, -10);
			THEN("The closest point is the y-coordinate.") {
				units::Coordinate2D expected(0, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line through origin.") {
		Ray r(units::Coordinate2D(0, 0), units::Coordinate2D(-1, -1).normalize());
		GIVEN("A point on the line.") {
			units::Coordinate2D p(10, 10);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(10, 10);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			units::Coordinate2D p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				units::Coordinate2D expected(8, 8);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			units::Coordinate2D p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				units::Coordinate2D expected(13, 13);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A horizontal line not through origin.") {
		Ray r(units::Coordinate2D(5, 5), units::Coordinate2D(-1, 0));
		GIVEN("A point on the line.") {
			units::Coordinate2D p(10, 5);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(10, 5);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			units::Coordinate2D p(10, 10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				units::Coordinate2D expected(10, 5);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			units::Coordinate2D p(10, -10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				units::Coordinate2D expected(10, 5);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line not through orign.") {
		Ray r(units::Coordinate2D(-1, 0), units::Coordinate2D(0, 1));
		GIVEN("A point on the line.") {
			units::Coordinate2D p(-1, -10);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(-1, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			units::Coordinate2D p(-10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				units::Coordinate2D expected(-1, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			units::Coordinate2D p(10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				units::Coordinate2D expected(-1, -10);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line not through origin.") {
		Ray r(units::Coordinate2D(5, 0), units::Coordinate2D(1, 1).normalize());
		GIVEN("A point on the line.") {
			units::Coordinate2D p(2, -3);
			THEN("The closest point is the point itself.") {
				units::Coordinate2D expected(2, -3);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			units::Coordinate2D p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				units::Coordinate2D expected(10.5f, 5.5f);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			units::Coordinate2D p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				units::Coordinate2D expected(15.5f, 10.5f);
				units::Coordinate2D result = geom_math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
}