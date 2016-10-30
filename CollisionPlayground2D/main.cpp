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
#include "Geometry/IntersectionMath.h"

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

	Rectangle r(500.0f, 300.0f, 100.0f, 100.0f);
	Polygon p = r.toPoly();
	
	std::size_t numPolys(20);
	std::vector<Polygon> polys;
	polys.reserve(numPolys);
	for (std::size_t i = 0; i < numPolys; ++i) {
		polys.push_back(genPoly(twister));
	}
	units::Coordinate2D extendVec(0,10);//(distDelta(twister), distDelta(twister));
	units::Coordinate2D drawStart(400, 300);

	// start game loop
	while (true) {
		if (!input.refresh())
			break; // Window was closed.
		if (input.wasKeyPressed( Input::ESC ) )
			break;
		if (input.wasKeyPressed( Input::E) ) {
			for (std::size_t i = 0; i < polys.size(); ++i) {
				polys[i] = polys[i].clipExtend(extendVec);
			}
			p = p.extend(extendVec);
			std::cout << p.size() << "\n";
		}
		if (input.wasKeyPressed( Input::UP ) ) {
			extendVec = units::Coordinate2D(distDelta(twister), distDelta(twister));
		}
		if (input.wasKeyPressed( Input::R) ) {
			r = Rectangle(distX(twister), distY(twister), distSize(twister),distSize(twister));
			p = r.toPoly();
			for (std::size_t i = 0; i < polys.size(); ++i) {
				polys[i] = genPoly(twister);
			}
			previousTime = SDL_GetTicks();
			continue;
		}
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - previousTime;

		// If there is anything being animated, do it here (say, a traversal).

		previousTime = currentTime;

		graphics.clear();

		graphics.setRenderColour(0,255,255);
		graphics.renderLine(util::coord2DToSDLPoint(drawStart), util::coord2DToSDLPoint(drawStart + extendVec * 5), 2);

		std::vector<bool> polyCollisions(polys.size());
		bool rectCollision = false;
		for (std::size_t i = 0; i < polys.size(); ++i) {
			if (isect::intersects(r, polys[i])) {
				rectCollision = true;
				polyCollisions[i] = true;
			}
			for (std::size_t k = i+1; k < polys.size(); ++k) {
				if (isect::intersects(polys[i], polys[k])) {
					polyCollisions[i] = true;
					polyCollisions[k] = true;
				}
			}
			polys[i].draw(graphics, polyCollisions[i]);
		}
		r.draw(graphics, rectCollision);
		//p.draw(graphics, false);

		graphics.present();
	}
	close();
	return 0;
}