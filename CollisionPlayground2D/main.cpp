#include <SDL.h>
#include <iostream>
#include <sstream>
#include <random>
#include <vector>

#include "Input.h"
#include "Graphics.h"
#include "Constants.h"
#include "Units.h"
#include "Util.h"
#include "Mover.h"
#include "Geometry/Rectangle.h"
#include "Geometry/Polygon.h"
#include "Geometry/Circle.h"
#include "Geometry/IntersectionMath.h"
#include "Geometry/CollisionMap.h"
#include "Geometry/Wall.h"

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
	virtual const std::vector<Collidable*> getColliding(const Collidable& collider, const units::Coordinate2D& delta) const {
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
std::vector<units::FPS> fpsCounter;
void showFPS(Graphics& graphics, const units::MS elapsedTime) {
	if (elapsedTime <= 0)
		return;
	if (fpsCounter.size() >= fpsSmoothing)
		fpsCounter.erase(fpsCounter.begin());
	const units::FPS fps = util::millisToFPS(elapsedTime);
	fpsCounter.push_back(fps);
	units::FPS ave(0);
	for (std::size_t i = 0; i < fpsCounter.size(); ++i) ave += fpsCounter[i];
	std::ostringstream stream("FPS: ");
	stream << ave/fpsCounter.size() << " - Collision Playground 2D";
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
		const units::Coordinate2D prev = p[i == 0 ? size - 1 : i - 1];
		const units::Coordinate2D curr = p[i];
		units::Coordinate2D edgeNormal(units::Coordinate2D(prev.y - curr.y, curr.x - prev.x).normalize());
		units::Coordinate2D start((prev + curr) *0.5f);
		units::Coordinate2D end(start + edgeNormal*50.0f);
		graphics.renderLine(util::coord2DToSDLPoint(start), util::coord2DToSDLPoint(end), 2);
	}
}

// -------------------------------------------------------------------------------------

Mover genMover(std::vector<Polygon> polys, std::mt19937& twister, const Rectangle& region) {
	Polygon collider = Polygon::generate(twister, Rectangle());
	std::uniform_real_distribution<units::Coordinate> distX(region.left(), region.right());
	std::uniform_real_distribution<units::Coordinate> distY(region.top(), region.bottom());
	units::Coordinate2D position(distX(twister), distY(twister));
	// Ensure that the moving polygon doesn't start inside another polygon (do collision tests until it is put down cleanly).
	while(true) {
		bool isOccupied = false;
		for (std::size_t i = 0; i < polys.size(); ++i) {
			if (isect::intersects(collider, position, polys[i], units::Coordinate2D(0,0))) {
				isOccupied = true;
				collider = Polygon::generate(twister, Rectangle());
				position = units::Coordinate2D(distX(twister), distY(twister));
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

int main (int argc, char* args[]) {
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

	Mover mover(genMover(polys, twister, region));
	SimpleCollisionMap objs(polys);

	previousTime = SDL_GetTicks();
	// Start the game loop.
	while (true) {
		if (!input.refresh())
			break; // Window was closed.
		if (input.wasKeyPressed( Input::ESC ) )
			break;

		// Horizontal movement.
		if (input.isKeyHeld( Input::LEFT ) && input.isKeyHeld( Input::RIGHT )) {
			mover.stopMovingHorizontal();
		} else if (input.isKeyHeld( Input::LEFT )) {
			mover.moveLeft();
		} else if (input.isKeyHeld( Input::RIGHT )) {
			mover.moveRight();
		} else {
			mover.stopMovingHorizontal();
		}
		// Vertical movement.
		if (input.isKeyHeld( Input::UP ) && input.isKeyHeld( Input::DOWN )) {
			mover.stopMovingVertical();
		} else if (input.isKeyHeld( Input::UP )) {
			mover.moveUp();
		} else if (input.isKeyHeld( Input::DOWN )) {
			mover.moveDown();
		} else {
			mover.stopMovingVertical();
		}
		if (input.wasKeyPressed( Input::R ) ) {
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
			isect::intersects(collider, polys[i]) ? graphics.setRenderColour(255, 0, 0) : graphics.setRenderColour(0, 100, 255);
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