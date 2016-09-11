#ifndef _COLLIDABLE_H
#define _COLLIDABLE_H

class Shape;

// Parent class for moving objects that may be involved with collisions.

class Collidable {
public:
	Collidable(){}
	~Collidable(){}

	void move(const Shape& collider, const units::Coordinate2D origin, const units::Coordinate2D delta, Room* level, bool shouldDeflect=false);


	//virtual void onCollision(const CollisionTile::CollisionInfo& collisionInfo) = 0;
	//virtual void onDelta(const CollisionTile::CollisionInfo& collisionInfo) = 0;
};

#endif //_COLLIDABLE_H