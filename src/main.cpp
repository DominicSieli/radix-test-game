#include "../radix/src/game.h"

int main(int argc, char* args[])
{
	Radix::Game game;

	while(game.state_manager.is_running())
	{
		game.input();
		game.update();
		game.render();
	}

	game.destroy();

	return 0;
}