#pragma once
#ifndef INCLUDE_ROOM_HPP
#define INCLUDE_ROOM_HPP

#include <SDL.h>
#include <vector>

#include "Geometry2D/Geometry.hpp"

namespace geom { class Shape; }
class Graphics;

class Room {
public:
	static const Uint8 HEIGHT;
	static const Uint8 WIDTH;

	Room();
	~Room();

	// Find all tiles in the map that collide with the given collider traveling from origin by delta.
	// Returns a vector of CollisionTiles in the order that the collider traverses them.
	void markCollidingTiles(const geom::Shape& collider, const geom::Coord2& origin, const geom::Coord2& delta);
	// Reset the tiles to be normal colour.
	void reset();

	// Render the viewable portion of the level to the screen.
	void draw(Graphics& graphics);
private:
	std::vector< std::vector<geom::Rect> > background_tiles_;
	std::vector< std::vector<bool> > is_marked_;
};

#endif // INCLUDE_ROOM_HPP
