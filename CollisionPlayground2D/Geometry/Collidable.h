#pragma once
#ifndef _GEOM_COLLIDABLE_H
#define _GEOM_COLLIDABLE_H

#include "Units.h"

class ShapeContainer;

namespace geom {
	class Collidable {
	public:
		virtual ~Collidable() {}
		virtual const Coord2& getPosition() const = 0;
		virtual const ShapeContainer& getCollider() const = 0;
	};
}
#endif //_GEOM_COLLIDABLE_H