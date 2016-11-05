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

Polygon genPoly(std::mt19937& rando) {
	std::uniform_int_distribution<units::Pixel> distX(util::tileToPixel(10), util::tileToPixel(room::width-10));
	std::uniform_int_distribution<units::Pixel> distY(util::tileToPixel(10), util::tileToPixel(room::height-10));
	const units::Coordinate2D cen(distX(rando), distY(rando));
	
	std::uniform_int_distribution<std::size_t> distVerts(3, 20);
	const std::size_t numVerts(distVerts(rando));

	// Generate random numbers between 0 and 2pi to make points around a circle.
	std::uniform_real_distribution<units::Coordinate> distPI(0.0f, 2.0f*3.14159265358979323f);
	std::vector<units::Coordinate> piVec;
	piVec.reserve(numVerts);

	for (std::size_t i = 0; i < numVerts; ++i) {
		piVec.push_back(distPI(rando));
	}

	// Sort descending (so we have counterclockwise winding).
	std::sort(piVec.begin(), piVec.end(), [](const units::Coordinate& lhs, const units::Coordinate& rhs) {
		return lhs > rhs;
	});

	std::normal_distribution<units::Coordinate> distRad(20.0f, 70.0f);
	units::Coordinate radius(distRad(rando));

	std::vector<units::Coordinate2D> vertices;
	vertices.reserve(numVerts);
	for (std::size_t i = 0; i < numVerts; ++i) {
		vertices.push_back(cen + units::Coordinate2D(radius * std::cosf(piVec[i]), radius * sinf(piVec[i])));
	}

	return Polygon(vertices);
}

Polygon getMover(std::vector<Polygon> polys, std::mt19937& twister) {
	Polygon mover = genPoly(twister);;
	// Ensure that the moving polygon doesn't start inside another polygon (do collision tests until it is put down cleanly).
	while (true) {
		bool isOccupied = false;
		for (std::size_t i = 0; i < polys.size(); ++i) {
			if (isect::intersects(mover, polys[i])) {
				isOccupied = true; // Mover is in an occupied spot.
				mover = genPoly(twister); // Try another spot.
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
	std::uniform_int_distribution<units::Pixel> distX(util::tileToPixel(10), util::tileToPixel(room::width-10));
	std::uniform_int_distribution<units::Pixel> distY(util::tileToPixel(10), util::tileToPixel(room::height-10));
	std::uniform_real_distribution<units::Coordinate> distSize(util::tileToPixel(2), util::tileToPixel(4));
	std::uniform_int_distribution<units::Pixel> distDelta(-util::tileToPixel(1), util::tileToPixel(1));
	std::uniform_real_distribution<units::Coordinate> normVec(-1.0f, 1.0f);
	
	previousTime = SDL_GetTicks();
		
	std::size_t numPolys(20);
	std::vector<Polygon> polys;
	polys.reserve(numPolys);
	for (std::size_t i = 0; i < numPolys; ++i) {
		polys.push_back(genPoly(twister));
	}
	Polygon mover = getMover(polys, twister);

	// Some variables for our moving polygon.
	const units::Velocity MAX_SPEED = 0.3f;
	const units::Acceleration ACCELERATION = 0.0025f;
	const units::Acceleration DECELERATION = 0.004f;
	units::Velocity2D velocity;
	units::Acceleration2D acceleration;
	//------------------------------------------
	

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
				polys[i] = genPoly(twister);
			}
			mover = getMover(polys, twister);
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
		// Get delta.
		const units::Coordinate2D delta (velocity * elapsedTime);
		if (!delta.isZero()) {
			// Check collisions here.
			bool wasCollision = false;
			units::Coordinate depth = 0;
			units::Coordinate2D normal;
			// Find the closest collision.
			for (std::size_t i = 0; i < polys.size(); ++i) {
				units::Coordinate testDepth;
				units::Coordinate2D testNorm;
				if (collision_math::collides(mover, delta, polys[i], testNorm, testDepth)) {
					if (!wasCollision) { // First time getting a collision.
						depth = testDepth;
						normal = testNorm;
					} else if (depth < testDepth) { // We got a collision before this. Take the closer one (has deeper depth, because it collided sooner).
						depth = testDepth;
						normal = testNorm;
					}
					wasCollision = true;
				}
			}
			units::Coordinate deltaMag = delta.magnitude();
			units::Coordinate2D deltaNorm = delta/deltaMag;
			mover = mover.translate(deltaNorm * (deltaMag - depth));
		}
		graphics.clear();

		for (std::size_t i = 0; i < polys.size(); ++i) {
			polys[i].draw(graphics, isect::intersects(mover, polys[i]));
		}

		mover.draw(graphics, true);

		graphics.present();
	}
	close();
	return 0;
}