#pragma once
#ifndef _COLLIDABLE_H
#define _COLLIDABLE_H

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
#endif //_COLLIDABLE_H