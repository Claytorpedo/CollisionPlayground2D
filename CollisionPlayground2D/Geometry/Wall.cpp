#include "Wall.h"

Wall::Wall(Shape* shape) : shape_(shape) {}
Wall::Wall(Shape* shape, const units::Coordinate2D& position) : shape_(shape), position_(position) {}
Wall::~Wall() {
	delete shape_;
}
const units::Coordinate2D& Wall::getPosition() const {
	return position_;
}
const Shape* const Wall::getCollider() const{
	return shape_;
}