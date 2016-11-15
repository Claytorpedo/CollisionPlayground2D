SRCS = main.cpp Graphics.cpp Input.cpp Room.cpp Geometry/CollisionMath.cpp Geometry/Polygon.cpp Geometry/Shape.cpp
#ODIR = objects
#OBJS = $(subst .cpp,.o,$(SRCS))


PROG = collisions

CC = g++

COMP_FLAGS = -std=c++11 -Wall

LINK_FLAGS = -lSDL2

all: $(SRCS)
	$(CC) $(SRCS) $(COMP_FLAGS) $(LINK_FLAGS) -o $(PROG)

#$(OBJDIR):
#	mkdir $(OBJDIR)

clean:

	rm -f $(PROG) $(OBJS)
