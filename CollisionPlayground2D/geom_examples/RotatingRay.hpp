#ifndef INCLUDE_GAME_ROTATING_RAY_HPP
#define INCLUDE_GAME_ROTATING_RAY_HPP

#include "../units.hpp"

#include <Geometry2D/Geometry.hpp>

class Input;
namespace game {
class RotatingRay {
public:
	static const game::Velocity MAX_ROT_SPEED;
	static const game::Acceleration ROT_ACCEL;
	static const game::Acceleration ROT_DECEL;

	RotatingRay();
	RotatingRay(ctp::Ray ray);
	~RotatingRay() = default;

	void receiveInput(const Input& input);
	void update(const game::MS elapsedTime);

	void rotateCCW();
	void rotateCW();
	void stopRotating();

	const ctp::Ray& getRay() const;
private:
	ctp::Ray ray_;
	game::Acceleration rot_accel_;
	game::Velocity rot_vel_;

	void _rotate();
};
}

#endif // INCLUDE_GAME_ROTATING_RAY_HPP
