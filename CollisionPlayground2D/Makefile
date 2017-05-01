SRCDIR = .
GEOMDIR = ./Geometry
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(GEOMDIR)/*.cpp)
ODIR = build
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))
 
PROG = collisions

CC = g++

COMP_FLAGS = -std=c++11 -Wall -pedantic
# Any linker flags, such as libraries to link to (pkg-config --libs).
LINK_FLAGS = `pkg-config --libs sdl2`

# Any additional include dirs, such as for libraries (pkg-config --cflags).
INCL_DIRS = `pkg-config --cflags sdl2`

DEBUG_FLAGS = -DDEBUG -g

.PHONY: all clean dir debug

all: dir $(PROG)

$(PROG): $(OBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@

$(OBJS): $(ODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

dir:
	mkdir -p $(ODIR)
	mkdir -p $(ODIR)/$(GEOMDIR)

clean:
	rm -rf $(ODIR) $(PROG)
