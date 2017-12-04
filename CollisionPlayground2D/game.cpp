#include <SDL.h>
#include <iostream>
#include <sstream>
#include <random>
#include <vector>

#include "Units.h"
#include "Constants.h"
#include "Util.h"
#include "Input.h"
#include "Graphics.h"
#include "Mover.h"

#include "Geometry/Geometry.h"

using namespace geom;

namespace game {

	// Extremely simple CollisionMap implementation: no data structure speedup at all.
	class SimpleCollisionMap : public CollisionMap {
	private:
		std::vector<Wall*> obstacles_;
	public:
		SimpleCollisionMap(std::vector<Polygon> polys) {
			add(polys);
		}
		~SimpleCollisionMap() {
			clear();
		}
		virtual const std::vector<Collidable*> getColliding(const Collidable& collider, const geom::Coord2& delta) const {
			return std::vector<Collidable*>(obstacles_.begin(), obstacles_.end());
		}
		void add(std::vector<Polygon> polys) {
			for (std::size_t i = 0; i < polys.size(); ++i) {
				Wall* w = new Wall(ShapeContainer(new Polygon(polys[i])));
				obstacles_.push_back(w);
			}
		}
		void clear() {
			for (std::size_t i = 0; i < obstacles_.size(); ++i)
				delete obstacles_[i];
			obstacles_.clear();
		}
	};

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
		// Draw the lines of the polygon.
		for (std::size_t i = 0; i < size - 1; ++i) {
			graphics.renderLine(util::coord2DToSDLPoint(p[i]), util::coord2DToSDLPoint(p[i + 1]), 1);
		}
		// Draw last line.
		graphics.renderLine(util::coord2DToSDLPoint(p[size - 1]), util::coord2DToSDLPoint(p[0]), 1);
		// Render the vertices over the lines.
		graphics.setRenderColour(255, 255, 0);
		for (std::size_t i = 0; i < size; ++i) {
			graphics.renderCircle(util::coord2DToSDLPoint(p[i]), 1);
		}
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

	Mover genMover(std::vector<Polygon> polys, std::mt19937& twister, const Rect& region) {
		Polygon collider = Polygon::generate(twister, Rect());
		std::uniform_real_distribution<geom::gFloat> distX(region.left(), region.right());
		std::uniform_real_distribution<geom::gFloat> distY(region.top(), region.bottom());
		geom::Coord2 position(distX(twister), distY(twister));
		// Ensure that the moving polygon doesn't start inside another polygon (do collision tests until it is put down cleanly).
		while (true) {
			bool isOccupied = false;
			for (std::size_t i = 0; i < polys.size(); ++i) {
				if (geom::intersects(collider, position, polys[i], geom::Coord2(0,0))) {
					isOccupied = true;
					collider = Polygon::generate(twister, Rect());
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
		Rect region(util::tileToCoord(10), util::tileToCoord(5), game::SCREEN_WIDTH - util::tileToCoord(20), game::SCREEN_HEIGHT - util::tileToCoord(10));

		std::size_t numPolys(20);
		std::vector<Polygon> polys;
		polys.reserve(numPolys);
		for (std::size_t i = 0; i < numPolys; ++i) {
			polys.push_back(Polygon::generate(twister, region));
		}

		Mover mover(genMover(polys, twister, region));
		SimpleCollisionMap objs(polys);

		previousTime = SDL_GetTicks();
		// Start the game loop.
		while (true) {
			if (!input.refresh())
				break; // Window was closed.
			if (input.wasKeyPressed(Input::ESC))
				break;

			// Horizontal movement.
			if (input.isKeyHeld(Input::LEFT) && input.isKeyHeld(Input::RIGHT)) {
				mover.stopMovingHorizontal();
			} else if (input.isKeyHeld(Input::LEFT)) {
				mover.moveLeft();
			} else if (input.isKeyHeld(Input::RIGHT)) {
				mover.moveRight();
			} else {
				mover.stopMovingHorizontal();
			}
			// Vertical movement.
			if (input.isKeyHeld(Input::UP) && input.isKeyHeld(Input::DOWN)) {
				mover.stopMovingVertical();
			} else if (input.isKeyHeld(Input::UP)) {
				mover.moveUp();
			} else if (input.isKeyHeld(Input::DOWN)) {
				mover.moveDown();
			} else {
				mover.stopMovingVertical();
			}
			if (input.wasKeyPressed(Input::R)) {
				objs.clear();
				for (std::size_t i = 0; i < numPolys; ++i)
					polys[i] = Polygon::generate(twister, region);
				mover = genMover(polys, twister, region);
				objs.add(polys);
			}
			currentTime = SDL_GetTicks();
			elapsedTime = currentTime - previousTime;
			previousTime = currentTime;

			mover.update(elapsedTime, &objs);

			graphics.clear();
			Polygon collider(mover.getCollider().poly()); // We know it's a polygon, because we made it above.
			collider.translate(mover.getPosition());
			for (std::size_t i = 0; i < polys.size(); ++i) {
	#ifdef DEBUG
				geom::intersects(collider, polys[i]) ? graphics.setRenderColour(255, 0, 0) : graphics.setRenderColour(0, 100, 255);
				drawPoly(polys[i], graphics);
	#else
				graphics.setRenderColour(0, 100, 255);
				drawPoly(polys[i], graphics);
	#endif
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