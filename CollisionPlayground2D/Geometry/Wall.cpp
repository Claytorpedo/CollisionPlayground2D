#include "Wall.h"

Wall::Wall(const ShapeContainer& shape) : shape_(shape) {}
Wall::Wall(const ShapeContainer& shape, const units::Coordinate2D& position) : shape_(shape), position_(position) {}
Wall::~Wall() {}
const units::Coordinate2D& Wall::getPosition() const {
	return position_;
}
const ShapeContainer& Wall::getCollider() const{
	return shape_;
}