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

	//Rectangle r(500.0f, 300.0f, 100.0f, 100.0f);
	
	// Approximate a circle with line segments.
	const Uint8 numSegs = 15;
	const units::Coordinate radius = 50;
	const units::Coordinate2D center(500,300);
	std::vector<units::Coordinate2D> vertices(numSegs);
	for (Uint8 i = 0; i < numSegs; ++i) {
		const units::Coordinate theta(2.0f*3.14159265358979323f*static_cast<float>(i) / static_cast<float>(numSegs));
		const units::Coordinate x = radius * cosf(theta);
		const units::Coordinate y = radius * sinf(theta);
		vertices[numSegs - i - 1] = (units::Coordinate2D(center.x + x, center.y + y));
	}


	Polygon p(vertices);

	units::Coordinate2D extendVec(distDelta(twister), distDelta(twister));
	//units::Coordinate2D extendVec(50, 0);

	// start game loop
	while (true) {
		if (!input.refresh())
			break; // Window was closed.
		if (input.wasKeyPressed( Input::ESC ) )
			break;
		if (input.wasKeyPressed( Input::E) ) {
			p = p.extend(extendVec);
			continue;
		}
		if (input.wasKeyPressed( Input::R) ) {
			extendVec = units::Coordinate2D(distDelta(twister), distDelta(twister));
			//p = Polygon(vertices);
			previousTime = SDL_GetTicks();
			continue;
		}
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - previousTime;

		// If there is anything being animated, do it here (say, a traversal).

		previousTime = currentTime;

		graphics.clear();

		graphics.setRenderColour(50,255,255);
		// Draw where the initial extend goes to.
		for (std::size_t i = 0; i < vertices.size(); ++i) {
			graphics.renderLine(util::coord2DToSDLPoint(vertices[i]), util::coord2DToSDLPoint(vertices[i] + extendVec));
		}

		p.draw(graphics, false);

		graphics.present();
	}
	close();
	return 0;
}