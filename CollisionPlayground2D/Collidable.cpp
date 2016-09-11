
#include "Units.h"
#include "Geometry/Shape.h"
#include "Direction.h"
#include "Room.h"

#include "Collidable.h"

using namespace direction;

void Collidable::move(const Shape& collider, const Coordinate2D origin, units::Coordinate2D delta, Room* room, bool shouldDeflect) {
	if (delta.isZero()) // Not moving, don't check for collisions.
		return;
	/*
	std::vector<CollisionTile> collisionTiles(level->getCollisionTiles(boundingBox, origin, delta));
	CollisionTile::CollisionInfo closestCollision;
	// Find the closest collision, if any.
	// collision check function returns a bool, a position of where the collision took place, and the normal of the plane/line segment collided with.
	// Save the closest collision (shortest vector from position to collision position). Default constructor sets isCollision to false.
	for (std::size_t i = 0; i < collisionTiles.size(); ++i) {
		 CollisionTile::CollisionInfo collisionInfo = collisionTiles[i].checkCollision(boundingBox, origin, delta);
		//collisionInfo = collisionTiles[i].checkVectorCollision(origin, boundingBox, destination, directions, level);
		if (collisionInfo.isCollision) {
			break;	// If there is a collision, exit the check (take the first collision).
		}
	}
	*/
	// Maybe onCollision and onDelta should take in a vector of all collision infos, in case you are interested in seeing how the object deflected.
	// This would have *potential* use with things like physics.
	// Or have the returned collision info contain the full vector of the moved delta (all the points from origin to where they ended up).


	/*if (collisionInfo.isCollision) {
		onVectorCollision(collisionInfo);
	} else {
		onVectorDelta(CollisionTile::CollisionInfo2D(destination, directions));
	}*/
	//onDelta(closestCollision);
}