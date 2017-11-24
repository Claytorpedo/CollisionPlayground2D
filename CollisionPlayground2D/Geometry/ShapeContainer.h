#pragma once
#ifndef _SHAPE_CONTAINER_H
#define _SHAPE_CONTAINER_H

#include "Shape.h"
#include "Polygon.h"
#include "Rectangle.h"

class ShapeContainer {
public:
	enum class ShapeType {
		RECTANGLE,
		POLYGON,
	};
	~ShapeContainer() { delete shape_; }
	ShapeContainer(Polygon* const p)   : type_(ShapeType::POLYGON), poly_(p), shape_(poly_) {}
	ShapeContainer(Rectangle* const r) : type_(ShapeType::RECTANGLE), rect_(r), shape_(rect_) {}
	ShapeContainer(const ShapeContainer& other);
	ShapeContainer(ShapeContainer&& other);
	ShapeContainer& operator=(const ShapeContainer& other);
	ShapeContainer& operator=(ShapeContainer&& other);

	inline const ShapeType& type()  const { return type_; }
	inline const Shape&     shape() const { return *shape_; }
	inline const Rectangle& rect()  const { return *rect_; }
	inline const Polygon&   poly()  const { return *poly_; }
private:
	ShapeType type_;
	union {
		Polygon* poly_;
		Rectangle* rect_;
	};
	Shape* shape_;
};

#endif // _SHAPE_CONTAINER_H
