SRCS = main.cpp Graphics.cpp Input.cpp Room.cpp Geometry/CollisionMath.cpp Geometry/Polygon.cpp Geometry/Shape.cpp
#ODIR = objects
#OBJS = $(subst .cpp,.o,$(SRCS))
 
PROG = collisions

CC = g++

COMP_FLAGS = -std=c++11 -Wall

# Find SDL2 include directory in linux.
INCL_DIR = `pkg-config --cflags sdl2`

# Find SDL2 library in linux.
LINK_FLAGS = `pkg-config --libs sdl2`

all: $(SRCS)
	$(CC) ${INCL_DIR} $(SRCS) $(COMP_FLAGS) $(LINK_FLAGS) -o $(PROG)

clean:

	rm -f $(PROG) $(OBJS)
