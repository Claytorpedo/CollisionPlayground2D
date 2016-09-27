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

void close() {
	SDL_Quit();
}
void closeWithError() {
	std::cout << "Press Enter to close." << std::endl;
	std::cin.ignore();
	close();
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
	std::uniform_int_distribution<units::Pixel> distDelta(-util::tileToPixel(20), util::tileToPixel(20));
	std::uniform_real_distribution<units::Coordinate> normVec(-1.0f, 1.0f);

	Rectangle rect(distX(twister), distY(twister), distSize(twister), distSize(twister));
	
	const Uint8 numLines = 5;
	std::vector<LineSegment> lines;
	lines.reserve(numLines);
	/*for (Uint8 i = 0; i < numLines; ++i) {
		Coordinate2D start(distX(twister), distY(twister));
		Coordinate2D end(distX(twister), distY(twister));
		lines.push_back(LineSegment(start, end));
	}*/
	lines.push_back(LineSegment(500, 500, 400, 500));
	//lines.push_back(LineSegment(500, 500, 400, 500));
	//lines.push_back(LineSegment(500, 500, 600, 500));
	//lines.push_back(LineSegment(500, 500, 500, 600));
	//lines.push_back(LineSegment(100, 100, 400, 400));
	//lines.push_back(LineSegment(100, 101, 400, 401));
	//lines.push_back(LineSegment(100, 101.5, 400, 401.5));
	//lines.push_back(LineSegment(500, 500.01, 400, 500.01));
	//units::Coordinate2D dir(normVec(twister), normVec(twister));
	//dir = dir.normalize();
	//Ray r(distX(twister), distY(twister), dir.x, dir.y);

	Ray r(301.0f, 500.0f, -1, 0);

	previousTime = SDL_GetTicks();

	// start game loop
	while (true) {
		if (!input.refresh())
			break; // Window was closed.
		if (input.wasKeyPressed( Input::ESC ) )
			break;
		if (input.wasKeyPressed( Input::R) ) {
			rect = Rectangle(distX(twister), distY(twister), distSize(twister), distSize(twister));
			/*lines.clear();
			for (Uint8 i = 0; i < numLines; ++i) {
				Coordinate2D start(distX(twister), distY(twister));
				Coordinate2D end(distX(twister), distY(twister));
				lines.push_back(LineSegment(start, end));
			}*/
			units::Coordinate2D dir(normVec(twister), normVec(twister));
			dir = dir.normalize();
			r = Ray(distX(twister), distY(twister), dir.x, dir.y);

			previousTime = SDL_GetTicks();
			continue;
		}
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - previousTime;

		// If there is anything being animated, do it here (say, a traversal).

		previousTime = currentTime;

		graphics.clear();

		// Find all the collisions.
		bool isRectColliding = false;
		bool isRayColliding = false;
		std::vector<bool> isLineColliding(lines.size());
		std::vector<SDL_Point> points;
		for (std::size_t i = 0; i < lines.size(); ++i) {
			if (rect.collides(lines[i])) {
				isRectColliding = true;
				isLineColliding[i] = true;
			}
			units::Coordinate2D p;
			if (r.intersects(lines[i], p)) {
				isLineColliding[i] = true;
				isRayColliding = true;
				points.push_back(util::coord2DToSDLPoint(p));
			}
			for (std::size_t j = i+1; j < lines.size(); ++j) {
				units::Coordinate2D p;
				if (lines[i].intersects(lines[j], p) ) {
					isLineColliding[i] = true;
					isLineColliding[j] = true;
					points.push_back(util::coord2DToSDLPoint(p));
				}
			}
			isLineColliding[i] ? graphics.setRenderColour(255, 0, 0) : graphics.setRenderColour(0,0,255);
			graphics.renderLine(util::coord2DToSDLPoint(lines[i].start), util::coord2DToSDLPoint(lines[i].end));
		}
		rect.draw(graphics, isRectColliding);
		graphics.setRenderColour(255,255,0);
		for (std::size_t p = 0; p < points.size(); ++p) {
			graphics.renderPoint(points[p], 2);
		}

		graphics.setRenderColour(100,255,0);
		graphics.renderCircle(util::coord2DToSDLPoint(r.origin), 3);
		isRayColliding ? graphics.setRenderColour(255,0,0) : graphics.setRenderColour(0,255,255);
		graphics.renderRay(util::coord2DToSDLPoint(r.origin), r.dir);
		graphics.present();
	}
	close();
	return 0;
}