SRCDIR := CollisionPlayground2D

EXNAME := geom_examples

SRCS := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/$(EXNAME)/*.cpp)
ODIR := build
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))

# Handle dependencies on header files.
DEPS := $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.d,$(SRCS))

MKDIRS := $(ODIR) $(ODIR)/$(EXNAME)

EXTERNAL := External
GEOM := $(EXTERNAL)/Geometry2D
SUBMODS := $(GEOM)
SUBMODCMD := $(MAKECMDGOALS)

PROG := examples

CC = g++
COMP_FLAGS = -std=c++17 -Wall -Wextra -pedantic
DEPS_FLAGS = -MMD -MP
LINK_FLAGS = `pkg-config --libs sdl2` -L$(GEOM)/lib/ -lgeom
INCL_DIRS = `pkg-config --cflags sdl2` -I $(EXTERNAL)

DEBUG_FLAGS = -DDEBUG -g

##To compile thread-safe version of submodule Geometry, set THREADED=TRUE
export THREADED

.PHONY: $(SUBMODS)
.PHONY: all debug clean cleanall clean_submods help

all:            ## Build the examples.
all: $(MKDIRS) $(SUBMODS) $(PROG)

$(MKDIRS):
	@mkdir -p $@

$(SUBMODS):
	$(MAKE) -C $@ $(SUBMODCMD)

$(PROG): $(OBJS) | $(SUBMODS)
	$(CC) $^ $(LINK_FLAGS) -o $@

$(OBJS): $(ODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(INCL_DIRS) $(COMP_FLAGS) $(DEPS_FLAGS) -c $< -o $@

debug:          ## Make debug build.
debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

clean:          ## Clean this project and all submodules.
clean: clean_local clean_submods

clean_local:    ## Clean this project, ignoring submodules.
	rm -rf $(ODIR) $(PROG)

clean_submods:  ## Clean only submodules.
clean_submods: SUBMODCMD := clean
clean_submods: $(SUBMODS)

help:           ## Display this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

# Include all dependency files. Use "-" flavour because they might not exist yet.
-include $(DEPS)