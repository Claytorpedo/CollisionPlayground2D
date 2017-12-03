#pragma once
#ifndef _GEOM_PROJECTION_H
#define _GEOM_PROJECTION_H

#include "Units.h"

// Simple struct for containing a min and max value.
namespace geom {
	struct Projection {
		gFloat min, max;
		Projection() : min(0), max(0) {}
		Projection(gFloat min, gFloat max) : min(min), max(max) {}
		inline Projection& operator+=(const gFloat& v) { min += v; max += v; return *this; }
	};
}
#endif // _GEOM_PROJECTION_H