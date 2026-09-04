OUT = -o
DIR = bin
CXX = g++
BIN = game
IFLAGS = -I
LFLAGS = -L.
STD = -std=c++23
CHECK = cppcheck
SRC = ./src/*.cpp
CHECKSTD = --std=c++23
OPTIMIZATION = -O3 -flto
BINPATH = ./$(DIR)/$(BIN)
RADIX_H = ./radix/src
RADIX_CPP = ./radix/src/*.cpp
RADIX = "./radix/lib/lib_radix.a"
SDL = -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer
WARNINGS = -w -Wall -Wextra -Wpedantic -Wfatal-errors
CHECKFLAGS = --quiet --enable=all --force --error-exitcode=1

build:
	mkdir -p $(DIR);
	$(CXX) $(STD) $(WARNINGS) $(OPTIMIZATION) $(SRC) $(RADIX_CPP) $(IFLAGS) $(RADIX_H) $(LFLAGS) $(SDL) $(OUT) $(BINPATH);

dev:
	mkdir -p $(DIR);
	$(CXX) $(STD) $(WARNINGS) $(SRC) $(LFLAGS) $(RADIX) $(SDL) $(OUT) $(BINPATH);
	$(BINPATH)

clean:
	rm -rf $(DIR);

check:
	$(CHECK) $(CHECKSTD) $(CHECKFLAGS) $(SRC);

run:
	$(BINPATH)
