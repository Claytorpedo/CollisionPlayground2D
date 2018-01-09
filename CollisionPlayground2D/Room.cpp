#include "Room.hpp"

#include <SDL.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "units.hpp"
#include "util.hpp"

#include "Graphics.hpp"
#include "Geometry/units.hpp"
#include "Geometry/constants.hpp"
#include "Geometry/Direction.hpp"
#include "Geometry/Shape.hpp"

const Uint8 Room::HEIGHT = 12 * 4;
const Uint8 Room::WIDTH = 20 * 4;

Room::Room() {
	for (Uint8 r = 0; r < HEIGHT; ++r) {
		std::vector<geom::Rect> row;
		for (Uint8 c = 0; c < WIDTH; ++c) {
			geom::Rect rect(game::util::tileToCoord(c), game::util::tileToCoord(r), game::util::tileToCoord(1), game::util::tileToCoord(1));
			row.push_back(rect);
		}
		background_tiles_.push_back(row);
		is_marked_.push_back(std::vector<bool>(WIDTH));
	}
}
Room::~Room() {}

void Room::markCollidingTiles(const geom::Shape& collider, const geom::Coord2& origin, const geom::Coord2& delta) {
	const geom::Coord2 destination(origin + delta);
	const geom::Direction xDir = delta.x > 0 ? geom::Direction::RIGHT : geom::Direction::LEFT;
	const geom::Direction yDir = delta.y > 0 ? geom::Direction::DOWN  : geom::Direction::UP;
	const bool isOuterAxisX = std::abs(delta.x) >= std::abs(delta.y); // We iterate first on the axis with more movement.
	const geom::Direction outerDir = isOuterAxisX ? xDir : yDir;
	const geom::Direction innerDir = isOuterAxisX ? yDir : xDir;

	// Define incrementors for the outer and inner loops. 
	const game::Tile outerIncr = isMax(outerDir) ? 1 : -1;
	const game::Tile innerIncr = isMax(innerDir) ? 1 : -1;

	// Starting value for the outer loop (the side of the bounding box on the opposite side of the delta direction at the origin).
	const game::Tile outerAxisStart = isOuterAxisX ? game::util::coordToTile(origin.x + collider.side(oppositeDirection(outerDir))) : 
	                                                 game::util::coordToTile(origin.y + collider.side(oppositeDirection(outerDir)));
	// End value for the outer loop (the side of the bounding box on the side of the delta direction at the destination).
	const game::Tile outerAxisEnd   = isOuterAxisX ? game::util::coordToTile(destination.x + collider.side(outerDir)) : 
	                                                 game::util::coordToTile(destination.y + collider.side(outerDir));

	/* To determine where to start and stop for the inner loop, we first find the ultimate starting and stopping values
	   (where the inner loop will start on the first iteration of the outer loop, and where the inner loop will end on 
	   the last iteration of the outler loop), and then calculate the slope of the vector from origin to destination 
	   (for the axis the inner loop is iterating on).

	   Then each update of the outer loop, we can calculate the starting and stopping values for the inner loop by
	   using this slope, and then clamp them by the known ultimate starting and ending values.
	*/

	// Second axis's initial starting position (the side of the bounding box in the opposite direction of the delta direction).
	const game::Tile innerAxisStart = isOuterAxisX ? game::util::coordToTile(origin.y + collider.side(oppositeDirection(innerDir))) :
	                                                 game::util::coordToTile(origin.x + collider.side(oppositeDirection(innerDir)));
	// Second axis's final ending position (the side of the bounding box in the delta direction at the destination).
	const game::Tile innerAxisEnd =   isOuterAxisX ? game::util::coordToTile(destination.y + collider.side(innerDir)) :
	                                                 game::util::coordToTile(destination.x + collider.side(innerDir));

	// Determine the slope. This is the m in y = mx + b
	const geom::gFloat slope = isOuterAxisX ? (std::abs(delta.x) < geom::constants::EPSILON ? 0.0f : delta.y / delta.x) :
	                                          (std::abs(delta.y) < geom::constants::EPSILON ? 0.0f : delta.x / delta.y);

	// Determine which side of the tiles (in the array) to check for innerAxisStart and innerAxisEnd.
	// This combined with the tiles from the outer loop lets us determine the p in y = m(p - q) + b, where p - q = x.
	const geom::gFloat innerAxisStartTileSide = isMax(outerDir) ? 0.0f : game::util::tileToCoord(1);
	const geom::gFloat innerAxisEndTileSide   = isMax(outerDir) ? game::util::tileToCoord(1) : 0.0f;

	// Determine the slope origins. This acts like q in y = m(p - q) + b, where p - q = x.
	const geom::gFloat innerAxisStartOrigin = isOuterAxisX ? origin.x + collider.side(outerDir) : origin.y + collider.side(outerDir);
	const geom::gFloat innerAxisEndOrigin   = isOuterAxisX ? origin.x + collider.side(oppositeDirection(outerDir)) :
	                                                              origin.y + collider.side(oppositeDirection(outerDir));

	// Determine where to offset the slopes from. This acts like the b in y = mx + b.
	const geom::gFloat innerAxisStartOffset = isOuterAxisX ? origin.y + collider.side(oppositeDirection(innerDir)) :
	                                                              origin.x + collider.side(oppositeDirection(innerDir));
	const geom::gFloat innerAxisEndOffset   = isOuterAxisX ? origin.y + collider.side(innerDir) : origin.x + collider.side(innerDir);

	for (game::Tile i = outerAxisStart; i != outerAxisEnd + outerIncr; i += outerIncr) {
		// Calculate the limits for the inner loop. This applies our y = mx + b (if solving for x, we essentially rotate the world and then solve for y).
		// ..................... axis     =                    m    * ( (                   p                      ) -        q            ) +       b
		const game::Tile calculatedStart = game::util::coordToTile(slope * (game::util::tileToCoord(i) + innerAxisStartTileSide - innerAxisStartOrigin) + innerAxisStartOffset);
		const game::Tile calculatedEnd =   game::util::coordToTile(slope * (game::util::tileToCoord(i) + innerAxisEndTileSide   - innerAxisEndOrigin  ) + innerAxisEndOffset);
		// Clamp values by limits.
		const game::Tile start = isMax(innerDir) ? ( calculatedStart < innerAxisStart ? innerAxisStart : calculatedStart ) :
		                                           ( calculatedStart > innerAxisStart ? innerAxisStart : calculatedStart);
		const game::Tile end =   isMax(innerDir) ? ( calculatedEnd > innerAxisEnd ? innerAxisEnd : calculatedEnd ) :
		                                           ( calculatedEnd < innerAxisEnd ? innerAxisEnd : calculatedEnd );

		for (game::Tile k = start; k != end + innerIncr; k += innerIncr) {
			const game::Tile row = isOuterAxisX ? k : i;
			const game::Tile col = isOuterAxisX ? i : k;
			if ( row >= 0 && col >= 0 &&
				row < static_cast<game::Tile>(background_tiles_.size()) && col < static_cast<game::Tile>(background_tiles_[row].size()) ) // Bounds check.
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
	inline SDL_Rect _convert_to_SDL_rect(geom::Rect r) {
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