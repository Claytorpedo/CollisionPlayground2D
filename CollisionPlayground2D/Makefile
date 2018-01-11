SRCDIR := .

SRCS := $(wildcard $(SRCDIR)/*.cpp)
ODIR := $(SRCDIR)/build
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))

MKDIRS := $(ODIR)

GEOM := $(SRCDIR)/Geometry2D
SUBMODS := $(GEOM)
SUBMODCMD := $(MAKECMDGOALS)

PROG := examples

CC = g++
COMP_FLAGS = -std=c++11 -Wall -pedantic
LINK_FLAGS = `pkg-config --libs sdl2` -L$(GEOM)/lib/ -lgeom
INCL_DIRS = `pkg-config --cflags sdl2`

DEBUG_FLAGS = -DDEBUG -g

##To compile thread-safe version of submodule Geometry, set THREADED=TRUE
export THREADED

.PHONY: $(SUBMODS)
.PHONY: all debug clean cleanall clean_submods help

all:            ## Build the examples.
all: $(MKDIRS) $(SUBMODS) $(PROG)

$(SUBMODS):
	$(MAKE) -C $@ $(SUBMODCMD)

$(PROG): $(OBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@

$(OBJS): $(ODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

debug:          ## Make debug build. 
debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

$(MKDIRS):
	@mkdir -p $@

clean:          ## Clean this project and all submodules.
clean: clean_local clean_submods

clean_local:    ## Clean this project, ignoring submodules.
	rm -rf $(ODIR) $(PROG)

clean_submods:  ## Clean only submodules.
clean_submods: SUBMODCMD := clean
clean_submods: $(SUBMODS)
	
help:           ## Display this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'
