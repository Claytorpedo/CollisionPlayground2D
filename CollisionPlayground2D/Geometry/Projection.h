#pragma once
#ifndef _PROJECTION_H
#define _PROJECTION_H

#include "Units.h"

// Simple struct for containing a min and max value.
struct Projection {
	units::Coordinate min, max;
	Projection() : min(0), max(0) {}
	Projection(units::Coordinate min, units::Coordinate max) : min(min), max(max) {}
	inline Projection& operator+=(const units::Coordinate& v) { min += v; max += v; return *this; }
};

#endif // _PROJECTION_H