#include <SDL.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>
#include <vector>

#include "units.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "Input.hpp"
#include "Graphics.hpp"
#include "Mover.hpp"
#include "gen.hpp"

#include "geom_examples/SimpleCollisionMap.hpp"

#include "Geometry2D/Geometry.hpp"

using namespace geom;

namespace game {
	const Rect        POLY_REGION = Rect(util::tileToCoord(10), util::tileToCoord(5), game::SCREEN_WIDTH - util::tileToCoord(20), game::SCREEN_HEIGHT - util::tileToCoord(10));
	const gFloat      POLY_MIN = 0.1f;
	const gFloat      POLY_MAX = 100.0f;
	const std::size_t POLY_MIN_VERTS = 3;
	const std::size_t POLY_MAX_VERTS = 20;

	void close() {
		SDL_Quit();
	}
	void closeWithError() {
		std::cout << "Press Enter to close." << std::endl;
		std::cin.ignore();
		close();
	}

	const int fpsSmoothing = 20; // Get an average over several frames.
	std::vector<game::FPS> fpsCounter;
	void showFPS(Graphics& graphics, const game::MS elapsedTime) {
		if (elapsedTime <= 0)
			return;
		if (fpsCounter.size() >= fpsSmoothing)
			fpsCounter.erase(fpsCounter.begin());
		const game::FPS fps = util::millisToFPS(elapsedTime);
		fpsCounter.push_back(fps);
		game::FPS ave(0);
		for (std::size_t i = 0; i < fpsCounter.size(); ++i) ave += fpsCounter[i];
		std::ostringstream stream("FPS: ");
		stream << ave / fpsCounter.size() << " - Collision Playground 2D";
		graphics.setWindowTitle(stream.str());
	}

	// ----------------------------- Drawing polygons -------------------------------------

	void drawPoly(const Polygon& p, Graphics& graphics) {
		const size_t size = p.size();
		for (std::size_t k = size -1, i = 0; i < size; k = i++)
			graphics.renderLine(util::coord2DToSDLPoint(p[k]), util::coord2DToSDLPoint(p[i]), 1);
	}
	void drawPolyVerts(const Polygon& p, Graphics& graphics) {
		const size_t size = p.size();
		graphics.setRenderColour(255, 255, 0);
		for (std::size_t i = 0; i < size; ++i)
			graphics.renderCircle(util::coord2DToSDLPoint(p[i]), 1);
	}
	void drawPolyEdgeNormals(const Polygon& p, Graphics& graphics) {
		graphics.setRenderColour(0, 0, 255);
		const size_t size = p.size();
		for (std::size_t i = 0; i < size; ++i) {
			const geom::Coord2 prev = p[i == 0 ? size - 1 : i - 1];
			const geom::Coord2 curr = p[i];
			geom::Coord2 edgeNormal(geom::Coord2(prev.y - curr.y, curr.x - prev.x).normalize());
			geom::Coord2 start((prev + curr) *0.5f);
			geom::Coord2 end(start + edgeNormal*50.0f);
			graphics.renderLine(util::coord2DToSDLPoint(start), util::coord2DToSDLPoint(end), 2);
		}
	}

	// -------------------------------------------------------------------------------------
	Mover genMover(const SimpleCollisionMap& map, std::mt19937& twister, const Rect& region) {
		Polygon collider(genPoly(twister, Rect(), POLY_MIN, POLY_MAX, POLY_MIN_VERTS, POLY_MAX_VERTS));
		std::uniform_real_distribution<geom::gFloat> distX(region.left(), region.right());
		std::uniform_real_distribution<geom::gFloat> distY(region.top(), region.bottom());
		geom::Coord2 position(distX(twister), distY(twister));
		// Ensure that the moving polygon doesn't start inside another polygon (do collision tests until it is put down cleanly).
		while (true) {
			bool isOccupied = false;
			for (std::size_t i = 0; i < map.size(); ++i) {
				if (geom::overlaps(collider, position, map[i]->getCollider(), geom::Coord2(0,0))) {
					isOccupied = true;
					collider = genPoly(twister, Rect(), POLY_MIN, POLY_MAX, POLY_MIN_VERTS, POLY_MAX_VERTS);
					position = geom::Coord2(distX(twister), distY(twister));
					std::cout << "Spot occupied. Trying somewhere else...\n";
					break;
				}
			}
			if (!isOccupied)
				break;
		}
		std::cout << "Mover has entered the level.\n";
		return Mover(ShapeContainer(collider), position);
	}

	// --------------------------------------------------------------------------------------

	int run(int argc, char* args[]) {
		Input input;
		Graphics graphics;
		game::MS currentTime, previousTime, elapsedTime;

		if (!graphics.init(game::SCREEN_WIDTH, game::SCREEN_HEIGHT)) {
			std::cerr << "Error: Failed to initialize graphics.\n";
			closeWithError();
			return -1;
		}
		std::random_device rd;
		std::mt19937 twister(rd());

		std::size_t numShapes(20);
		SimpleCollisionMap map;
		for (std::size_t i = 0; i < numShapes; ++i)
			map.add(genPoly(twister, POLY_REGION, POLY_MIN, POLY_MAX, POLY_MIN_VERTS, POLY_MAX_VERTS));

		Mover mover(genMover(map, twister, POLY_REGION));

		previousTime = SDL_GetTicks();
		// Start the game loop.
		while (true) {
			if (!input.refresh())
				break; // Window was closed.
			if (input.wasKeyPressed(SDLK_ESCAPE))
				break;

			// Horizontal movement.
			if ((input.isKeyHeld(SDLK_LEFT) || input.isKeyHeld(SDLK_a)) && (input.isKeyHeld(SDLK_RIGHT) || input.isKeyHeld(SDLK_d))) {
				mover.stopMovingHorizontal();
			} else if (input.isKeyHeld(SDLK_LEFT) || input.isKeyHeld(SDLK_a)) {
				mover.moveLeft();
			} else if (input.isKeyHeld(SDLK_RIGHT) || input.isKeyHeld(SDLK_d)) {
				mover.moveRight();
			} else {
				mover.stopMovingHorizontal();
			}
			// Vertical movement.
			if ((input.isKeyHeld(SDLK_UP) || input.isKeyHeld(SDLK_w)) && (input.isKeyHeld(SDLK_DOWN) || input.isKeyHeld(SDLK_s))) {
				mover.stopMovingVertical();
			} else if (input.isKeyHeld(SDLK_UP) || input.isKeyHeld(SDLK_w)) {
				mover.moveUp();
			} else if (input.isKeyHeld(SDLK_DOWN) || input.isKeyHeld(SDLK_s)) {
				mover.moveDown();
			} else {
				mover.stopMovingVertical();
			}
			if (input.wasKeyPressed(SDLK_r)) {
				map.clear();
				for (std::size_t i = 0; i < numShapes; ++i)
					map.add(genPoly(twister, POLY_REGION, POLY_MIN, POLY_MAX, POLY_MIN_VERTS, POLY_MAX_VERTS));
				mover = genMover(map, twister, POLY_REGION);
			}
			currentTime = SDL_GetTicks();
			elapsedTime = currentTime - previousTime;
			previousTime = currentTime;

			mover.update(elapsedTime, map);

			graphics.clear();
			Polygon collider(mover.getCollider().shape().toPoly());
			collider.translate(mover.getPosition());
			for (std::size_t i = 0; i < map.size(); ++i) {
	#ifdef DEBUG
				geom::overlaps(collider, map[i]->getCollider()) ? graphics.setRenderColour(255, 0, 0) : graphics.setRenderColour(0, 100, 255);
	#else
				graphics.setRenderColour(0, 100, 255);
	#endif
				drawPoly(map[i]->getCollider().shape().toPoly(), graphics);
			}

			graphics.setRenderColour(255, 0, 0);
			drawPoly(collider, graphics);
			graphics.present();
			showFPS(graphics, elapsedTime);
		}
		close();
		return 0;
	}
}