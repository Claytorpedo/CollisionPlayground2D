#include "CollisionMath.h"

#include "../Units.h"
#include "../Util.h"
#include "../Constants.h"

#include "IntersectionMath.h"
#include "LineSegment.h"
#include "Ray.h"
#include "Shape.h"
#include "Rectangle.h"
#include "Polygon.h"

namespace collision_math {
	bool collides(const Polygon& collider, const units::Coordinate2D delta, const Polygon& other) {
		return isect::intersects(collider.clipExtend(delta), other);
	}

	// Test for collision, and if so find the collision normal and depth/amount of overlap.
	bool collides(const Polygon& collider, const units::Coordinate2D delta, const Polygon& other,
		units::Coordinate2D& out_normal, units::Coordinate2D& out_depth) {
			return true;
	}
	
}