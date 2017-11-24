#pragma once
#ifndef _SHAPE_CONTAINER_H
#define _SHAPE_CONTAINER_H

#include "Shape.h"
#include "Polygon.h"
#include "Rectangle.h"

#include <iostream>

class ShapeContainer {
public:
	enum class ShapeType {
		RECTANGLE,
		POLYGON,
	};
	~ShapeContainer() { delete shape_; }
	ShapeContainer(Polygon* const p)   : type_(ShapeType::POLYGON), poly_(p), shape_(poly_) {}
	ShapeContainer(Rectangle* const r) : type_(ShapeType::RECTANGLE), rect_(r), shape_(rect_) {}

#define __CLONE_SHAPE(other) \
	switch (type_) { \
		case(ShapeType::RECTANGLE): rect_ = new Rectangle(*other.rect_); shape_ = rect_;  break; \
		case(ShapeType::POLYGON):   poly_ = new Polygon(*other.poly_);   shape_ = poly_;  break; \
	}
#define __MOVE_SHAPE(other) \
	switch (type_) { \
		case(ShapeType::RECTANGLE): rect_ = std::move(other.rect_); shape_ = rect_;  break; \
		case(ShapeType::POLYGON):   poly_ = std::move(other.poly_); shape_ = poly_;  break; \
	}

	ShapeContainer(const ShapeContainer& other) : type_(other.type_) {
		__CLONE_SHAPE(other)
	}
	ShapeContainer(ShapeContainer&& other) : type_(other.type_) {
		other.shape_ = nullptr;
		__MOVE_SHAPE(other);
	}
	ShapeContainer& operator=(const ShapeContainer& other) {
		if (this == &other)
			return *this;
		type_ = other.type_;
		__CLONE_SHAPE(other);
		return *this;
	}
	ShapeContainer& operator=(ShapeContainer&& other) {
		if (this == &other)
			return *this;
		type_ = other.type_;
		other.shape_ = nullptr;
		__MOVE_SHAPE(other);
	}

	inline const ShapeType& type()  const { return type_;  }
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

#undef __CLONE_SHAPE
#undef __MOVE_SHAPE

#endif // _SHAPE_CONTAINER_H
