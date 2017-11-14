#pragma once
#ifndef _COLLIDABLE_H
#define _COLLIDABLE_H

#include "../Units.h"

class Shape;
class Rectangle;

class Collidable {
public:
	virtual ~Collidable() {}
	virtual const units::Coordinate2D& getPosition() const = 0;
	virtual const Shape* const getCollider() const = 0;
};

#endif //_COLLIDABLE_H