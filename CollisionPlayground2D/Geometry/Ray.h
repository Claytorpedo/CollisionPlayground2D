#ifndef _RAY_H
#define _RAY_H

#include "../Units.h"

class Ray {
public:
	units::Coordinate2D origin, dir;

	Ray() : origin(), dir() {}
	// Dir should be a normalized vector.
	Ray(units::Coordinate2D origin, units::Coordinate2D dir) : origin(origin), dir(dir) {}
	Ray(units::Coordinate origin_x, units::Coordinate origin_y, units::Coordinate dir_x, units::Coordinate dir_y) 
		: origin(origin_x, origin_y), dir(dir_x, dir_y) {}
	Ray(const Ray& o) : origin(o.origin), dir(o.dir) {}
};

#endif // _RAY_H