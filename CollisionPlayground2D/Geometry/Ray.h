#ifndef _GEOM_RAY_H
#define _GEOM_RAY_H

#include "Units.h"

namespace geom {
	class Ray {
	public:
		Coord2 origin, dir;

		Ray() : origin(), dir() {}
		// Dir should be a normalized vector.
		Ray(Coord2 origin, Coord2 dir) : origin(origin), dir(dir) {}
		Ray(gFloat origin_x, gFloat origin_y, gFloat dir_x, gFloat dir_y)
			: origin(origin_x, origin_y), dir(dir_x, dir_y) {}
		Ray(const Ray& o) : origin(o.origin), dir(o.dir) {}
	};
}
#endif // _GEOM_RAY_H