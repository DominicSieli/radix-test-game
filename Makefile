build:
	mkdir -p bin;
	g++ -w -std=c++23 -Wall -Wextra -Wpedantic -Wfatal-errors ./src/*.cpp -o ./bin/game -L. "./radix/lib/lib_radix.a" -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer;

clean:
	rm -f ./bin/game;

check:
	cppcheck --quiet --enable=all --force --error-exitcode=1 ./src/*.cpp;

run:
	./bin/game
