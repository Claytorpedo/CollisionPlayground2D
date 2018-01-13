#include "Mover.hpp"

#include "../Input.hpp"

namespace game {
	const game::Velocity     Mover::MAX_SPEED = 0.3f;
	const game::Velocity     Mover::MAX_DIAGONAL_SPEED = Mover::MAX_SPEED * (game::Velocity)std::sin(geom::constants::PI / 4.0f);
	const game::Acceleration Mover::ACCELERATION = 0.0025f;
	const game::Acceleration Mover::DECELERATION = 0.004f;

	void Mover::_init() const {
		if (collider_.type() == geom::ShapeType::POLYGON)
			collider_.poly().computeNormals();
	}

	Mover::Mover(geom::Movable::CollisionType type, const geom::ShapeContainer& collider, const geom::Coord2& position) : Movable(type), collider_(collider), position_(position) {
		_init();
	}
	Mover::Mover(const geom::ShapeContainer& collider, const geom::Coord2& position) : collider_(collider), position_(position) {
		_init();
	}

	void Mover::update(const game::MS elapsedTime, const geom::CollisionMap& map) {
		const game::Velocity maxSpeed = (!geom::math::almostZero(acceleration_.x) && !geom::math::almostZero(acceleration_.y)) ? MAX_DIAGONAL_SPEED : MAX_SPEED;
		update_position(elapsedTime, maxSpeed, map);
	}

	void Mover::update_position(const game::MS elapsedTime, const game::Velocity maxSpeed, const geom::CollisionMap& map) {
		velocity_ += acceleration_ * (geom::gFloat)(elapsedTime);
		velocity_.x = geom::math::clamp(velocity_.x, -maxSpeed, maxSpeed);
		velocity_.y = geom::math::clamp(velocity_.y, -maxSpeed, maxSpeed);
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
		const geom::Coord2 delta(velocity_*(geom::gFloat)(elapsedTime));
		position_ = Movable::move(collider_, position_, delta, map);
	}

	void Mover::setPosition(const geom::Coord2& position) {
		position_ = position;
	}

	const geom::Coord2& Mover::getPosition() const {
		return position_;
	}

	const geom::ShapeContainer& Mover::getCollider() const {
		return collider_;
	}

	void Mover::receiveInput(Input& input) {
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
