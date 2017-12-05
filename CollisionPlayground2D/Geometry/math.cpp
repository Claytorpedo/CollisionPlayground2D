#include "math.hpp"

#include "Ray.hpp"
#include "units.hpp"

namespace geom {
	namespace math {
		Coord2 closestPointOnLine(const Ray& ray, const Coord2& point) {
			return ray.origin + (point - ray.origin).dot(ray.dir) * ray.dir;
		}
		Coord2 closestPointOnLine(const Coord2& line1, const Coord2& line2, const Coord2& point) {
			const Coord2 dir(line2 - line1); // Unnormalized direction.
			return line1 + ((point - line1).dot(dir) / dir.magnitude2()) * dir;
		}
	}
}
