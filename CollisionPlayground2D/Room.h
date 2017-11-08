#ifndef _ROOM_H
#define _ROOM_H

#include <SDL.h>
#include <vector>

#include "Units.h"
#include "Geometry/Rectangle.h"

class Shape;
class Graphics;

class Room {
private: 
	std::vector< std::vector<Rectangle> > background_tiles_; 
	std::vector< std::vector<bool> > is_marked_;
public:
	static const Uint8 HEIGHT;
	static const Uint8 WIDTH;

	Room();
	~Room();

	// Find all tiles in the map that collide with the given collider traveling from origin by delta.
	// Returns a vector of CollisionTiles in the order that the collider traverses them.
	void markCollidingTiles(const Shape& collider, const units::Coordinate2D origin, const units::Coordinate2D delta);
	// Reset the tiles to be normal colour.
	void reset();

	// Render the viewable portion of the level to the screen.
	void draw(Graphics& graphics);
};

#endif // _ROOM_H