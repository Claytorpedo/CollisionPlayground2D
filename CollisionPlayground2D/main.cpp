#include <SDL.h>
#include <iostream>
#include <random>

#include "Room.h"
#include "Input.h"
#include "Graphics.h"
#include "Constants.h"
#include "Units.h"
#include "Geometry/Rectangle.h"

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

	Rectangle rect(0, 0, distSize(twister), distSize(twister));
	Coordinate2D position(distX(twister), distY(twister));
	Coordinate2D delta(distDelta(twister), distDelta(twister));


	previousTime = SDL_GetTicks();

	room.markCollidingTiles(rect, position, delta);

	graphics.clear();
	room.draw(graphics);
	Rectangle r(position, rect.w, rect.h);
	r.setColour(0,255,255,255);
	r.draw(graphics, false, 3);
	// Top left to delta
	SDL_Point o = { static_cast<int>(position.x), static_cast<int>(position.y) };
	SDL_Point e = { static_cast<int>(position.x + delta.x), static_cast<int>(position.y + delta.y) };
	graphics.renderLine(o, e, 255, 255, 0, 255, 1);
	// Top right to delta.
	o.x += static_cast<int>(r.w);
	e.x += static_cast<int>(r.w);
	graphics.renderLine(o, e, 255, 255, 0, 255, 1);
	// Bottom right to delta
	o.y += static_cast<int>(r.h);
	e.y += static_cast<int>(r.h);
	graphics.renderLine(o, e, 255, 255, 0, 255, 1);
	// Bottom left to delta
	o.x -= static_cast<int>(r.w);
	e.x -= static_cast<int>(r.w);
	graphics.renderLine(o, e, 255, 255, 0, 255, 1);

	graphics.present();

	// start game loop
	while (true) {
		if (!input.refresh())
			break; // Window was closed.
		if (input.wasKeyPressed( Input::ESC ) )
			break;
		if (input.wasKeyPressed( Input::R) ) {
			rect.w = distSize(twister);
			rect.h = distSize(twister);
			position = Coordinate2D(distX(twister), distY(twister));
			delta = Coordinate2D(distDelta(twister), distDelta(twister));
			room.reset();

			room.markCollidingTiles(rect, position, delta);

			graphics.clear();
			room.draw(graphics);
			Rectangle r(position, rect.w, rect.h);
			r.setColour(0,255,255,255);
			r.draw(graphics, false, 3);
			// Top left to delta
			SDL_Point o = { static_cast<int>(position.x), static_cast<int>(position.y) };
			SDL_Point e = { static_cast<int>(position.x + delta.x), static_cast<int>(position.y + delta.y) };
			graphics.renderLine(o, e, 255, 255, 0, 255, 1);
			// Top right to delta.
			o.x += static_cast<int>(r.w);
			e.x += static_cast<int>(r.w);
			graphics.renderLine(o, e, 255, 255, 0, 255, 1);
			// Bottom right to delta
			o.y += static_cast<int>(r.h);
			e.y += static_cast<int>(r.h);
			graphics.renderLine(o, e, 255, 255, 0, 255, 1);
			// Bottom left to delta
			o.x -= static_cast<int>(r.w);
			e.x -= static_cast<int>(r.w);
			graphics.renderLine(o, e, 255, 255, 0, 255, 1);

			graphics.present();

			previousTime = SDL_GetTicks();
			continue;
		}
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - previousTime;

		// If there is anything being animated, do it here (say, a traversal).

		previousTime = currentTime;

		units::MS frameDur = SDL_GetTicks() - currentTime;
		if (frameDur < 16)
			SDL_Delay(16 - frameDur);
	}
	close();
	return 0;
}