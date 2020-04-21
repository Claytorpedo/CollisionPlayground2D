#include "Mover.hpp"

#include "../Input.hpp"

namespace game {
const game::Velocity     Mover::MAX_SPEED = 0.35f;
const game::Velocity     Mover::MAX_DIAGONAL_SPEED = Mover::MAX_SPEED * (game::Velocity)std::sin(ctp::constants::PI / 4.0f);
const game::Acceleration Mover::ACCELERATION = 0.0025f;
const game::Acceleration Mover::DECELERATION = 0.004f;

void Mover::_init() {
	if (collider_.type() == ctp::ShapeType::POLYGON)
		collider_.poly().computeNormals();
}

Mover::Mover(ctp::Movable::CollisionType type, const ctp::ShapeContainer& collider, const ctp::Coord2& position) : Movable(type), collider_(collider), position_(position) {
	_init();
}
Mover::Mover(const ctp::ShapeContainer& collider, const ctp::Coord2& position) : collider_(collider), position_(position) {
	_init();
}

void Mover::update(const game::MS elapsedTime, const ctp::CollisionMap& map) {
	const game::Velocity maxSpeed = (!ctp::math::almostZero(acceleration_.x) && !ctp::math::almostZero(acceleration_.y)) ? MAX_DIAGONAL_SPEED : MAX_SPEED;
	_update_position(elapsedTime, maxSpeed, map);
}

void Mover::_update_position(const game::MS elapsedTime, const game::Velocity maxSpeed, const ctp::CollisionMap& map) {
	velocity_ += acceleration_ * (ctp::gFloat)(elapsedTime);
	velocity_.x = std::clamp(velocity_.x, -maxSpeed, maxSpeed);
	velocity_.y = std::clamp(velocity_.y, -maxSpeed, maxSpeed);
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
	const ctp::Coord2 delta(velocity_ * (ctp::gFloat)(elapsedTime));
	position_ = Movable::move(collider_, position_, delta, map);
}

bool Mover::onCollision(ctp::Movable::CollisionInfo& info) {
	// Project velocity against the normal, losing speed from the collision.
	velocity_ = velocity_.project(info.normal.perpCW());
	return true;
}

void Mover::setPosition(const ctp::Coord2& position) {
	position_ = position;
}

ctp::Coord2 Mover::getPosition() const {
	return position_;
}

ctp::ConstShapeRef Mover::getCollider() const {
	return collider_;
}

void Mover::receiveInput(const Input& input) {
	// Horizontal movement.
	if ((input.isKeyHeld(SDLK_LEFT) || input.isKeyHeld(SDLK_a)) && (input.isKeyHeld(SDLK_RIGHT) || input.isKeyHeld(SDLK_d))) {
		stopMovingHorizontal();
	} else if (input.isKeyHeld(SDLK_LEFT) || input.isKeyHeld(SDLK_a)) {
		moveLeft();
	} else if (input.isKeyHeld(SDLK_RIGHT) || input.isKeyHeld(SDLK_d)) {
		moveRight();
	} else {
		stopMovingHorizontal();
	}
	// Vertical movement.
	if ((input.isKeyHeld(SDLK_UP) || input.isKeyHeld(SDLK_w)) && (input.isKeyHeld(SDLK_DOWN) || input.isKeyHeld(SDLK_s))) {
		stopMovingVertical();
	} else if (input.isKeyHeld(SDLK_UP) || input.isKeyHeld(SDLK_w)) {
		moveUp();
	} else if (input.isKeyHeld(SDLK_DOWN) || input.isKeyHeld(SDLK_s)) {
		moveDown();
	} else {
		stopMovingVertical();
	}
}

void Mover::moveLeft() { acceleration_.x = -ACCELERATION; }
void Mover::moveRight() { acceleration_.x = ACCELERATION; }
void Mover::moveUp() { acceleration_.y = -ACCELERATION; }
void Mover::moveDown() { acceleration_.y = ACCELERATION; }
void Mover::stopMovingHorizontal() { acceleration_.x = 0.0f; }
void Mover::stopMovingVertical() { acceleration_.y = 0.0f; }
void Mover::stopMoving() { acceleration_ = game::Acceleration2D(0, 0); }
}
