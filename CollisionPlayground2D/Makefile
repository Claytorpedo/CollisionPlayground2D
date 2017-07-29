SRCDIR = .
GEOMDIR = $(SRCDIR)/Geometry
GEOMSRC = $(wildcard $(GEOMDIR)/*.cpp)
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(GEOMSRC)
ODIR = $(SRCDIR)/build
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))

TESTDIR = $(SRCDIR)/test
TESTODIR = $(ODIR)
TESTSRCS = $(wildcard $(TESTDIR)/*.cpp) $(GEOMSRC)
TESTOBJS = $(patsubst $(SRCDIR)/%.cpp,$(TESTODIR)/%.o,$(TESTSRCS))

PROG = collisions

CC = g++

COMP_FLAGS = -std=c++11 -Wall -pedantic
# Any linker flags, such as libraries to link to (pkg-config --libs).
LINK_FLAGS = `pkg-config --libs sdl2`

# Any additional include dirs, such as for libraries (pkg-config --cflags).
INCL_DIRS = `pkg-config --cflags sdl2`

DEBUG_FLAGS = -DDEBUG -g

.PHONY: all clean dir testdir debug test

all: dir $(PROG)

$(PROG): $(OBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@

$(OBJS): $(ODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

# test:
# Similar to all, but looks in TEST DIR and builds whatever test files there are (maybe only one?)
# build things into build/test
# runtest: just execute the test file
test: dir testdir runtests

runtests: $(TESTOBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@
	
$(TESTOBJS): $(TESTODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

testdir:
	mkdir -p $(ODIR)/$(TESTDIR)

dir:
	mkdir -p $(ODIR)
	mkdir -p $(ODIR)/$(GEOMDIR)

clean:
	rm -rf $(ODIR) $(PROG)
