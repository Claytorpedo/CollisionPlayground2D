#include <SDL.h>
#include <iostream>
#include <random>
#include <vector>

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
	const units::Coordinate f(constants::TAU / static_cast<units::Coordinate>(numSegs));
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

// Returns true if there was a collision, false if none was found.
bool findClosestCollision(const Polygon& mover, const std::vector<Polygon>& polys, const units::Coordinate2D& dir, const units::Coordinate dist,
	units::Coordinate &out_dist, units::Coordinate2D& out_edge) {
	if (dir.isZero() || dist == 0.0f)
		return false;

	bool wasCollision = false;

	units::Coordinate   moveDist = dist;
	units::Coordinate2D edge(0,0);

	for (std::size_t i = 0; i < polys.size(); ++i) {
		units::Coordinate testDist;
		units::Coordinate2D testEdge;
		if (collision_math::collides(mover, dir, dist, polys[i], testDist, testEdge)) {
			if (!wasCollision) { // First time getting a collision.
				moveDist = testDist;
				edge = testEdge;
				wasCollision = true;
			} else if (moveDist > testDist) { // We got a collision before this. Take the closer one.
				moveDist = testDist;
				edge = testEdge;
			}
			if (moveDist == 0.0f)
				break;// Can't move any less than not moving!
		}
	}
	// Set our outputs.
	out_dist = moveDist;
	out_edge = edge;

	return wasCollision;
}

// Move the mover polygon by delta.
void move(Polygon& mover, const std::vector<Polygon>& polys, const units::Coordinate2D& delta) {
	if (delta.isZero())
		return; // Nowhere to move.
	const units::Coordinate   originalDist  = delta.magnitude();
	const units::Coordinate2D originalDir = delta/originalDist; // normalize delta dir.
	units::Coordinate remainingDist = originalDist;
	units::Coordinate2D currentDir = originalDir;
	units::Coordinate moveDist(0);
	units::Coordinate2D deflectEdge(0,0);
	while ( true ) {
		if ( !findClosestCollision(mover, polys, currentDir, remainingDist, moveDist, deflectEdge) ) {
			// No collision. Move the mover and exit.
			mover = mover.translate(currentDir * remainingDist);
			return;
		}
		// We collided with something.
		mover = mover.translate(currentDir*moveDist);
		// See if we have anywhere left to move.
		remainingDist -= moveDist;
		if (remainingDist < constants::EPSILON || deflectEdge.isZero())
			return;
		// Find the projection of the remaining distance along the original direction on the deflection vector.
		// Note that direction of the edge doesn't matter: it is treated like a line we are projecting against.
		const units::Coordinate2D projDir = deflectEdge.normalize();
		// Project using the original delta direction, to avoid "bouncing" off of corners.
		const units::Coordinate dot(projDir.dot(originalDir * remainingDist));
		const units::Coordinate2D projection(dot*projDir.x, dot*projDir.y);
		// Projection is our new delta. Get new direction and remaining distance to move.
		remainingDist = projection.magnitude();
		if (remainingDist < constants::EPSILON)
			return;

		currentDir = projection/remainingDist;
	}
}

int main (int argc, char* args[]) {
	Room room;
	Input input;
	Graphics graphics;
	units::MS currentTime, previousTime, elapsedTime;
	

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
		// Get delta.
		const units::Coordinate2D delta (velocity * elapsedTime);
		const units::Coordinate prevX = mover.left();
		const units::Coordinate prevY = mover.top();
		move(mover, polys, delta);

		graphics.clear();
		for (std::size_t i = 0; i < polys.size(); ++i) {
			polys[i].draw(graphics, isect::intersects(mover, polys[i]));
		}

		mover.draw(graphics, true);

		//SDL_Delay(80 > elapsedTime ? 80 - elapsedTime : 0);

		graphics.present();
	}
	close();
	return 0;
}