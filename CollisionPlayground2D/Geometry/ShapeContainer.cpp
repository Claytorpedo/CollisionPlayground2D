#include "ShapeContainer.h"
#include "Shape.h"
#include "Polygon.h"
#include "Rectangle.h"

#define __CLONE_SHAPE(other) \
	switch (type_) { \
		case(ShapeType::RECTANGLE): rect_ = new Rectangle(*other.rect_);  shape_ = rect_;   break; \
		case(ShapeType::POLYGON):   poly_ = new Polygon(*other.poly_);    shape_ = poly_;   break; \
		case(ShapeType::CIRCLE):    circle_ = new Circle(*other.circle_); shape_ = circle_; break; \
		case(ShapeType::EMPTY):     shape_ = nullptr; \
	}
#define __MOVE_SHAPE(other) \
	other.shape_ = nullptr; \
	switch (type_) { \
		case(ShapeType::RECTANGLE): rect_ = std::move(other.rect_);     shape_ = rect_;   break; \
		case(ShapeType::POLYGON):   poly_ = std::move(other.poly_);     shape_ = poly_;   break; \
		case(ShapeType::CIRCLE):    circle_ = std::move(other.circle_); shape_ = circle_; break; \
		case(ShapeType::EMPTY):     shape_ = nullptr; \
	}

ShapeContainer::ShapeContainer(const ShapeContainer& other) : type_(other.type_) {
	__CLONE_SHAPE(other);
}
ShapeContainer::ShapeContainer(ShapeContainer&& other) : type_(other.type_) {
	__MOVE_SHAPE(other);
}
ShapeContainer& ShapeContainer::operator=(const ShapeContainer& other) {
	if (this == &other)
		return *this;
	delete shape_;
	type_ = other.type_;
	__CLONE_SHAPE(other);
	return *this;
}
ShapeContainer& ShapeContainer::operator=(ShapeContainer&& other) {
	if (this == &other)
		return *this;
	delete shape_;
	type_ = other.type_;
	__MOVE_SHAPE(other);
	return *this;
}
#undef __CLONE_SHAPE
#undef __MOVE_SHAPE