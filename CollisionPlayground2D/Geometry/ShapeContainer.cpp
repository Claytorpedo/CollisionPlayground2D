#include "ShapeContainer.h"
#include "Shape.h"
#include "Polygon.h"
#include "Rectangle.h"

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

ShapeContainer::ShapeContainer(const ShapeContainer& other) : type_(other.type_) {
	__CLONE_SHAPE(other);
}
ShapeContainer::ShapeContainer(ShapeContainer&& other) : type_(other.type_) {
	other.shape_ = nullptr;
	__MOVE_SHAPE(other);
}
ShapeContainer& ShapeContainer::operator=(const ShapeContainer& other) {
	if (this == &other)
		return *this;
	type_ = other.type_;
	__CLONE_SHAPE(other);
	return *this;
}
ShapeContainer& ShapeContainer::operator=(ShapeContainer&& other) {
	if (this == &other)
		return *this;
	type_ = other.type_;
	other.shape_ = nullptr;
	__MOVE_SHAPE(other);
}
#undef __CLONE_SHAPE
#undef __MOVE_SHAPE