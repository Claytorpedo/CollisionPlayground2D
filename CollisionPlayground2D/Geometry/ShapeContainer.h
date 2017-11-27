#pragma once
#ifndef _SHAPE_CONTAINER_H
#define _SHAPE_CONTAINER_H

#include "Shape.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "Circle.h"

enum class ShapeType {
	EMPTY,
	RECTANGLE,
	POLYGON,
	CIRCLE,
};

class ShapeContainer {
public:
	~ShapeContainer() { delete shape_; }
	ShapeContainer()                            : type_(ShapeType::EMPTY),                              shape_(nullptr) {}
	explicit ShapeContainer(Polygon* const p)   : type_(ShapeType::POLYGON),   poly_(p),                shape_(poly_) {}
	explicit ShapeContainer(Rectangle* const r) : type_(ShapeType::RECTANGLE), rect_(r),                shape_(rect_) {}
	explicit ShapeContainer(Circle* const c)    : type_(ShapeType::CIRCLE),    circle_(c),              shape_(circle_) {}
	ShapeContainer(const Polygon& p)            : type_(ShapeType::POLYGON),   poly_(new Polygon(p)),   shape_(poly_) {}
	ShapeContainer(const Rectangle& r)          : type_(ShapeType::RECTANGLE), rect_(new Rectangle(r)), shape_(rect_) {}
	ShapeContainer(const Circle& c)             : type_(ShapeType::CIRCLE),    circle_(new Circle(c)),  shape_(circle_) {}
	ShapeContainer(const ShapeContainer& other);
	ShapeContainer(ShapeContainer&& other);
	ShapeContainer& operator=(const ShapeContainer& other);
	ShapeContainer& operator=(ShapeContainer&& other);

	inline const ShapeType& type()   const { return type_; }
	inline const Shape&     shape()  const { return *shape_; }
	inline const Rectangle& rect()   const { return *rect_; }
	inline const Polygon&   poly()   const { return *poly_; }
	inline const Circle&    circle() const { return *circle_; }
private:
	ShapeType type_;
	union {
		Polygon*   poly_;
		Rectangle* rect_;
		Circle*    circle_;
	};
	Shape* shape_;
};

#endif // _SHAPE_CONTAINER_H
