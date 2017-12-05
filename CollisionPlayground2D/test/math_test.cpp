#include "catch.hpp"
#include "definitions.h"

#include "../Geometry/math.h"

#include "../Geometry/Ray.h"
#include "../Geometry/units.h"

using geom::Ray;
using geom::Coord2;

SCENARIO("Getting the closest point on a line to a point, from a line made with two points.", "[geom_math]") {
	GIVEN("A horizontal line through origin.") {
		Coord2 l1(0, 0), l2(2, 0);
		GIVEN("A point on the line.") {
			Coord2 p(10, 0);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the x-coordinate.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the x-coordinate.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line through orign.") {
		Coord2 l1(0, 0), l2(0, 10);
		GIVEN("A point on the line.") {
			Coord2 p(0, -10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest point is the y-coordinate.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the y-coordinate.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line through origin.") {
		Coord2 l1(0, 0), l2(2, 2);
		GIVEN("A point on the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(8, 8);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(13, 13);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A horizontal line not through origin.") {
		Coord2 l1(5, 5), l2(-2, 5);
		GIVEN("A point on the line.") {
			Coord2 p(10, 5);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line not through orign.") {
		Coord2 l1(-1, 0), l2(-1, -10);
		GIVEN("A point on the line.") {
			Coord2 p(-1, -10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line not through origin.") {
		Coord2 l1(5, 0), l2(0, -5);
		GIVEN("A point on the line.") {
			Coord2 p(2, -3);
			THEN("The closest point is the point itself.") {
				Coord2 expected(2, -3);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(10.5f, 5.5f);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(15.5f, 10.5f);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
}

SCENARIO("Getting the closest point on a line to a point, from a line made by a ray.", "[geom_math]") {
	GIVEN("A horizontal line through origin.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		GIVEN("A point on the line.") {
			Coord2 p(10, 0);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the x-coordinate.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the x-coordinate.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line through orign.") {
		Ray r(Coord2(0, 0), Coord2(0, 1));
		GIVEN("A point on the line.") {
			Coord2 p(0, -10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest point is the y-coordinate.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the y-coordinate.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line through origin.") {
		Ray r(Coord2(0, 0), Coord2(-1, -1).normalize());
		GIVEN("A point on the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(8, 8);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(13, 13);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A horizontal line not through origin.") {
		Ray r(Coord2(5, 5), Coord2(-1, 0));
		GIVEN("A point on the line.") {
			Coord2 p(10, 5);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line not through orign.") {
		Ray r(Coord2(-1, 0), Coord2(0, 1));
		GIVEN("A point on the line.") {
			Coord2 p(-1, -10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line not through origin.") {
		Ray r(Coord2(5, 0), Coord2(1, 1).normalize());
		GIVEN("A point on the line.") {
			Coord2 p(2, -3);
			THEN("The closest point is the point itself.") {
				Coord2 expected(2, -3);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(10.5f, 5.5f);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(15.5f, 10.5f);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
}