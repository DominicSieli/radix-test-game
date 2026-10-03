DIR			 := bin
CXX			 := g++
BIN			 := game
CHECK		 := cppcheck

STD			 := -std=c++23
WARNINGS	 := -Wall -Wextra -Wpedantic -Wfatal-errors
OPTIMIZATION := -O3 -flto

SRC			 := $(wildcard src/*.cpp)
RADIX_SRC	 := $(wildcard radix/src/*.cpp)

RADIX_H		 := ./radix/src
RADIX_LIB	 := ./radix/lib/lib_radix.a
BINPATH		 := $(DIR)/$(BIN)

CPPFLAGS	 := -I$(RADIX_H)
LDFLAGS		 := -L./radix/lib
LDLIBS		 := -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer

.PHONY: link_lib_debug link_lib_optimized build_debug build_optimized clean check run compile_commands

link_lib_debug:
	mkdir -p $(DIR)
	$(CXX) $(STD) $(WARNINGS) $(SRC) $(RADIX_LIB) $(LDFLAGS) $(LDLIBS) -o $(BINPATH)
	$(BINPATH)


link_lib_optimized:
	mkdir -p $(DIR)
	$(CXX) $(STD) $(WARNINGS) $(OPTIMIZATION) $(SRC) $(RADIX_LIB) $(LDFLAGS) $(LDLIBS) -o $(BINPATH)
	$(BINPATH)

build_debug:
	mkdir -p $(DIR)
	$(CXX) $(STD) $(WARNINGS) $(SRC) $(RADIX_SRC) $(CPPFLAGS) $(LDLIBS) -o $(BINPATH)
	$(BINPATH)

build_optimized:
	mkdir -p $(DIR)
	$(CXX) $(STD) $(WARNINGS) $(OPTIMIZATION) $(SRC) $(RADIX_SRC) $(CPPFLAGS) $(LDLIBS) -o $(BINPATH)
	$(BINPATH)

clean:
	rm -rf $(DIR)
	rm -rf .cache
	rm -f compile_commands.json

check:
	bear -- make
	$(CHECK) $(SRC) $(RADIX_SRC)

run:
	$(BINPATH)

compile_commands:
	bear -- make
