#include "ShapeContainer.hpp"
#include "Shape.hpp"
#include "Polygon.hpp"
#include "Rectangle.hpp"

#define GEOM_SHAPE_CONTAINER_CLONE_SHAPE(other) \
	switch (type_) { \
		case(ShapeType::RECTANGLE): rect_ = new Rect(*other.rect_);  shape_ = rect_;   break; \
		case(ShapeType::POLYGON):   poly_ = new Polygon(*other.poly_);    shape_ = poly_;   break; \
		case(ShapeType::CIRCLE):    circle_ = new Circle(*other.circle_); shape_ = circle_; break; \
		case(ShapeType::EMPTY):     shape_ = nullptr; \
	}
#define GEOM_SHAPE_CONTAINER_MOVE_SHAPE(other) \
	other.shape_ = nullptr; \
	switch (type_) { \
		case(ShapeType::RECTANGLE): rect_ = std::move(other.rect_);     shape_ = rect_;   break; \
		case(ShapeType::POLYGON):   poly_ = std::move(other.poly_);     shape_ = poly_;   break; \
		case(ShapeType::CIRCLE):    circle_ = std::move(other.circle_); shape_ = circle_; break; \
		case(ShapeType::EMPTY):     shape_ = nullptr; \
	}

namespace geom {
	ShapeContainer::ShapeContainer(const ShapeContainer& other) : type_(other.type_) {
		GEOM_SHAPE_CONTAINER_CLONE_SHAPE(other);
	}
	ShapeContainer::ShapeContainer(ShapeContainer&& other) : type_(other.type_) {
		GEOM_SHAPE_CONTAINER_MOVE_SHAPE(other);
	}
	ShapeContainer& ShapeContainer::operator=(const ShapeContainer& other) {
		if (this == &other)
			return *this;
		delete shape_;
		type_ = other.type_;
		GEOM_SHAPE_CONTAINER_CLONE_SHAPE(other);
		return *this;
	}
	ShapeContainer& ShapeContainer::operator=(ShapeContainer&& other) {
		if (this == &other)
			return *this;
		delete shape_;
		type_ = other.type_;
		GEOM_SHAPE_CONTAINER_MOVE_SHAPE(other);
		return *this;
	}
}
