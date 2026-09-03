OUT = -o
DIR = bin
CXX = g++
BIN = game
IFLAGS = -I
LFLAGS = -L.
STD = -std=c++23
CHECK = cppcheck
SRC = ./src/*.cpp
OPTIMIZATION = -O3 -flto
BINPATH = ./$(DIR)/$(BIN)
RADIX_HEADERS = ../radix/src
RADIX_SRC = ../radix/src/*.cpp
LIB_RADIX = "./radix/lib/lib_radix.a"
SDL = -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer
WARNINGS = -w -Wall -Wextra -Wpedantic -Wfatal-errors
CHECKFLAGS = --quiet --enable=all --force --error-exitcode=1

build:
	mkdir -p $(DIR);
	$(CXX) $(STD) $(WARNINGS) $(OPTIMIZATION) $(SRC) $(RADIX_SRC) $(IFLAGS) $(RADIX_HEADERS) $(LFLAGS) $(SDL) $(OUT) $(BINPATH);

dev:
	mkdir -p $(DIR);
	$(CXX) $(STD) $(WARNINGS) $(SRC) $(LFLAGS) $(LIB_RADIX) $(SDL) $(OUT) $(BINPATH);
	$(BINPATH)

clean:
	rm -rf $(DIR);

check:
	$(CHECK) $(CHECKFLAGS) $(SRC);

run:
	$(BINPATH)
