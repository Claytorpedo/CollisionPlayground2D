#pragma once
#ifndef _SHAPE_CONTAINER_H
#define _SHAPE_CONTAINER_H

#include "Shape.h"
#include "Polygon.h"
#include "Rectangle.h"

struct ShapeContainer {
	enum class ShapeType {
		UNKNOWN,
		RECTANGLE,
		POLYGON,
	};
	const ShapeType type;
	union {
		const Polygon poly;
		const Rectangle rect;
	};
	const Shape& shape;
	ShapeContainer(const Polygon& p)   : type(ShapeType::POLYGON), poly(p), shape(p) {}
	ShapeContainer(const Rectangle& r) : type(ShapeType::RECTANGLE), rect(r), shape(r) {}
};

#endif // _SHAPE_CONTAINER_H
