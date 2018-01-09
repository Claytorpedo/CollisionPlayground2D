SRCDIR = .

SRCS = $(wildcard $(SRCDIR)/*.cpp)
ODIR = $(SRCDIR)/build
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))

GEOMDIR = $(SRCDIR)/Geometry
GEOMSRC = $(wildcard $(GEOMDIR)/*.cpp)
GEOMOBJS = $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(GEOMSRC))

TESTDIR = $(SRCDIR)/test
TESTODIR = $(ODIR)
TESTSRCS = $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS = $(patsubst $(SRCDIR)/%.cpp,$(TESTODIR)/%.o,$(TESTSRCS))

PROG = collisions
TESTPROG = $(PROG)_test

CC = g++

COMP_FLAGS = -std=c++11 -Wall -pedantic
# Any linker flags, such as libraries to link to (pkg-config --libs).
LINK_FLAGS = `pkg-config --libs sdl2`

# Any additional include dirs, such as for libraries (pkg-config --cflags).
INCL_DIRS = `pkg-config --cflags sdl2`

DEBUG_FLAGS = -DDEBUG -g

.PHONY: all clean help dir testdir debug test runtest

all: dir $(PROG)

$(PROG): $(GEOMOBJS) $(OBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@

$(OBJS): $(ODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@
$(GEOMOBJS) : $(ODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

test: dir testdir buildtests

runtest: test
	$(SRCDIR)/$(TESTPROG)

buildtests: $(GEOMOBJS) $(TESTOBJS)
	$(CC) $^ $(LINK_FLAGS) -o $(TESTPROG)
	
$(TESTOBJS): $(TESTODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

testdir:
	mkdir -p $(ODIR)/$(TESTDIR)

dir:
	mkdir -p $(ODIR)
	mkdir -p $(ODIR)/$(GEOMDIR)

clean:
	rm -rf $(ODIR) $(PROG) $(TESTPROG)