#include "../radix/headers/game.h"

int main(int argc, char* args[])
{
	Radix::Game game;

	while(game.Running() == true)
	{
		game.Input();
		game.Update();
		game.Render();
	}

	game.Destroy();

	return 0;
}