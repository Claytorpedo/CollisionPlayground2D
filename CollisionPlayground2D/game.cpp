#include <SDL.h>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <vector>

#include "units.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "Input.hpp"
#include "Graphics.hpp"
#include "Mover.hpp"

#include "Geometry/Geometry.hpp"

using namespace geom;

namespace game {
	const Rect        POLY_REGION = Rect(util::tileToCoord(10), util::tileToCoord(5), game::SCREEN_WIDTH - util::tileToCoord(20), game::SCREEN_HEIGHT - util::tileToCoord(10));
	const gFloat      POLY_MIN = 0.1f;
	const gFloat      POLY_MAX = 100.0f;
	const std::size_t POLY_MIN_VERTS = 3;
	const std::size_t POLY_MAX_VERTS = 20;

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
		for (std::size_t k = size -1, i = 0; i < size; k = i++)
			graphics.renderLine(util::coord2DToSDLPoint(p[k]), util::coord2DToSDLPoint(p[i]), 1);
	}
	void drawPolyVerts(const Polygon& p, Graphics& graphics) {
		const size_t size = p.size();
		graphics.setRenderColour(255, 255, 0);
		for (std::size_t i = 0; i < p.size(); ++i)
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
	// Random generation.

	// Randomly generate a polygon.
	// region is a bounding box defining the region to place the polygon's center in (part of the polygon can be outside this region).
	// minRad and maxRad control how large the generated polygon will be.
	// minVerts and maxVerts control how many vertices the generated polygon can have.
	Polygon genPoly(std::mt19937& rando, const Rect& region, const gFloat minRad, const gFloat maxRad, const std::size_t minVerts, const std::size_t maxVerts) {
		// Randomly generate the polygon's center within the region.
		std::uniform_real_distribution<gFloat> distX(region.left(), region.right());
		std::uniform_real_distribution<gFloat> distY(region.top(), region.bottom());
		const Coord2 cen(distX(rando), distY(rando));

		if (minVerts < 3 || maxVerts < 3) std::cerr << "Error: Cannot generate a polygon with fewer than 3 vertices. Defaulting to 3 minimum.\n";
		const std::size_t min = minVerts < 3 ? 3 : minVerts;
		std::uniform_int_distribution<std::size_t> distVerts(min, maxVerts < min ? min : maxVerts);
		const std::size_t numVerts(distVerts(rando));

		// Generate random numbers between 0 and tau (2pi) to make points around a circle.
		std::uniform_real_distribution<gFloat> distPI(0.0f, constants::TAU);
		std::vector<gFloat> piVec;
		piVec.reserve(numVerts);
		for (std::size_t i = 0; i < numVerts; ++i)
			piVec.push_back(distPI(rando));
		// Sort descending (so we have counterclockwise winding).
		std::sort(piVec.begin(), piVec.end(), [](const gFloat& lhs, const gFloat& rhs) {
			return lhs > rhs;
		});

		// Get radius for polygon.
		std::uniform_real_distribution<gFloat> distRad(minRad, maxRad);
		gFloat radius(distRad(rando));

		std::vector<Coord2> vertices;
		vertices.reserve(numVerts);
		for (std::size_t i = 0; i < numVerts; ++i)
			vertices.push_back(cen + Coord2(radius * std::cos(piVec[i]), radius * std::sin(piVec[i])));
		return Polygon(vertices);
	}

	Mover genMover(std::vector<Polygon> polys, std::mt19937& twister, const Rect& region) {
		Polygon collider(genPoly(twister, Rect(), POLY_MIN, POLY_MAX, POLY_MIN_VERTS, POLY_MAX_VERTS));
		std::uniform_real_distribution<geom::gFloat> distX(region.left(), region.right());
		std::uniform_real_distribution<geom::gFloat> distY(region.top(), region.bottom());
		geom::Coord2 position(distX(twister), distY(twister));
		// Ensure that the moving polygon doesn't start inside another polygon (do collision tests until it is put down cleanly).
		while (true) {
			bool isOccupied = false;
			for (std::size_t i = 0; i < polys.size(); ++i) {
				if (geom::overlaps(collider, position, polys[i], geom::Coord2(0,0))) {
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

		std::size_t numPolys(20);
		std::vector<Polygon> polys;
		polys.reserve(numPolys);
		for (std::size_t i = 0; i < numPolys; ++i) {
			polys.push_back(genPoly(twister, POLY_REGION, POLY_MIN, POLY_MAX, POLY_MIN_VERTS, POLY_MAX_VERTS));
		}

		Mover mover(genMover(polys, twister, POLY_REGION));
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
					polys[i] = genPoly(twister, POLY_REGION, POLY_MIN, POLY_MAX, POLY_MIN_VERTS, POLY_MAX_VERTS);
				mover = genMover(polys, twister, POLY_REGION);
				objs.add(polys);
			}
			currentTime = SDL_GetTicks();
			elapsedTime = currentTime - previousTime;
			previousTime = currentTime;

			mover.update(elapsedTime, &objs);

			graphics.clear();
			Polygon collider(mover.getCollider().shape().toPoly());
			collider.translate(mover.getPosition());
			for (std::size_t i = 0; i < polys.size(); ++i) {
	#ifdef DEBUG
				geom::overlaps(collider, polys[i]) ? graphics.setRenderColour(255, 0, 0) : graphics.setRenderColour(0, 100, 255);
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