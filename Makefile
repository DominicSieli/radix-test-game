CXX = g++
GAME = ./bin/game
SOURCE = ./src/*.cpp
BUILD_DIRECTORY = bin
BINARY = ./bin/game
CXXFLAGS = -w -std=c++20 -Wall -Wextra -Wpedantic -Wfatal-errors
CHECK_FLAGS = --quiet --enable=all --force --error-exitcode=1

IFLAGS = -I.
LDFLAGS = -L.
ILIBS = "./radix/lib/lib_radix.a"
LDLIBS = "./radix/lib/lib_radix.a" -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

build:
	mkdir -p $(BUILD_DIRECTORY);
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(GAME) $(IFLAGS) $(ILIBS) $(LDFLAGS) $(LDLIBS);

clean:
	rm -f $(GAME);

check:
	cppcheck $(CHECK_FLAGS) $(SOURCE);

run:
	$(GAME)
