#include "RotatingRay.hpp"

#include "../Input.hpp"

namespace game {
const game::Velocity RotatingRay::MAX_ROT_SPEED = 0.05f;
const game::Acceleration RotatingRay::ROT_ACCEL = 0.0001f;
const game::Acceleration RotatingRay::ROT_DECEL = 0.0004f;

RotatingRay::RotatingRay() : ray_{ctp::Coord2(0, 0), ctp::Coord2(1, 0)}, rot_accel_(0), rot_vel_(0) {}
RotatingRay::RotatingRay(ctp::Ray ray) : ray_(ray), rot_accel_(0), rot_vel_(0) {}

void RotatingRay::receiveInput(const Input& input) {
	const bool holdingLeftKey = input.isKeyHeld(SDLK_LEFT) || input.isKeyHeld(SDLK_a) || input.isKeyHeld(SDLK_DOWN) || input.isKeyHeld(SDLK_s);
	const bool holdingRightKey = input.isKeyHeld(SDLK_RIGHT) || input.isKeyHeld(SDLK_d) || input.isKeyHeld(SDLK_UP) || input.isKeyHeld(SDLK_w);
	if (holdingLeftKey && holdingRightKey) {
		stopRotating();
	} else if (holdingLeftKey) {
		rotateCW();
	} else if (holdingRightKey) {
		rotateCCW();
	} else {
		stopRotating();
	}
}
void RotatingRay::update(const game::MS elapsedTime) {
	if (rot_accel_ == 0.0f) {
		if (rot_vel_ > 0.0f)
			rot_vel_ = ctp::math::clamp(rot_vel_ - ROT_DECEL * game::Acceleration(elapsedTime), 0.0f, rot_vel_);
		else if (rot_vel_ < 0.0f)
			rot_vel_ = ctp::math::clamp(rot_vel_ + ROT_DECEL * game::Acceleration(elapsedTime), 0.0f, rot_vel_);
	} else {
		rot_vel_ += rot_accel_ * game::Acceleration(elapsedTime);
		rot_vel_ = std::clamp(rot_vel_, -MAX_ROT_SPEED, MAX_ROT_SPEED);
	}
	_rotate();
}

void RotatingRay::rotateCCW() { rot_accel_ = -ROT_ACCEL; }
void RotatingRay::rotateCW() { rot_accel_ = ROT_ACCEL; }
void RotatingRay::stopRotating() { rot_accel_ = 0.0f; }

const ctp::Ray& RotatingRay::getRay() const { return ray_; }

void RotatingRay::_rotate() {
	if (rot_vel_ == 0.0f)
		return;
	ctp::gFloat sine(std::sin(rot_vel_));
	ctp::gFloat cosine(std::cos(rot_vel_));

	ray_.dir = ctp::Coord2(ray_.dir.x * cosine - ray_.dir.y * sine, ray_.dir.x * sine + ray_.dir.y * cosine);
}
}
