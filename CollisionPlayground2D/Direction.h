#ifndef _DIRECTION_H
#define _DIRECTION_H


namespace direction {
   enum Direction {
     UP,
	 DOWN,
	 LEFT,
	 RIGHT,
	 NONE,
	 TOTAL_DIRECTIONS
   };

   inline Direction oppositeDirection(Direction direction) {
	   if (direction == UP)
		   return DOWN;
	   if (direction == DOWN)
		   return UP;
	   if (direction == LEFT)
		   return RIGHT;
	   if (direction == RIGHT)
		   return LEFT;
	   return NONE;
   }
   inline bool isHorizontal(Direction direction) { return direction == LEFT || direction == RIGHT;}
   inline bool isVertical(Direction direction)	 { return direction == UP   || direction == DOWN;}
   inline bool hasDirection(Direction direction) { return isHorizontal(direction) || isVertical(direction);}

   inline bool isMax(Direction side) { return side == RIGHT || side == DOWN; }
   inline bool isMin(Direction side) { return side == LEFT  || side == UP; }
}

#endif // _DIRECTION_H