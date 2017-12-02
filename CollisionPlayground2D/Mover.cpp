#include "Mover.h"

#include "Geometry/ShapeContainer.h"
#include "Geometry/Polygon.h"
#include "Geometry/CollisionMap.h"
#include "Geometry/Util.h"

const units::Velocity     Mover::MAX_SPEED = 0.3f;
const units::Velocity     Mover::MAX_DIAGONAL_SPEED = Mover::MAX_SPEED * (units::Velocity)std::sin(constants::PI / 4.0f);
const units::Acceleration Mover::ACCELERATION = 0.0025f;
const units::Acceleration Mover::DECELERATION = 0.004f;

void Mover::_init() const {
	if (collider_.type() == ShapeType::POLYGON)
		collider_.poly().computeNormals();
}

Mover::Mover(Movable::CollisionType type, const ShapeContainer& collider, const units::Coordinate2D& position) : Movable(type), collider_(collider), position_(position) {
	_init();
}
Mover::Mover(const ShapeContainer& collider, const units::Coordinate2D& position) : collider_(collider), position_(position) {
	_init();
}

void Mover::update(const units::MS elapsedTime, const CollisionMap* const map) {
	const units::Velocity maxSpeed = (!util::almostZero(acceleration_.x) && !util::almostZero(acceleration_.y)) ? MAX_DIAGONAL_SPEED : MAX_SPEED;
	update_position(elapsedTime, maxSpeed, map);
}

void Mover::update_position(const units::MS elapsedTime, const units::Velocity maxSpeed, const CollisionMap* const map) {
	velocity_ += acceleration_ * (units::Coordinate)(elapsedTime);
	velocity_.x = util::clamp(velocity_.x, -maxSpeed, maxSpeed);
	velocity_.y = util::clamp(velocity_.y, -maxSpeed, maxSpeed);
	if (acceleration_.x == 0 && velocity_.x != 0) {
		const bool isPos(velocity_.x > 0);
		velocity_.x += (isPos ? -1.0f : 1.0f) * DECELERATION * elapsedTime;
		velocity_.x = isPos ? (velocity_.x < 0 ? 0.0f : velocity_.x) : (velocity_.x > 0 ? 0.0f : velocity_.x);
	}
	if (acceleration_.y == 0 && velocity_.y != 0) {
		const bool isPos(velocity_.y > 0);
		velocity_.y += (isPos ? -1.0f : 1.0f) * DECELERATION * elapsedTime;
		velocity_.y = isPos ? (velocity_.y < 0 ? 0.0f : velocity_.y) : (velocity_.y > 0 ? 0.0f : velocity_.y);
	}
	const units::Coordinate2D delta(velocity_*(units::Coordinate)(elapsedTime));
	position_ = Movable::move(collider_, position_, delta, map);
}

void Mover::setPosition(units::Coordinate2D position) {
	position_ = position;
}

const units::Coordinate2D& Mover::getPosition() const {
	return position_;
}

const ShapeContainer& Mover::getCollider() const {
	return collider_;
}

void Mover::moveLeft()  { acceleration_.x = -ACCELERATION; }
void Mover::moveRight() { acceleration_.x =  ACCELERATION; }
void Mover::moveUp()    { acceleration_.y = -ACCELERATION; }
void Mover::moveDown()  { acceleration_.y =  ACCELERATION; }
void Mover::stopMovingHorizontal() { acceleration_.x = 0.0f; }
void Mover::stopMovingVertical()   { acceleration_.y = 0.0f; }
void Mover::stopMoving()           { acceleration_ = units::Acceleration2D(0, 0); }