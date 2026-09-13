#include "../radix/src/game.h"

using namespace radix;

int main(int argc, char* args[])
{
	Game game;

	while(game.is_running() == true)
	{
		game.input();
		game.update();
		game.render();
	}

	game.destroy();

	return 0;
}