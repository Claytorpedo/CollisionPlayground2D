#include "Room.h"

#include <SDL.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "Graphics.h"
#include "Geometry/Units.h"
#include "Geometry/Constants.h"
#include "Geometry/Util.h"
#include "Geometry/Direction.h"
#include "Geometry/Shape.h"

const Uint8 Room::HEIGHT = 12 * 4;
const Uint8 Room::WIDTH = 20 * 4;

Room::Room() {
	for (Uint8 r = 0; r < HEIGHT; ++r) {
		std::vector<Rectangle> row;
		for (Uint8 c = 0; c < WIDTH; ++c) {
			Rectangle rect(util::tileToCoord(c), util::tileToCoord(r), util::tileToCoord(1), util::tileToCoord(1));
			row.push_back(rect);
		}
		background_tiles_.push_back(row);
		is_marked_.push_back(std::vector<bool>(WIDTH));
	}
}
Room::~Room() {}

void Room::markCollidingTiles(const Shape& collider, const units::Coordinate2D origin, const units::Coordinate2D delta) {
	const units::Coordinate2D destination(origin + delta);
	const direction::Direction xDir = delta.x > 0 ? direction::RIGHT : direction::LEFT;
	const direction::Direction yDir = delta.y > 0 ? direction::DOWN  : direction::UP;
	const bool isOuterAxisX = std::abs(delta.x) >= std::abs(delta.y); // We iterate first on the axis with more movement.
	const direction::Direction outerDir = isOuterAxisX ? xDir : yDir;
	const direction::Direction innerDir = isOuterAxisX ? yDir : xDir;

	// Define incrementors for the outer and inner loops. 
	const units::Tile outerIncr = isMax(outerDir) ? 1 : -1;
	const units::Tile innerIncr = isMax(innerDir) ? 1 : -1;

	// Starting value for the outer loop (the side of the bounding box on the opposite side of the delta direction at the origin).
	const units::Tile outerAxisStart = isOuterAxisX ? util::coordToTile(origin.x + collider.side(oppositeDirection(outerDir))) : 
	                                                  util::coordToTile(origin.y + collider.side(oppositeDirection(outerDir)));
	// End value for the outer loop (the side of the bounding box on the side of the delta direction at the destination).
	const units::Tile outerAxisEnd   = isOuterAxisX ? util::coordToTile(destination.x + collider.side(outerDir)) : 
	                                                  util::coordToTile(destination.y + collider.side(outerDir));

	/* To determine where to start and stop for the inner loop, we first find the ultimate starting and stopping values
	   (where the inner loop will start on the first iteration of the outer loop, and where the inner loop will end on 
	   the last iteration of the outler loop), and then calculate the slope of the vector from origin to destination 
	   (for the axis the inner loop is iterating on).

	   Then each update of the outer loop, we can calculate the starting and stopping values for the inner loop by
	   using this slope, and then clamp them by the known ultimate starting and ending values.
	*/

	// Second axis's initial starting position (the side of the bounding box in the opposite direction of the delta direction).
	const units::Tile innerAxisStart = isOuterAxisX ? util::coordToTile(origin.y + collider.side(oppositeDirection(innerDir))) :
	                                                  util::coordToTile(origin.x + collider.side(oppositeDirection(innerDir)));
	// Second axis's final ending position (the side of the bounding box in the delta direction at the destination).
	const units::Tile innerAxisEnd =   isOuterAxisX ? util::coordToTile(destination.y + collider.side(innerDir)) :
	                                                  util::coordToTile(destination.x + collider.side(innerDir));

	// Determine the slope. This is the m in y = mx + b
	const units::Coordinate slope = isOuterAxisX ? (std::abs(delta.x) < constants::EPSILON ? 0.0f : delta.y / delta.x) : 
	                                               (std::abs(delta.y) < constants::EPSILON ? 0.0f : delta.x / delta.y);

	// Determine which side of the tiles (in the array) to check for innerAxisStart and innerAxisEnd.
	// This combined with the tiles from the outer loop lets us determine the p in y = m(p - q) + b, where p - q = x.
	const units::Coordinate innerAxisStartTileSide = isMax(outerDir) ? 0.0f : util::tileToCoord(1);
	const units::Coordinate innerAxisEndTileSide   = isMax(outerDir) ? util::tileToCoord(1) : 0.0f;

	// Determine the slope origins. This acts like q in y = m(p - q) + b, where p - q = x.
	const units::Coordinate innerAxisStartOrigin = isOuterAxisX ? origin.x + collider.side(outerDir) : origin.y + collider.side(outerDir);
	const units::Coordinate innerAxisEndOrigin   = isOuterAxisX ? origin.x + collider.side(oppositeDirection(outerDir)) : 
	                                                              origin.y + collider.side(oppositeDirection(outerDir));

	// Determine where to offset the slopes from. This acts like the b in y = mx + b.
	const units::Coordinate innerAxisStartOffset = isOuterAxisX ? origin.y + collider.side(oppositeDirection(innerDir)) :
	                                                              origin.x + collider.side(oppositeDirection(innerDir));
	const units::Coordinate innerAxisEndOffset   = isOuterAxisX ? origin.y + collider.side(innerDir) : origin.x + collider.side(innerDir);

	for (units::Tile i = outerAxisStart; i != outerAxisEnd + outerIncr; i += outerIncr) {
		// Calculate the limits for the inner loop. This applies our y = mx + b (if solving for x, we essentially rotate the world and then solve for y).
		// ..................... axis     =                    m    * ( (                   p                      ) -        q            ) +       b
		const units::Tile calculatedStart = util::coordToTile(slope * (util::tileToCoord(i) + innerAxisStartTileSide - innerAxisStartOrigin) + innerAxisStartOffset);
		const units::Tile calculatedEnd =   util::coordToTile(slope * (util::tileToCoord(i) + innerAxisEndTileSide   - innerAxisEndOrigin  ) + innerAxisEndOffset);
		// Clamp values by limits.
		const units::Tile start = isMax(innerDir) ? ( calculatedStart < innerAxisStart ? innerAxisStart : calculatedStart ) :
		                                            ( calculatedStart > innerAxisStart ? innerAxisStart : calculatedStart);
		const units::Tile end =   isMax(innerDir) ? ( calculatedEnd > innerAxisEnd ? innerAxisEnd : calculatedEnd ) :
		                                            ( calculatedEnd < innerAxisEnd ? innerAxisEnd : calculatedEnd );

		for (units::Tile k = start; k != end + innerIncr; k += innerIncr) {
			const units::Tile row = isOuterAxisX ? k : i;
			const units::Tile col = isOuterAxisX ? i : k;
			if ( row >= 0 && col >= 0 &&
				row < static_cast<units::Tile>(background_tiles_.size()) && col < static_cast<units::Tile>(background_tiles_[row].size()) ) // Bounds check.
				is_marked_[row][col] = true;
		}
	}
}

void Room::reset() {
	for (std::size_t r = 0; r < is_marked_.size(); ++r) {
		for (std::size_t c = 0; c < is_marked_[r].size(); ++c) {
			is_marked_[r][c] = false;
		}
	}
}

namespace {
	inline SDL_Rect _convert_to_SDL_rect(Rectangle r) {
		SDL_Rect rect = { static_cast<int>(r.x), static_cast<int>(r.y), static_cast<int>(r.w), static_cast<int>(r.h) };
		return rect;
	}
}
void Room::draw(Graphics& graphics) {
	for ( std::size_t row = 0 ; row < background_tiles_.size(); ++row ) {
		for ( std::size_t col = 0; col < background_tiles_[row].size(); ++col ) {
			if (is_marked_[row][col])
				graphics.setRenderColour(255, 0, 0);
			else
				graphics.setRenderColour(0, 0, 255);
			graphics.renderRect(_convert_to_SDL_rect(background_tiles_[row][col]));
		}
	}
}