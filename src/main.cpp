#include "../radix/lib/game.h"

int main(int argc, char* args[])
{
	radix::Game game;

	while(game.is_running() == true)
	{
		game.input();
		game.update();
		game.render();
	}

	game.destroy();

	return 0;
}