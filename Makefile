CXX = g++
CXXFLAGS = -w -std=c++20 -Wfatal-errors

IFLAGS = -I.
LDFLAGS = -L.
ILIBS = "./radix/lradix"
LDLIBS = "./radix/lradix" -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

build:
	$(CXX) $(CXXFLAGS) ./src/*.cpp -o ./game  $(IFLAGS) $(ILIBS) $(LDFLAGS) $(LDLIBS);

clean:
	rm -f ./game;

check:
	cppcheck --quiet --enable=all --force --error-exitcode=1 ./src/*.cpp;

run:
	./game
