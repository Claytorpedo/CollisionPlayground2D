#include "Room.h"

#include <SDL.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "Graphics.h"
#include "Units.h"
#include "Constants.h"
#include "Util.h"
#include "Direction.h"
#include "Geometry/Shape.h"

using namespace direction;

Room::Room() {
	for (Uint8 r = 0; r < room::height; ++r) {
		std::vector<Rectangle> row;
		for (Uint8 c = 0; c < room::width; ++c) {
			row.push_back(Rectangle(util::tileToCoord(c), util::tileToCoord(r), util::tileToCoord(1), util::tileToCoord(1)));
		}
		background_tiles_.push_back(row);
		is_marked_.push_back(std::vector<bool>(room::width));
	}
}
Room::~Room() {}

void Room::markCollidingTiles(const Shape& collider, const units::Coordinate2D origin, const units::Coordinate2D delta) {
	const units::Coordinate2D destination(origin + delta);
	const Direction xDir = delta.x > 0 ? RIGHT : LEFT;
	const Direction yDir = delta.y > 0 ? DOWN  : UP;

	// Define prarameters for the outer loop. They will stay the same.
	const bool isFirstAxisX = std::abs(delta.x) > std::abs(delta.y);
	const units::Tile firstIncr =  isFirstAxisX ? ( xDir == RIGHT ? 1 : -1 ) : ( yDir == DOWN  ? 1 : -1);
	const units::Tile secondIncr = isFirstAxisX ? ( yDir == DOWN  ? 1 : -1 ) : ( xDir == RIGHT ? 1 : -1);

	// Starting value for the outer loop (the side of the box on the opposite side of the delta direction at the origin).
	const units::Tile firstAxisStart = isFirstAxisX ? util::coordToTile(origin.x + collider.side(oppositeDirection(xDir))) : 
		                                              util::coordToTile(origin.y + collider.side(oppositeDirection(yDir)));
	// End value for the outer loop (the side of the box on the side of the delta direction at the destination).
	const units::Tile firstAxisEnd =   isFirstAxisX ? util::coordToTile(destination.x + collider.side(xDir)) : 
		                                              util::coordToTile(destination.y + collider.side(yDir));

	/* To determine where to start and stop for the second loop, we first find the ultimate starting and stopping values
	   (where the inner loop will start on the first iteration of the outer loop, and where the inner loop will end on 
	   the last iteration of the outler loop), and then calculate the slope of the vector from origin to destination 
	   (for the axis the inner loop is iterating on).

	   Then each update of the outer loop, we can calculate the starting and stopping values for the inner loop by
	   using this slope, and then clamp them by the known ultimate starting and ending values.
	*/

	// Second axis's initial starting position (the side of the bounding box in the opposite direction of the delta direction).
	const units::Tile secondAxisStart = isFirstAxisX ? util::coordToTile(origin.y + collider.side(oppositeDirection(yDir))) :
		                                               util::coordToTile(origin.x + collider.side(oppositeDirection(xDir)));
	// Second axis's final ending position (the side of the bounding box in the delta direction at the destination).
	const units::Tile secondAxisEnd =   isFirstAxisX ? util::coordToTile(destination.y + collider.side(yDir)) :
		                                               util::coordToTile(destination.x + collider.side(xDir));

	// Determine the slope. This is the m in y = mx + b
	const units::Coordinate slope = isFirstAxisX ? (std::abs(delta.x) < constants::EPSILON ? 0.0f : delta.y / delta.x) : 
		                                           (std::abs(delta.y) < constants::EPSILON ? 0.0f : delta.x / delta.y);

	// Determine which side of the tiles (in the array) to check for secondAxisStart and secondAxisEnd.
	// This combined with the tiles from the outer loop lets us determine the p in y = m(p - q) + b, where p - q = x.
	const units::Coordinate secondAxisStartTileSide = isFirstAxisX ? xDir == RIGHT ? util::tileToCoord(1) : 0.0f :
		                                                             yDir == DOWN  ? util::tileToCoord(1) : 0.0f;
	const units::Coordinate secondAxisEndTileSide = secondAxisStartTileSide == 0.0f ? util::tileToCoord(1) : 0.0f;

	// Determine the slope origins. This acts like q in y = m(p - q) + b, where p - q = x.
	const units::Coordinate secondAxisStartOrigin = isFirstAxisX ? origin.x + collider.side(xDir) : origin.y + collider.side(yDir);
	const units::Coordinate secondAxisEndOrigin =   isFirstAxisX ? origin.x + collider.side(oppositeDirection(xDir)) : 
		                                                           origin.y + collider.side(oppositeDirection(yDir));

	// Determine where to offset the slopes from. This acts like the b in y = mx + b.
	const units::Coordinate secondAxisStartOffset = isFirstAxisX ? origin.y + collider.side(oppositeDirection(yDir)) :
		                                                           origin.x + collider.side(oppositeDirection(xDir));
	const units::Coordinate secondAxisEndOffset =   isFirstAxisX ? origin.y + collider.side(yDir) : origin.x + collider.side(xDir);

	for (units::Tile i = firstAxisStart; i != firstAxisEnd + firstIncr; i += firstIncr) {
		// Calculate the limits for the second loop. This applies our y = mx + b (if solving for x, we essentially rotate the world and then solve for y).
		// ..................... axis     =                    m    * ( (                   p                      )  -       q              ) +     b
		const units::Tile calculatedStart = util::coordToTile(slope * (util::tileToCoord(i) + secondAxisStartTileSide - secondAxisStartOrigin) + secondAxisStartOffset);
		const units::Tile calculatedEnd =   util::coordToTile(slope * (util::tileToCoord(i) + secondAxisEndTileSide   - secondAxisEndOrigin  ) + secondAxisEndOffset);
		// Clamp values by limits.
		const units::Tile start = slope > 0.0f ? ( calculatedStart < secondAxisStart ? secondAxisStart : calculatedStart ) :
			                                     ( calculatedStart > secondAxisStart ? secondAxisStart : calculatedStart);
		const units::Tile end = slope > 0.0f ? ( calculatedEnd > secondAxisEnd ? secondAxisEnd : calculatedEnd ) :
			                                   ( calculatedEnd < secondAxisEnd ? secondAxisEnd : calculatedEnd );

		for (units::Tile k = start; k != end + secondIncr; k += secondIncr) {
			const units::Tile row = isFirstAxisX ? k : i;
			const units::Tile col = isFirstAxisX ? i : k;
			if ( row >= 0 && col >= 0 && row < background_tiles_.size() && col < background_tiles_[row].size() ) // Bounds check.
				is_marked_[row][col] = true;
		}
	}
}

void Room::draw(Graphics& graphics) {

	// Either should have some member of graphics passed, or some camera passed to say what range of the tiles to draw.
	// OPTIMIZE TO ONLY DRAW TILES IN THE CAMERA'S VIEW
	for ( units::Tile row = 0 ; row < background_tiles_.size(); ++row ) {
		for ( units::Tile col = 0; col < background_tiles_[row].size(); ++col ) {
			background_tiles_[row][col].draw(graphics, is_marked_[row][col]);
		}
	}
}