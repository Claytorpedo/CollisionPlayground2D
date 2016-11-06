#include <SDL.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

#include "Room.h"
#include "Input.h"
#include "Graphics.h"
#include "Constants.h"
#include "Units.h"
#include "Geometry/LineSegment.h"
#include "Geometry/Rectangle.h"
#include "Geometry/Ray.h"
#include "Geometry/Polygon.h"
#include "Geometry/CollisionMath.h"

void close() {
	SDL_Quit();
}
void closeWithError() {
	std::cout << "Press Enter to close." << std::endl;
	std::cin.ignore();
	close();
}

Polygon genCircle(units::Coordinate2D cen, units::Coordinate radius, Uint8 numSegs=15) {
	// Approximate a circle with line segments.
	std::vector<units::Coordinate2D> vertices(numSegs);
	const units::Coordinate f(2.0f*3.14159265358979323f / static_cast<units::Coordinate>(numSegs));
	for (Uint8 i = 0; i < numSegs; ++i) {
		const units::Coordinate theta(f*static_cast<units::Coordinate>(i));
		const units::Coordinate2D pos(radius * cosf(theta), radius * sinf(theta));
		vertices[numSegs - i - 1] = cen + pos;
	}
	return Polygon(vertices);
}

Polygon getMover(std::vector<Polygon> polys, std::mt19937& twister, const Rectangle& region) {
	Polygon mover = Polygon::generate(twister, region);
	// Ensure that the moving polygon doesn't start inside another polygon (do collision tests until it is put down cleanly).
	while (true) {
		bool isOccupied = false;
		for (std::size_t i = 0; i < polys.size(); ++i) {
			if (isect::intersects(mover, polys[i])) {
				isOccupied = true; // Mover is in an occupied spot.
				mover = Polygon::generate(twister, region); // Try another spot.
				std::cout << "Spot occupied. Trying somewhere else...\n";
				break;
			}
		}
		if (!isOccupied)
			break; // Mover is on a spot free of other polygons.
	}
	std::cout << "Mover has entered the level.\n";
	return mover;
}

// Move the mover polygon by delta.
void move(Polygon& mover, const std::vector<Polygon>& polys, const units::Coordinate2D& delta) {
	if (!delta.isZero()) {
		bool wasCollision = false;
		units::Coordinate deltaMag = delta.magnitude();
		units::Coordinate2D dir = delta/deltaMag; // normalize delta dir.
		units::Coordinate   dist = deltaMag;
		units::Coordinate2D normal;
		// Find the closest collision.
		for (std::size_t i = 0; i < polys.size(); ++i) {
			units::Coordinate testDelta;
			units::Coordinate2D testNorm;
			if (collision_math::collides(mover, dir, deltaMag, polys[i], testNorm, testDelta)) {
				if (!wasCollision) { // First time getting a collision.
					dist = testDelta;
					normal = testNorm;
				} else if (dist > testDelta) { // We got a collision before this. Take the closer one.
					dist = testDelta;
					normal = testNorm;
				}
				wasCollision = true;
			}
		}
		// Move to new location.
		mover = mover.translate(dir*dist);
		units::Coordinate remainingDist = deltaMag - dist; // See if there is any more distance to move.
		std::cout << "remaining dist: " << remainingDist  << " dir: " << dir.x << "," << dir.y << " deltaMag: " << deltaMag << "\n\n";
		if (remainingDist > constants::EPSILON) {
			// Find the deflection vector by projecting along the line perpendicular to the normal.
			const units::Coordinate2D perpNorm(-normal.y, normal.x);
			const units::Coordinate dot(perpNorm.dot(dir * remainingDist));
			const units::Coordinate2D projection(dot*perpNorm.x, dot*perpNorm.y);
			std::cout << "projection: " << projection.x << "," << projection.y << "\n";
			// Continue moving.
			move(mover, polys, projection);
		}
	}
}

int main (int argc, char* args[]) {
	Room room;
	Input input;
	units::MS currentTime, previousTime, elapsedTime;
	Graphics graphics;

	if (!graphics.init()) {
		std::cerr << "Error: Failed to initialize graphics.\n";
		closeWithError();
		return -1;
	}
	std::random_device rd;
	std::mt19937 twister(rd());
	Rectangle region(util::tileToCoord(10), util::tileToCoord(5), constants::SCREEN_WIDTH - util::tileToCoord(20), constants::SCREEN_HEIGHT - util::tileToCoord(10));
		
	std::size_t numPolys(20);
	std::vector<Polygon> polys;
	polys.reserve(numPolys);
	for (std::size_t i = 0; i < numPolys; ++i) {
		polys.push_back(Polygon::generate(twister, region));
	}
	Polygon mover = getMover(polys, twister, region);

	// Some variables for our moving polygon.
	const units::Velocity MAX_SPEED = 0.3f;
	const units::Acceleration ACCELERATION = 0.0025f;
	const units::Acceleration DECELERATION = 0.004f;
	units::Velocity2D velocity;
	units::Acceleration2D acceleration;
	//------------------------------------------
	
	previousTime = SDL_GetTicks();
	// start game loop
	while (true) {
		if (!input.refresh())
			break; // Window was closed.
		if (input.wasKeyPressed( Input::ESC ) )
			break;

		// Horizontal movement.
		if (input.isKeyHeld( Input::LEFT ) && input.isKeyHeld( Input::RIGHT )) {
			acceleration.x = 0.0f;
		} else if (input.isKeyHeld( Input::LEFT )) {
			acceleration.x = -ACCELERATION;
		} else if (input.isKeyHeld( Input::RIGHT )) {
			acceleration.x = ACCELERATION;
		} else {
			acceleration.x = 0.0f;
		}
		// Vertical movement.
		if (input.isKeyHeld( Input::UP ) && input.isKeyHeld(Input::DOWN)) {
			acceleration.y = 0.0f;
		} else if (input.isKeyHeld( Input::UP )) {
			acceleration.y = -ACCELERATION;
		} else if (input.isKeyHeld( Input::DOWN )) {
			acceleration.y = ACCELERATION;
		} else {
			acceleration.y = 0.0f;
		}
		if (input.wasKeyPressed( Input::R ) ) {
			for (std::size_t i = 0; i < numPolys; ++i) {
				polys[i] = Polygon::generate(twister, region);
			}
			mover = getMover(polys, twister, region);
		}
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - previousTime;
		previousTime = currentTime;

		// Update mover here.
		// Control acceleration.
		velocity += acceleration * elapsedTime;
		velocity.x = util::clamp(velocity.x, -MAX_SPEED, MAX_SPEED);
		velocity.y = util::clamp(velocity.y, -MAX_SPEED, MAX_SPEED);
		// Control deceleration.
		if (acceleration.x == 0 && velocity.x != 0) {
			bool isPos(velocity.x > 0);
			velocity.x += (isPos ? -1.0f : 1.0f) * DECELERATION * elapsedTime;
			velocity.x = isPos ? (velocity.x < 0 ? 0.0f : velocity.x) : (velocity.x > 0 ? 0.0f : velocity.x);
		}
		if (acceleration.y == 0 && velocity.y != 0) {
			bool isPos(velocity.y > 0);
			velocity.y += (isPos ? -1.0f : 1.0f) * DECELERATION * elapsedTime;
			velocity.y = isPos ? (velocity.y < 0 ? 0.0f : velocity.y) : (velocity.y > 0 ? 0.0f : velocity.y);
		}

		graphics.clear(); ////////////////////// here so we can debug things in "move"
		// Get delta.

		const units::Coordinate2D delta (velocity * elapsedTime);
		move(mover, polys, delta);

		for (std::size_t i = 0; i < polys.size(); ++i) {
			polys[i].draw(graphics, isect::intersects(mover, polys[i]));
		}

		mover.draw(graphics, true);

		graphics.present();
	}
	close();
	return 0;
}