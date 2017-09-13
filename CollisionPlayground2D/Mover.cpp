#include "Mover.h"

#include <vector>

#include "Geometry/Polygon.h"
#include "Util.h"

void Mover::update(const units::MS elapsedTime, const std::vector<Polygon> polys) {
	const units::Velocity maxSpeed = (!util::almostZero(acceleration_.x) && !util::almostZero(acceleration_.y)) ? mover::MAX_DIAGONAL_SPEED : mover::MAX_SPEED;
	update_position(elapsedTime, maxSpeed, polys);
}

void Mover::update_position(const units::MS elapsedTime, const units::Velocity maxSpeed, const std::vector<Polygon> polys) {
	velocity_ += acceleration_ * (units::Coordinate)(elapsedTime);
	velocity_.x = util::clamp(velocity_.x, -maxSpeed, maxSpeed);
	velocity_.y = util::clamp(velocity_.y, -maxSpeed, maxSpeed);
	if (acceleration_.x == 0 && velocity_.x != 0) {
		const bool isPos(velocity_.x > 0);
		velocity_.x += (isPos ? -1.0f : 1.0f) * mover::DECELERATION * elapsedTime;
		velocity_.x = isPos ? (velocity_.x < 0 ? 0.0f : velocity_.x) : (velocity_.x > 0 ? 0.0f : velocity_.x);
	}
	if (acceleration_.y == 0 && velocity_.y != 0) {
		const bool isPos(velocity_.y > 0);
		velocity_.y += (isPos ? -1.0f : 1.0f) * mover::DECELERATION * elapsedTime;
		velocity_.y = isPos ? (velocity_.y < 0 ? 0.0f : velocity_.y) : (velocity_.y > 0 ? 0.0f : velocity_.y);
	}
	const units::Coordinate2D delta(velocity_*(units::Coordinate)(elapsedTime));
	position_ = Collidable::move(position_, collider_, delta, polys);
}

void Mover::setPosition(units::Coordinate2D position) {
	position_ = position;
}

Polygon Mover::getCollider() const {
	return Polygon::translate(collider_, position_);
}

void Mover::moveLeft()  { acceleration_.x = -mover::ACCELERATION; }
void Mover::moveRight() { acceleration_.x =  mover::ACCELERATION; }
void Mover::moveUp()    { acceleration_.y = -mover::ACCELERATION; }
void Mover::moveDown()  { acceleration_.y =  mover::ACCELERATION; }
void Mover::stopMovingHorizontal() { acceleration_.x = 0.0f; }
void Mover::stopMovingVertical()   { acceleration_.y = 0.0f; }
void Mover::stopMoving()           { acceleration_ = units::Acceleration2D(0, 0); }