#include "geom_math.h"

#include "Ray.h"
#include "Units.h"

namespace geom_math {
	units::Coordinate2D closestPointOnLine(const Ray& ray, const units::Coordinate2D& point) {
		return ray.origin + (point - ray.origin).dot(ray.dir) * ray.dir;
	}
	units::Coordinate2D closestPointOnLine(const units::Coordinate2D& line1, const units::Coordinate2D& line2, const units::Coordinate2D& point) {
		const units::Coordinate2D dir(line2 - line1); // Unnormalized direction.
		return line1 + ((point - line1).dot(dir) / dir.magnitude2()) * dir;
	}
}
